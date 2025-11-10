// Integer-only softmax with MAC ILP (LANES=4) + degree-5 polynomial (Estrin, Q40).
// Keeps the same normalization step (per-element division) for now.

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

#define LANES 4

// ---------- Tunables ----------
#define QCOEF   40          // fixed-point for coefficients (Q40)
#define RSHIFT   8          // scale down residual r = (vs - q*vln2) >> RSHIFT
// ------------------------------

// exp(x) ~ c0 + c1 x + c2 x^2 + c3 x^3 + c4 x^4 + c5 x^5 (defaults: Taylor @0)
static const uint64_t C0_Q = (1ULL<<QCOEF);          // 1
static const uint64_t C1_Q = (1ULL<<QCOEF);          // 1
static const uint64_t C2_Q = (1ULL<<(QCOEF-1));      // 1/2
static const uint64_t C3_Q = (1ULL<<QCOEF) / 6;      // 1/6
static const uint64_t C4_Q = (1ULL<<QCOEF) / 24;     // 1/24
static const uint64_t C5_Q = (1ULL<<QCOEF) / 120;    // 1/120

// Degree-2 constants you already had (for Barrett scaling etc.)
static const double A_coef = 0.3585;
static const double B_coef = 1.353;
static const double C_coef = 0.344;
static const double LN2   = 0.6931471805599453094;

// Custom MAC: rd := rd + rs1*rs2 (low 64)
#define MACACC_R(dest, x, y) \
  __asm__ (".insn r 0x5B, 0x02, 0x00, %0, %1, %2" : "+&r"(dest) : "r"(x), "r"(y))

// Safe add with clamp to non-negative
static inline uint64_t clampu64_nonneg(int64_t x) { return (x < 0) ? 0ULL : (uint64_t)x; }

// Evaluate degree-5 Estrin polynomial in fixed-point QCOEF on integer r
// r_in is already scaled down by RSHIFT (i.e., r_in = (vs - q*vln2) >> RSHIFT).
static inline uint64_t poly5_estrin_mac(int64_t r_in)
{
    // Powers via MAC (to exercise your FU and avoid mul):
    // r2 = r^2, r4 = r2^2. For r3, r5 we fold inside Estrin (not needed explicitly).
    uint64_t r2 = 0;                 // r^2
    MACACC_R(r2, r_in, r_in);

    uint64_t r4 = 0;                 // r^4 = r2 * r2
    MACACC_R(r4, (int64_t)r2, (int64_t)r2);

    // e0 = c0 + c1*r
    uint64_t tmp = 0;                // tmp = c1*r  (QCOEF)
    MACACC_R(tmp, r_in, (int64_t)C1_Q);
    __int128 e0 = (__int128)C0_Q + (__int128)tmp; // QCOEF

    // e1 = c2 + c3*r
    tmp = 0;
    MACACC_R(tmp, r_in, (int64_t)C3_Q);
    __int128 e1 = (__int128)C2_Q + (__int128)tmp; // QCOEF

    // e2 = c4 + c5*r
    tmp = 0;
    MACACC_R(tmp, r_in, (int64_t)C5_Q);
    __int128 e2 = (__int128)C4_Q + (__int128)tmp; // QCOEF

    // termA = (e2 * r2) >> QCOEF   (still QCOEF after shift)
    uint64_t acc = 0;
    MACACC_R(acc, (int64_t)r2, (int64_t)e2);      // low64(e2 * r2)
    __int128 termA = (__int128)acc >> QCOEF;      // QCOEF

    // termB = termA + e1           (QCOEF)
    __int128 termB = termA + e1;                  // QCOEF

    // termC = (termB * r2) >> QCOEF (QCOEF)
    acc = 0;
    MACACC_R(acc, (int64_t)r2, (int64_t)termB);
    __int128 termC = (__int128)acc >> QCOEF;      // QCOEF

    // poly_Q = termC + e0          (QCOEF)
    __int128 poly_Q = termC + e0;                 // QCOEF

    // Convert back to integer (drop QCOEF). Clamp to non-negative.
    int64_t poly = (int64_t)(poly_Q >> QCOEF);
    return clampu64_nonneg(poly);
}

