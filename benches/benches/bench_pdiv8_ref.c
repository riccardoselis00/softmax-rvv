#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bench.h"

#ifdef SOFT_USE_M5OPS
  #include <gem5/m5ops.h>
#else
  static inline void m5_reset_stats(uint64_t a, uint64_t b) { (void)a; (void)b; }
  static inline void m5_dump_stats(uint64_t a, uint64_t b)  { (void)a; (void)b; }
  static inline void m5_exit(uint64_t a)                    { (void)a; }
#endif

static inline int8_t sat_int8(int16_t x)
{
    if (x > 127)  return 127;
    if (x < -128) return -128;
    return (int8_t)x;
}

// Reference semantics: 8 lanes of int8 div with b==0 => 0, saturate to int8.
static inline uint64_t pdiv8_ref(uint64_t a, uint64_t b)
{
    uint64_t out = 0;

    for (int lane = 0; lane < 8; ++lane) {
        int shift = lane * 8;

        int8_t aa = (int8_t)((a >> shift) & 0xFF);
        int8_t bb = (int8_t)((b >> shift) & 0xFF);

        int16_t q;
        if (bb == 0) {
            q = 0;
        } else {
            // Signed division in wider type (forces real div instructions)
            q = (int16_t)aa / (int16_t)bb;
        }

        int8_t res = sat_int8(q);
        out |= ((uint64_t)(uint8_t)res) << shift;
    }

    return out;
}

static void usage(const char* prog)
{
    printf("Usage: %s [--iters=N] [--seed=S]\n", prog);
    printf("  --iters default 200000\n");
    printf("  --seed  default 12345\n");
}

static int parse_u64_arg(const char* s, const char* key, uint64_t* out)
{
    size_t n = strlen(key);
    if (strncmp(s, key, n) != 0) return 0;
    *out = (uint64_t)strtoull(s + n, NULL, 10);
    return 1;
}

int main(int argc, char** argv)
{
    uint64_t iters = 200000ULL;
    uint64_t seed  = 12345ULL;

    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--help")) { usage(argv[0]); return 0; }
        if (parse_u64_arg(argv[i], "--iters=", &iters)) continue;
        if (parse_u64_arg(argv[i], "--seed=",  &seed))  continue;

        printf("Unknown arg: %s\n", argv[i]);
        usage(argv[0]);
        return 1;
    }

    bench_rng_seed(seed);

    // Same input evolution as custom bench (fair comparison).
    // Note: we keep bb != 0 per lane by OR-ing 0x01 in each byte.
    uint64_t a = bench_rand_u64();
    uint64_t b = bench_rand_u64() | 0x0101010101010101ULL;

    const uint64_t da = 0x1D1D1D1D1D1D1D1DULL;
    const uint64_t db = 0x2B2B2B2B2B2B2B2BULL;

    printf("[REF] pdiv8 ROI: iters=%llu\n", (unsigned long long)iters);
    fflush(stdout);

    // Warm-up excluded from stats
    uint64_t warm = 0;
    for (int i = 0; i < 128; ++i) {
        a += da;
        b = (b + db) | 0x0101010101010101ULL;
        warm ^= pdiv8_ref(a, b);
    }
    bench_sink_u64(warm);

    // ---------------- ROI ----------------
    m5_reset_stats(0, 0);

    uint64_t checksum = 0;
    for (uint64_t i = 0; i < iters; ++i) {
        a += da;
        b = (b + db) | 0x0101010101010101ULL;

        uint64_t out = pdiv8_ref(a, b);
        checksum += (out & 0xFFu);
    }

    bench_sink_u64(checksum);

    m5_dump_stats(0, 0);
    m5_exit(0);
    return 0;
}
