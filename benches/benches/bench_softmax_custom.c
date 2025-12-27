#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bench.h"

#if defined(SOFT_USE_M5) && (SOFT_USE_M5 == 1)
  #include <gem5/m5ops.h>
#else
  static inline void m5_reset_stats(uint64_t a, uint64_t b) { (void)a; (void)b; }
  static inline void m5_dump_stats(uint64_t a, uint64_t b)  { (void)a; (void)b; }
  static inline void m5_exit(uint64_t a)                    { (void)a; }
#endif

// ----------------------------------------------------------------------------
// Custom instructions
// ----------------------------------------------------------------------------

// MACACC: rd[i] := sat_int8( rd[i] + rs1[i] * rs2[i] )  (8 lanes packed in 64-bit)
static inline __attribute__((always_inline))
uint64_t macacc8(uint64_t acc, uint64_t a, uint64_t b)
{
    __asm__ volatile (".insn r 0x5B, 0x02, 0x00, %0, %1, %2"
                      : "+&r"(acc) : "r"(a), "r"(b) : "memory");
    return acc;
}

// PDIV8: rd[i] := sat_int8( rs1[i] / rs2[i] ) (8 lanes packed), b==0 -> 0
// NOTE: encoding must match your gem5 decode for pdiv8 (funct3=0x03 here).
static inline __attribute__((always_inline))
uint64_t pdiv8(uint64_t a, uint64_t b)
{
    uint64_t rd;
    __asm__ volatile (".insn r 0x5B, 0x03, 0x00, %0, %1, %2"
                      : "=r"(rd) : "r"(a), "r"(b) : "memory");
    return rd;
}

// ----------------------------------------------------------------------------
// Small helpers
// ----------------------------------------------------------------------------

static inline __attribute__((always_inline))
uint64_t splat_u8(uint8_t v)
{
    uint64_t b = (uint64_t)v;
    uint64_t r = b;
    r |= (r << 8);
    r |= (r << 16);
    r |= (r << 32);
    return r;
}

// Sum 8 unsigned bytes in a 64-bit word (no cross-lane carry issues)
static inline __attribute__((always_inline))
uint32_t sum_u8_lanes(uint64_t x)
{
    // Pairwise sum into 16-bit lanes
    uint64_t lo = x & 0x00FF00FF00FF00FFULL;
    uint64_t hi = (x >> 8) & 0x00FF00FF00FF00FFULL;
    uint64_t s16 = lo + hi; // each 16-bit <= 510

    // Sum 16-bit lanes -> 32-bit lanes
    uint64_t lo16 = s16 & 0x0000FFFF0000FFFFULL;
    uint64_t hi16 = (s16 >> 16) & 0x0000FFFF0000FFFFULL;
    uint64_t s32 = lo16 + hi16; // each 32-bit <= 2040

    // Sum 32-bit lanes -> 64-bit
    uint64_t s64 = (s32 & 0x00000000FFFFFFFFULL) + (s32 >> 32); // <= 4080
    return (uint32_t)s64;
}

// Clip delta into [-16, 0]
static inline __attribute__((always_inline))
int8_t clip_delta_i8(int d)
{
    if (d < -16) d = -16;
    if (d > 0)   d = 0;
    return (int8_t)d;
}

// Pack 8 deltas (already int8) into 64-bit lanes
static inline __attribute__((always_inline))
uint64_t pack8_i8_from_x(const int8_t* x, int base, int8_t max_x)
{
    uint64_t z = 0;
    // unrolled 8 lanes
    int d0 = (int)x[base+0] - (int)max_x; z |= (uint64_t)(uint8_t)clip_delta_i8(d0) << 0;
    int d1 = (int)x[base+1] - (int)max_x; z |= (uint64_t)(uint8_t)clip_delta_i8(d1) << 8;
    int d2 = (int)x[base+2] - (int)max_x; z |= (uint64_t)(uint8_t)clip_delta_i8(d2) << 16;
    int d3 = (int)x[base+3] - (int)max_x; z |= (uint64_t)(uint8_t)clip_delta_i8(d3) << 24;
    int d4 = (int)x[base+4] - (int)max_x; z |= (uint64_t)(uint8_t)clip_delta_i8(d4) << 32;
    int d5 = (int)x[base+5] - (int)max_x; z |= (uint64_t)(uint8_t)clip_delta_i8(d5) << 40;
    int d6 = (int)x[base+6] - (int)max_x; z |= (uint64_t)(uint8_t)clip_delta_i8(d6) << 48;
    int d7 = (int)x[base+7] - (int)max_x; z |= (uint64_t)(uint8_t)clip_delta_i8(d7) << 56;
    return z;
}