__attribute__((noinline, used))
int int_softmax_mac_ilp(const int32_t * __restrict v, int N, int M, double S,
                        uint8_t * __restrict out8,
                        uint64_t * __restrict mac_hash_out)
{
    if (N <= 0) return -1;
    if (M <= 0 || M > 30) return -2;

    // 1) vmax
    int32_t vmax = v[0];
    for (int i = 1; i < N; ++i) if (v[i] > vmax) vmax = v[i];

    // 2) vstable = v - vmax
    int32_t *vstable = (int32_t*)malloc((size_t)N * sizeof(int32_t));
    if (!vstable) return -3;
    for (int i = 0; i < N; ++i) vstable[i] = v[i] - vmax;

    // 3) integer constants (Barrett + old poly constants)
    int64_t vln2 = (int64_t)(LN2 / S); if (vln2 <= 0) vln2 = 1;

    __int128 base  = (__int128)1 << (2 * M);
    __int128 mu128 = base / (__int128)vln2; if (mu128 <= 0) mu128 = 1;
    int64_t  mu    = (int64_t)mu128;

    // vb, vc kept only for compatibility (not used by the new poly path)
    int64_t vb = (int64_t)(B_coef / S);
    double  denom = A_coef * S * S; if (denom <= 0.0) denom = 1e-12;
    int64_t vc = (int64_t)(denom > 0 ? (C_coef / denom) : 0);

    // 4) approx exp buffer
    int64_t *vapprox = (int64_t*)malloc((size_t)N * sizeof(int64_t));
    if (!vapprox) { free(vstable); return -4; }

    uint64_t mac_hash = 0;

    // ---- main loop, 4-lane blocked ----
    int i = 0;
    int n_vec = (N / LANES) * LANES;

    for (; i < n_vec; i += LANES) {
        // Gather lanes
        int32_t vs0 = vstable[i+0], vs1 = vstable[i+1];
        int32_t vs2 = vstable[i+2], vs3 = vstable[i+3];

        // Barrett q = floor(vs * mu / 2^(2M))
        __int128 p0 = (__int128)vs0 * (__int128)mu;
        __int128 p1 = (__int128)vs1 * (__int128)mu;
        __int128 p2 = (__int128)vs2 * (__int128)mu;
        __int128 p3 = (__int128)vs3 * (__int128)mu;

        int64_t q0 = (int64_t)(p0 >> (2 * M));
        int64_t q1 = (int64_t)(p1 >> (2 * M));
        int64_t q2_ = (int64_t)(p2 >> (2 * M));
        int64_t q3_ = (int64_t)(p3 >> (2 * M));

        // Residuals r = vs - q*vln2   (NOTE: we do NOT add vb here)
        int64_t r0 = (int64_t)vs0 - q0 * vln2;
        int64_t r1 = (int64_t)vs1 - q1 * vln2;
        int64_t r2r = (int64_t)vs2 - q2_ * vln2;
        int64_t r3r = (int64_t)vs3 - q3_ * vln2;

        // Downscale residual to keep powers small
        int64_t rr0 = r0 >> RSHIFT;
        int64_t rr1 = r1 >> RSHIFT;
        int64_t rr2 = r2r >> RSHIFT;
        int64_t rr3 = r3r >> RSHIFT;

        // ---- Degree-5 polynomial via Estrin, all with MAC multiplies ----
        uint64_t appr0 = poly5_estrin_mac(rr0);
        uint64_t appr1 = poly5_estrin_mac(rr1);
        uint64_t appr2 = poly5_estrin_mac(rr2);
        uint64_t appr3 = poly5_estrin_mac(rr3);

        // Hash the MAC-visible results (observability)
        mac_hash ^= (appr0 ^ appr1 ^ appr2 ^ appr3);

        // Final 2^{-q} scaling like before; guard negative/large shifts
        int64_t s0 = (q0 < 0) ? 0 : q0;
        int64_t s1 = (q1 < 0) ? 0 : q1;
        int64_t s2 = (q2_ < 0) ? 0 : q2_;
        int64_t s3 = (q3_ < 0) ? 0 : q3_;

        uint64_t out0 = (s0 >= 63) ? 0ULL : (appr0 >> s0);
        uint64_t out1 = (s1 >= 63) ? 0ULL : (appr1 >> s1);
        uint64_t out2 = (s2 >= 63) ? 0ULL : (appr2 >> s2);
        uint64_t out3 = (s3 >= 63) ? 0ULL : (appr3 >> s3);

        vapprox[i+0] = (int64_t)out0;
        vapprox[i+1] = (int64_t)out1;
        vapprox[i+2] = (int64_t)out2;
        vapprox[i+3] = (int64_t)out3;
    }

    // tail (scalar)
    for (; i < N; ++i) {
        int32_t vs = vstable[i];
        __int128 p = (__int128)vs * (__int128)mu;
        int64_t  q = (int64_t)(p >> (2 * M));
        int64_t  r = (int64_t)vs - q * vln2;
        int64_t  rr = r >> RSHIFT;

        uint64_t appr = poly5_estrin_mac(rr);
        mac_hash ^= appr;

        int64_t s = (q < 0) ? 0 : q;
        uint64_t outv = (s >= 63) ? 0ULL : (appr >> s);
        vapprox[i] = (int64_t)outv;
    }

    // 6) sum & normalize (unchanged)
    __int128 sum = 0;
    for (int j = 0; j < N; ++j) sum += (__int128)vapprox[j];

    if (sum == 0) {
        int idx = 0;
        for (int j = 1; j < N; ++j) if (v[j] > v[idx]) idx = j;
        for (int j = 0; j < N; ++j) out8[j] = 0;
        out8[idx] = 255;
    } else {
        for (int j = 0; j < N; ++j) {
            __int128 num = (__int128)vapprox[j] * 255 + (sum >> 1);
            int64_t p8 = (int64_t)(num / sum);
            if (p8 < 0) p8 = 0; if (p8 > 255) p8 = 255;
            out8[j] = (uint8_t)p8;
        }
    }

    if (mac_hash_out) *mac_hash_out = mac_hash;

    free(vstable);
    free(vapprox);
    return 0;
}

