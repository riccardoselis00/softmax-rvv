// softmax_int_mac_ilp_blocked4.c — Integer-only softmax with MAC ILP
// Blocked (4 elems) + round-robin MAC bursts. Multiple MACs per element.
// Keep normalization as-is for now; we’ll kill per-element division later.

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>



#include <gem5/asm/generic/m5ops.h>
// #define m5_reset_stats(x,y) M5_RESET_STATS((x),(y))
// #define m5_dump_stats(x,y)  M5_DUMP_STATS((x),(y))

#define LANES 4              // fixed 4-lane ILP
#define KMAC  4              // MACs per element (raise to increase MAC intensity)

// degree-2 poly scaffold params (unchanged semantics)
static const double A_coef = 0.3585;
static const double B_coef = 1.353;
static const double C_coef = 0.344;
static const double LN2   = 0.6931471805599453094;

// Custom MAC: rd := rd + rs1*rs2 (low 64 bits)
#define MACACC_R(dest, x, y) \
  __asm__ (".insn r 0x5B, 0x02, 0x00, %0, %1, %2" : "+&r"(dest) : "r"(x), "r"(y))

// Small per-round tweak to decorrelate successive MAC operands without
// inflating magnitude (keeps values “different” but bounded).
static inline int64_t tweak(int64_t t, int r) {
    // XOR with small round-specific constants (no growth)
    static const uint64_t C[8] = {
        0x9E37U, 0xD2B7U, 0x7F4AU, 0x1CE6U, 0x3C6EU, 0x5BF1U, 0x27D4U, 0xA9B9U
    };
    return (int64_t)( ((uint64_t)t) ^ C[r & 7] );
}

// Non-inlined kernel so its body (with .insn) always appears
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

    // 4) output buffer (approx “exp” values before normalization)
    int64_t *vapprox = (int64_t*)malloc((size_t)N * sizeof(int64_t));
    if (!vapprox) { free(vstable); return -4; }

    uint64_t mac_hash = 0;  // keeps MAC results observable

    // ---- main loop, blocked at 4 elements (LANES) ----
    int i = 0;
    int n_vec = (N / LANES) * LANES;

    for (; i < n_vec; i += LANES) {
        // Gather lanes
        int32_t vs0 = vstable[i+0];
        int32_t vs1 = vstable[i+1];
        int32_t vs2 = vstable[i+2];
        int32_t vs3 = vstable[i+3];

        // Barrett q = floor(vs * mu / 2^(2M))
        __int128 p0 = (__int128)vs0 * (__int128)mu;
        __int128 p1 = (__int128)vs1 * (__int128)mu;
        __int128 p2 = (__int128)vs2 * (__int128)mu;
        __int128 p3 = (__int128)vs3 * (__int128)mu;

        int64_t q0 = (int64_t)(p0 >> (2 * M));
        int64_t q1 = (int64_t)(p1 >> (2 * M));
        int64_t q2 = (int64_t)(p2 >> (2 * M));
        int64_t q3 = (int64_t)(p3 >> (2 * M));

        // t = (vs - q*vln2) + vb  (kept as 64-bit)
        int64_t t0 = (int64_t)vs0 - q0 * vln2;  t0 += vb;
        int64_t t1 = (int64_t)vs1 - q1 * vln2;  t1 += vb;
        int64_t t2 = (int64_t)vs2 - q2 * vln2;  t2 += vb;
        int64_t t3 = (int64_t)vs3 - q3 * vln2;  t3 += vb;

        // ---- PHASE 1: round-robin MAC burst (multiple MACs per element) ----
        uint64_t a0 = 0, a1 = 0, a2 = 0, a3 = 0;

        // We issue KMAC passes; in each pass we MAC all 4 lanes once, with a tiny
        // per-pass tweak to operands to avoid identical repeats.
        for (int r = 0; r < KMAC; ++r) {
            int64_t u0 = tweak(t0, r);
            int64_t u1 = tweak(t1, r);
            int64_t u2 = tweak(t2, r);
            int64_t u3 = tweak(t3, r);

            // Fire across lanes back-to-back (keeps multiple MAC FUs busy)
            MACACC_R(a0, u0, u0);
            MACACC_R(a1, u1, u1);
            MACACC_R(a2, u2, u2);
            MACACC_R(a3, u3, u3);
        }

        // ---- PHASE 2: consume (hash + numer/shift/store) ----
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
        for (int r = 0; r < KMAC; ++r) {
            int64_t u = tweak(t, r);
            MACACC_R(acc, u, u);
        }
        mac_hash ^= acc;

        uint64_t numer = acc + (uint64_t)vc;
        int64_t s = (q < 0) ? 0 : q;
        uint64_t appr = (s >= 63) ? 0ULL : (numer >> s);
        vapprox[i] = (int64_t)appr;
    }

    // 6) sum & normalize (keep division for now; we’ll optimize later)
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

int main(int argc, char **argv) {
    // Make N large so you see steady-state and FU scaling
    size_t N = (argc > 1) ? (size_t)strtoull(argv[1], 0, 10) : (1u << 20);  // 1,048,576
    int M = 8;
    double S = 0.02;

    int32_t *v   = (int32_t*)malloc(N * sizeof(int32_t));
    uint8_t *out = (uint8_t*)malloc(N * sizeof(uint8_t));
    if (!v || !out) { puts("alloc_fail"); return 1; }

    // deterministic spread around 0
    for (size_t i = 0; i < N; ++i) {
        uint32_t r = (uint32_t)(i * 1315423911u + 0x9E3779B9u);
        v[i] = (int32_t)((int32_t)(r % 101) - 50);
    }

    uint64_t mac_hash = 0;

    //start analysis
    m5_reset_stats(0, 0);
    int rc = int_softmax_mac_ilp(v, (int)N, M, S, out, &mac_hash);
    m5_dump_stats(0, 0);
    //end analysis
    if (rc != 0) { printf("error %d\n", rc); return 1; }

    uint64_t out_sum = 0;
    for (size_t i = 0; i < N; ++i) out_sum += out[i];

    printf("N=%zu LANES=%d KMAC=%d mac_hash=%llu out_sum=%llu\n",
           N, LANES, KMAC, (unsigned long long)mac_hash, (unsigned long long)out_sum);

    free(v); free(out);
    return 0;
}