// Compute K such that:
// - K >= 7 so (num*127 >> K) fits in int8 (<=127)
// - K large enough so denom = (sum >> K) fits in int8 reasonably
// We tie K to N to avoid dynamic shifts in the hot loops.
static inline int choose_shift_K(int N)
{
    int k = 0;
    int t = (N <= 1) ? 1 : (N - 1);
    while (t > 0) { t >>= 1; k++; }   // k = ceil_log2(N)
    k = k + 1;                        // safety for denom (keeps denom smaller)
    if (k < 7) k = 7;                 // safety for numerator scaling into int8
    if (k > 15) k = 15;               // keep shifts sane
    return k;
}

// ----------------------------------------------------------------------------
// Core: softmax-like pipeline (CUSTOM ONLY)
// - exp approx: num = 127 + 7*z, z in [-16,0]
// - store numerators (uint8) to out[]
// - sum numerators (uint32)
// - normalize with pdiv8 on scaled numerators, outputs in [0..127]
// ----------------------------------------------------------------------------
static inline __attribute__((always_inline))
void softmax_int8_poly_macacc_pdiv8(
    const int8_t* __restrict x,
    uint8_t* __restrict out,
    int N,
    int K
){
    // Constants for poly: num = 7*z + 127  (lane-wise sat int8)
    const uint64_t ONE = 0x0101010101010101ULL;
    const uint64_t C0  = splat_u8(127);
    const uint64_t C1  = splat_u8(7);

    // 1) max
    int8_t max_x = x[0];
    for (int i = 1; i < N; ++i) {
        if (x[i] > max_x) max_x = x[i];
    }

    // 2) exp-like + sum
    uint32_t sum = 0;

    for (int i = 0; i < N; i += 8) {
        uint64_t z = pack8_i8_from_x(x, i, max_x);

        // poly optimized: acc=0; acc += 7*z; acc += 127
        uint64_t acc = 0;
        acc = macacc8(acc, C1, z);    // acc = 7*z
        acc = macacc8(acc, C0, ONE);  // acc = 7*z + 127  (15..127)

        // store numerators (raw)
        __builtin_memcpy(out + i, &acc, 8);

        // accumulate sum of bytes (treat as uint8)
        sum += sum_u8_lanes(acc);
    }

    // 3) normalize: p ~ (num * 127) / sum, outputs in [0..127]
    // We do fixed-point scaling using the same K for numerator and denom:
    // denom8 = ceil(sum / 2^K), numS = round(num*127 / 2^K), p = numS / denom8
    // This keeps both operands in int8 range and makes pdiv8 usable.
    uint32_t round = (K > 0) ? (1u << (K - 1)) : 0u;

    uint32_t denom_u = (sum + ((1u << K) - 1u)) >> K;  // ceil(sum / 2^K)
    if (denom_u == 0) denom_u = 1;
    if (denom_u > 127) denom_u = 127;

    uint64_t denom_vec = splat_u8((uint8_t)denom_u);

    for (int i = 0; i < N; i += 8) {
        uint64_t num_raw;
        __builtin_memcpy(&num_raw, out + i, 8);

        // Build packed scaled numerators (8 lanes)
        // numS_lane = (num_lane * 127 + round) >> K  in [0..127]
        uint64_t ns = 0;

        // unrolled 8 lanes
        {
            uint32_t n0 = (uint8_t)(num_raw >> 0);
            uint32_t v0 = (n0 * 127u + round) >> K;
            ns |= (uint64_t)(uint8_t)v0 << 0;

            uint32_t n1 = (uint8_t)(num_raw >> 8);
            uint32_t v1 = (n1 * 127u + round) >> K;
            ns |= (uint64_t)(uint8_t)v1 << 8;

            uint32_t n2 = (uint8_t)(num_raw >> 16);
            uint32_t v2 = (n2 * 127u + round) >> K;
            ns |= (uint64_t)(uint8_t)v2 << 16;

            uint32_t n3 = (uint8_t)(num_raw >> 24);
            uint32_t v3 = (n3 * 127u + round) >> K;
            ns |= (uint64_t)(uint8_t)v3 << 24;

            uint32_t n4 = (uint8_t)(num_raw >> 32);
            uint32_t v4 = (n4 * 127u + round) >> K;
            ns |= (uint64_t)(uint8_t)v4 << 32;

            uint32_t n5 = (uint8_t)(num_raw >> 40);
            uint32_t v5 = (n5 * 127u + round) >> K;
            ns |= (uint64_t)(uint8_t)v5 << 40;

            uint32_t n6 = (uint8_t)(num_raw >> 48);
            uint32_t v6 = (n6 * 127u + round) >> K;
            ns |= (uint64_t)(uint8_t)v6 << 48;

            uint32_t n7 = (uint8_t)(num_raw >> 56);
            uint32_t v7 = (n7 * 127u + round) >> K;
            ns |= (uint64_t)(uint8_t)v7 << 56;
        }

        // Packed division (CUSTOM)
        uint64_t p = pdiv8(ns, denom_vec);

        // store probabilities (0..127)
        __builtin_memcpy(out + i, &p, 8);
    }
}