int main(int argc, char **argv)
{
    size_t N = (argc > 1) ? (size_t)strtoull(argv[1], 0, 10) : (1u << 20);  // 1,048,576
    int M = 8;
    double S = 0.02;

    int32_t *v   = (int32_t*)malloc(N * sizeof(int32_t));
    uint8_t *out = (uint8_t*)malloc(N * sizeof(uint8_t));
    if (!v || !out) { puts("alloc_fail"); return 1; }

    for (size_t i = 0; i < N; ++i) {
        uint32_t r = (uint32_t)(i * 1315423911u + 0x9E3779B9u);
        v[i] = (int32_t)((int32_t)(r % 101) - 50);
    }

    uint64_t mac_hash = 0;
    int rc = int_softmax_mac_ilp(v, (int)N, M, S, out, &mac_hash);
    if (rc != 0) { printf("error %d\n", rc); return 1; }

    uint64_t out_sum = 0;
    for (size_t i = 0; i < N; ++i) out_sum += out[i];

    printf("N=%zu LANES=%d QCOEF=%d RSHIFT=%d mac_hash=%llu out_sum=%llu\n",
           N, LANES, QCOEF, RSHIFT,
           (unsigned long long)mac_hash, (unsigned long long)out_sum);

    free(v); free(out);
    return 0;
}
