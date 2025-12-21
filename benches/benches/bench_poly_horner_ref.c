#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bench.h"
#include "ops.h"
#include "poly_horner.h"

#ifdef SOFT_USE_M5OPS
  #include <gem5/m5ops.h>
#else
  static inline void m5_reset_stats(uint64_t a, uint64_t b) { (void)a; (void)b; }
  static inline void m5_dump_stats(uint64_t a, uint64_t b)  { (void)a; (void)b; }
  static inline void m5_exit(uint64_t a)                    { (void)a; }
#endif

static void usage(const char* prog) {
  printf("Usage: %s [--iters=N] [--degree=D] [--range=low|high] [--seed=S]\n", prog);
}

static int parse_u64_arg(const char* s, const char* key, uint64_t* out) {
  size_t n = strlen(key);
  if (strncmp(s, key, n) != 0) return 0;
  *out = (uint64_t)strtoull(s + n, NULL, 10);
  return 1;
}

static int parse_i32_arg(const char* s, const char* key, int* out) {
  size_t n = strlen(key);
  if (strncmp(s, key, n) != 0) return 0;
  *out = (int)strtol(s + n, NULL, 10);
  return 1;
}

static uint64_t make_z0(int lo, int hi) {
  uint64_t x = bench_rand_u64();
  uint64_t z = 0;
  int span = hi - lo + 1;

  for (int lane = 0; lane < 8; ++lane) {
    uint8_t byte = (uint8_t)(x >> (8 * lane));
    int v = lo + (byte % span);
    int8_t vi = (int8_t)v;
    z |= ((uint64_t)(uint8_t)vi) << (8 * lane);
  }
  return z;
}

int main(int argc, char** argv)
{
  uint64_t iters = 200000ULL;
  int degree = 2;
  uint64_t seed = 12345ULL;
  int lo = -8, hi = 7;

  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "--help")) { usage(argv[0]); return 0; }
    if (!strcmp(argv[i], "--range=low"))  { lo = -8;  hi = 7;  continue; }
    if (!strcmp(argv[i], "--range=high")) { lo = -64; hi = 63; continue; }
    if (parse_u64_arg(argv[i], "--iters=", &iters)) continue;
    if (parse_i32_arg(argv[i], "--degree=", &degree)) continue;
    if (parse_u64_arg(argv[i], "--seed=", &seed)) continue;

    printf("Unknown arg: %s\n", argv[i]);
    usage(argv[0]);
    return 1;
  }

  if (degree < 0 || degree > 16) {
    printf("degree must be in [0,16]\n");
    return 1;
  }

  const ops_vtable_t* ops = ops_get(OPS_IMPL_REF);

  int8_t coeffs[17] = {0};
  if (degree == 2) {
    coeffs[0] = 0; coeffs[1] = 1; coeffs[2] = 1;
  } else {
    for (int k = 0; k <= degree; ++k) coeffs[k] = (int8_t)((k % 5) - 2);
  }

  bench_rng_seed(seed);
  uint64_t z = make_z0(lo, hi);
  const uint64_t delta = 0x0101010101010101ULL;

  printf("[REF] poly_horner ROI: iters=%llu degree=%d range=[%d,%d]\n",
         (unsigned long long)iters, degree, lo, hi);
  fflush(stdout);

  // Warm-up: excluded from stats
  uint64_t warm = 0;
  for (int i = 0; i < 64; ++i) {
    z += delta;
    warm ^= poly_horner_eval_i8x8(ops, z, coeffs, degree);
  }
  bench_sink_u64(warm);

  // ROI starts here
  m5_reset_stats(0, 0);

  uint64_t checksum = 0;
  for (uint64_t i = 0; i < iters; ++i) {
    z += delta;
    uint64_t out = poly_horner_eval_i8x8(ops, z, coeffs, degree);
    checksum += (out & 0xFFu);
  }

  bench_sink_u64(checksum);

  m5_dump_stats(0, 0);
  m5_exit(0);

  return 0;
}