// ----------------------------------------------------------------------------
// CLI / benchmark harness (ROI)
// ----------------------------------------------------------------------------

static void usage(const char* prog)
{
    printf("Usage: %s [--n=N] [--iters=R] [--seed=S]\n", prog);
    printf("  --n     default 256 (must be multiple of 8)\n");
    printf("  --iters default 2000  (number of softmax calls in ROI)\n");
    printf("  --seed  default 12345\n");
}

static int parse_u64_arg(const char* s, const char* key, uint64_t* out)
{
    size_t n = strlen(key);
    if (strncmp(s, key, n) != 0) return 0;
    *out = (uint64_t)strtoull(s + n, NULL, 10);
    return 1;
}

static int parse_i32_arg(const char* s, const char* key, int* out)
{
    size_t n = strlen(key);
    if (strncmp(s, key, n) != 0) return 0;
    *out = (int)strtol(s + n, NULL, 10);
    return 1;
}

int main(int argc, char** argv)
{
    int N = 256;
    uint64_t iters = 2000;
    uint64_t seed  = 12345;

    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--help")) { usage(argv[0]); return 0; }
        if (parse_i32_arg(argv[i], "--n=", &N)) continue;
        if (parse_u64_arg(argv[i], "--iters=", &iters)) continue;
        if (parse_u64_arg(argv[i], "--seed=", &seed)) continue;

        printf("Unknown arg: %s\n", argv[i]);
        usage(argv[0]);
        return 1;
    }

#if !defined(SOFT_HAVE_CUSTOM_OPS) || (SOFT_HAVE_CUSTOM_OPS != 1)
    printf("This bench requires SOFT_HAVE_CUSTOM_OPS=1\n");
    return 1;
#endif

    if (N <= 0 || (N % 8) != 0) {
        printf("Error: --n must be a positive multiple of 8.\n");
        return 1;
    }

    const int K = choose_shift_K(N);

    // Allocate small arrays (stay cache-resident => compute-dominated)
    int8_t*  x   = (int8_t*)malloc((size_t)N);
    uint8_t* out = (uint8_t*)malloc((size_t)N);
    if (!x || !out) {
        printf("malloc failed\n");
        return 1;
    }

    bench_rng_seed(seed);

    // Init logits in a typical “softmax-ish” range
    for (int i = 0; i < N; ++i) {
        // [-64, 63]
        uint8_t r = (uint8_t)bench_rand_u64();
        x[i] = (int8_t)((int)(r & 0x7F) - 64);
    }

    printf("[CUSTOM] softmax pipeline ROI\n");
    printf("  n=%d, iters=%llu, K=%d\n", N, (unsigned long long)iters, K);
    printf("  Expected per softmax call: MACACC ≈ 2*(N/8), PDIV8 ≈ (N/8)\n");
    fflush(stdout);

    // Warmup excluded from stats
    for (int w = 0; w < 8; ++w) {
        softmax_int8_poly_macacc_pdiv8(x, out, N, K);
        bench_sink_u64(out[w & (N-1)]);
    }

    // ---------------- ROI ----------------
    m5_reset_stats(0, 0);

    uint64_t checksum = 0;
    for (uint64_t r = 0; r < iters; ++r) {
        // Perturb one element to prevent trivial repetition
        x[(int)(r & (uint64_t)(N - 1))] = (int8_t)(x[(int)(r & (uint64_t)(N - 1))] + 1);

        softmax_int8_poly_macacc_pdiv8(x, out, N, K);

        // cheap checksum
        checksum += out[(int)(r & (uint64_t)(N - 1))];
    }

    bench_sink_u64(checksum);

    m5_dump_stats(0, 0);
    m5_exit(0);

    // not reached in gem5 when m5_exit triggers
    free(x);
    free(out);
    return 0;
}