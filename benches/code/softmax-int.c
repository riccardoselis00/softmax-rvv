// softmax_int_mac_ilp_fixed4.c — Integer-only softmax with MAC ILP (LANES = 4, reordered loop)

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

#define LANES 4  // fixed: 4 lanes of ILP

// degree-2 poly coefs and ln2 (no libm needed)
static const double A_coef = 0.3585;
static const double B_coef = 1.353;
static const double C_coef = 0.344;
static const double LN2   = 0.6931471805599453094;

// Custom MAC: rd := rd + rs1*rs2 (low 64). Update fields if your decoder differs.
#define MACACC_R(dest, x, y) \
  __asm__ (".insn r 0x5B, 0x02, 0x00, %0, %1, %2" : "+&r"(dest) : "r"(x), "r"(y))

// Non-inlined kernel so its body (with .insn) always appears in the .s
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

    // 3) integer constants (Barrett + poly)
    int64_t vln2 = (int64_t)(LN2 / S);
    if (vln2 <= 0) vln2 = 1;

    __int128 base  = (__int128)1 << (2 * M);
    __int128 mu128 = base / (__int128)vln2;   // floor
    if (mu128 <= 0) mu128 = 1;
    int64_t mu = (int64_t)mu128;

    int64_t vb = (int64_t)(B_coef / S);
    double denom = A_coef * S * S; if (denom <= 0.0) denom = 1e-12;
    int64_t vc = (int64_t)(denom > 0 ? (C_coef / denom) : 0);

    // 4) output buffer (approx exp values before normalization)
    int64_t *vapprox = (int64_t*)malloc((size_t)N * sizeof(int64_t));
    if (!vapprox) { free(vstable); return -4; }

    uint64_t mac_hash = 0;  // keeps MAC results observable

    // 5) main loop (LANES = 4)
    int i = 0;
    int n_vec = (N / LANES) * LANES;

    for (; i < n_vec; i += LANES) {
        // per-lane inputs
        int32_t vs0 = vstable[i+0];
        int32_t vs1 = vstable[i+1];
        int32_t vs2 = vstable[i+2];
        int32_t vs3 = vstable[i+3];

        // q per lane (Barrett): q = floor(vs * mu / 2^(2M))
        __int128 p0 = (__int128)vs0 * (__int128)mu;
        __int128 p1 = (__int128)vs1 * (__int128)mu;
        __int128 p2 = (__int128)vs2 * (__int128)mu;
        __int128 p3 = (__int128)vs3 * (__int128)mu;

        int64_t q0 = (int64_t)(p0 >> (2 * M));
        int64_t q1 = (int64_t)(p1 >> (2 * M));
        int64_t q2 = (int64_t)(p2 >> (2 * M));
        int64_t q3 = (int64_t)(p3 >> (2 * M));

        // t = (vs - q*vln2) + vb
        int64_t t0 = (int64_t)vs0 - q0 * vln2;  t0 += vb;
        int64_t t1 = (int64_t)vs1 - q1 * vln2;  t1 += vb;
        int64_t t2 = (int64_t)vs2 - q2 * vln2;  t2 += vb;
        int64_t t3 = (int64_t)vs3 - q3 * vln2;  t3 += vb;

        // ---- PHASE 1: fire MACs back-to-back across lanes (no dependent reads) ----
        uint64_t a0 = 0, a1 = 0, a2 = 0, a3 = 0;   // accumulators (low 64 of t^2)
        MACACC_R(a0, t0, t0);
        MACACC_R(a1, t1, t1);
        MACACC_R(a2, t2, t2);
        MACACC_R(a3, t3, t3);

        // ---- PHASE 2: now consume results (hash + numer/shift/store) ----
        mac_hash ^= (a0 ^ a1 ^ a2 ^ a3);

        uint64_t numer0 = a0 + (uint64_t)vc;
        uint64_t numer1 = a1 + (uint64_t)vc;
        uint64_t numer2 = a2 + (uint64_t)vc;
        uint64_t numer3 = a3 + (uint64_t)vc;

        int64_t s0 = (q0 < 0) ? 0 : q0;
        int64_t s1 = (q1 < 0) ? 0 : q1;
        int64_t s2 = (q2 < 0) ? 0 : q2;
        int64_t s3 = (q3 < 0) ? 0 : q3;

        uint64_t appr0 = (s0 >= 63) ? 0ULL : (numer0 >> s0);
        uint64_t appr1 = (s1 >= 63) ? 0ULL : (numer1 >> s1);
        uint64_t appr2 = (s2 >= 63) ? 0ULL : (numer2 >> s2);
        uint64_t appr3 = (s3 >= 63) ? 0ULL : (numer3 >> s3);

        vapprox[i+0] = (int64_t)appr0;
        vapprox[i+1] = (int64_t)appr1;
        vapprox[i+2] = (int64_t)appr2;
        vapprox[i+3] = (int64_t)appr3;
    }

    // tail (scalar)
    for (; i < N; ++i) {
        int32_t vs = vstable[i];
        __int128 p = (__int128)vs * (__int128)mu;
        int64_t q = (int64_t)(p >> (2 * M));
        int64_t t = (int64_t)vs - q * vln2;  t += vb;
        uint64_t acc = 0;
        MACACC_R(acc, t, t);
        mac_hash ^= acc;
        uint64_t numer = acc + (uint64_t)vc;
        int64_t s = (q < 0) ? 0 : q;
        uint64_t appr = (s >= 63) ? 0ULL : (numer >> s);
        vapprox[i] = (int64_t)appr;
    }

    // 6) sum & normalize (exact 128-bit sum)
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
            int64_t p = (int64_t)(num / sum);
            if (p < 0) p = 0; if (p > 255) p = 255;
            out8[j] = (uint8_t)p;
        }
    }

    if (mac_hash_out) *mac_hash_out = mac_hash;

    free(vstable);
    free(vapprox);
    return 0;
}

int main(int argc, char **argv) {
    // >>> Make N large via argv (≥ 1e6) so you see steady-state and FU scaling
    size_t N = (argc > 1) ? (size_t)strtoull(argv[1], 0, 10) : (1u << 20);  // default: 1,048,576
    int M = 8;
    double S = 0.02;

    // allocate inputs/outputs
    int32_t *v = (int32_t*)malloc(N * sizeof(int32_t));
    uint8_t *out8 = (uint8_t*)malloc(N * sizeof(uint8_t));
    if (!v || !out8) { puts("alloc_fail"); return 1; }

    // fill v with a deterministic spread around 0
    for (size_t i = 0; i < N; ++i) {
        uint32_t r = (uint32_t)(i * 1315423911u + 0x9E3779B9u);
        v[i] = (int32_t)((int32_t)(r % 101) - 50);  // [-50, 50]
    }

    uint64_t mac_hash = 0;
    int rc = int_softmax_mac_ilp(v, (int)N, M, S, out8, &mac_hash);
    if (rc != 0) { printf("error %d\n", rc); return 1; }

    // observable checksum of output
    uint64_t out_sum = 0;
    for (size_t i = 0; i < N; ++i) out_sum += out8[i];

    printf("N=%zu LANES=%d mac_hash=%llu out_sum=%llu\n",
           N, LANES, (unsigned long long)mac_hash, (unsigned long long)out_sum);

    free(v); free(out8);
    return 0;
}

