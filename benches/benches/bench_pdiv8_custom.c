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

// -----------------------------------------------------------------------------
// Custom PDIV8 instruction (8x int8 lanes packed in 64-bit)
//
// IMPORTANT: This encoding assumes you decode pdiv8 on:
//   opcode = 0x5B, funct3 = 0x03, funct7 = 0x00
// If your gem5 decode differs, adjust funct3/funct7 accordingly.
// -----------------------------------------------------------------------------
static inline uint64_t pdiv8_hw(uint64_t a, uint64_t b)
{
    uint64_t rd;
    __asm__ volatile (".insn r 0x5B, 0x03, 0x00, %0, %1, %2"
                      : "=r"(rd) : "r"(a), "r"(b) : "memory");
    return rd;
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

#if !defined(SOFT_HAVE_CUSTOM_OPS) || (SOFT_HAVE_CUSTOM_OPS != 1)
    printf("This bench requires SOFT_HAVE_CUSTOM_OPS=1\n");
    return 1;
#endif

    bench_rng_seed(seed);

    // Inputs in registers; update with cheap ALU to keep them changing.
    // Force denominators non-zero per lane by OR-ing with 0x01 in each byte.
    uint64_t a = bench_rand_u64();
    uint64_t b = bench_rand_u64() | 0x0101010101010101ULL;

    const uint64_t da = 0x1D1D1D1D1D1D1D1DULL;  // arbitrary deltas
    const uint64_t db = 0x2B2B2B2B2B2B2B2BULL;

    printf("[CUSTOM] pdiv8 ROI: iters=%llu\n", (unsigned long long)iters);
    fflush(stdout);

    // Warm-up excluded from stats
    uint64_t warm = 0;
    for (int i = 0; i < 128; ++i) {
        a += da;
        b = (b + db) | 0x0101010101010101ULL;
        warm ^= pdiv8_hw(a, b);
    }
    bench_sink_u64(warm);

    // ---------------- ROI ----------------
    m5_reset_stats(0, 0);

    uint64_t checksum = 0;
    for (uint64_t i = 0; i < iters; ++i) {
        a += da;
        b = (b + db) | 0x0101010101010101ULL;

        uint64_t out = pdiv8_hw(a, b);
        checksum += (out & 0xFFu); // cheap reduction
    }

    bench_sink_u64(checksum);

    m5_dump_stats(0, 0);
    m5_exit(0);
    return 0;
}
