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

// ------------------------------------------------------------
// Helpers
// ------------------------------------------------------------

static inline __attribute__((always_inline))
int8_t clip_delta_i8(int d)
{
  if (d < -16) d = -16;
  if (d > 0)   d = 0;
  return (int8_t)d;
}

// Same K logic as custom bench: keeps operands in int8-ish range
static inline int choose_shift_K(int N)
{
  int k = 0;
  int t = (N <= 1) ? 1 : (N - 1);
  while (t > 0) { t >>= 1; k++; }  // ceil_log2(N)
  k = k + 1;
  if (k < 7)  k = 7;
  if (k > 15) k = 15;
  return k;
}

// Polynomial "exp-like": num = 127 + 7*z, z in [-16,0]
// Result in [15,127], always positive.
static inline __attribute__((always_inline))
uint8_t poly_num_u8_from_delta(int8_t z)
{
  // z <= 0 so 7*z is in [-112, 0]
  int v = 127 + 7 * (int)z;
  // should already be in [15,127], but keep clamp for safety
  if (v < 0)   v = 0;
  if (v > 127) v = 127;
  return (uint8_t)v;
}

// ------------------------------------------------------------
// Baseline softmax pipeline (NO custom)
// - same polynomial: num = 127 + 7*delta
// - same scaling scheme used in custom bench:
//     denom_u = ceil(sum / 2^K) clipped to [1..127]
//     ns = (num*127 + round) >> K  clipped to [0..127]
//     p  = ns / denom_u  clipped to [0..127]
// Output is uint8 in [0..127] (matches custom bench output range).
// ------------------------------------------------------------
static inline __attribute__((always_inline))
void softmax_int8_poly_ref(
    const int8_t* __restrict x,
    uint8_t* __restrict out,
    int N,
    int K)
{
  // 1) max
  int8_t max_x = x[0];
  for (int i = 1; i < N; ++i) {
    if (x[i] > max_x) max_x = x[i];
  }

  // 2) exp-like numerators + sum
  uint32_t sum = 0;
  for (int i = 0; i < N; ++i) {
    int d = (int)x[i] - (int)max_x;
    int8_t z = clip_delta_i8(d);
    uint8_t num = poly_num_u8_from_delta(z);
    out[i] = num;
    sum += (uint32_t)num;
  }

  // 3) normalize (scalar IntDiv baseline)
  uint32_t round = (K > 0) ? (1u << (K - 1)) : 0u;

  uint32_t denom_u = (sum + ((1u << K) - 1u)) >> K;  // ceil(sum / 2^K)
  if (denom_u == 0) denom_u = 1;
  if (denom_u > 127) denom_u = 127;

  for (int i = 0; i < N; ++i) {
    uint32_t n = (uint32_t)out[i];           // 0..127
    uint32_t ns = (n * 127u + round) >> K;   // 0..127-ish
    if (ns > 127u) ns = 127u;

    // scalar division (the expensive baseline)
    uint32_t p = ns / denom_u;               // 0..127
    if (p > 127u) p = 127u;
    out[i] = (uint8_t)p;
  }
}

// ------------------------------------------------------------
// CLI / benchmark harness (ROI)
// ------------------------------------------------------------

static void usage(const char* prog)
{
  printf("Usage: %s [--n=N] [--iters=R] [--seed=S]\n", prog);
  printf("  --n     default 256 (must be multiple of 8 for comparability)\n");
  printf("  --iters default 2000 (number of softmax calls in ROI)\n");
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

  if (N <= 0 || (N % 8) != 0) {
    printf("Error: --n must be a positive multiple of 8.\n");
    return 1;
  }

  const int K = choose_shift_K(N);

  int8_t*  x   = (int8_t*)malloc((size_t)N);
  uint8_t* out = (uint8_t*)malloc((size_t)N);
  if (!x || !out) {
    printf("malloc failed\n");
    return 1;
  }

  bench_rng_seed(seed);

  // logits in [-64,63]
  for (int i = 0; i < N; ++i) {
    uint8_t r = (uint8_t)bench_rand_u64();
    x[i] = (int8_t)((int)(r & 0x7F) - 64);
  }

  printf("[REF] softmax pipeline ROI\n");
  printf("  n=%d, iters=%llu, K=%d\n", N, (unsigned long long)iters, K);
  printf("  Expected per softmax call: scalar IntDiv ≈ N (normalization loop)\n");
  fflush(stdout);

  // Warmup excluded
  for (int w = 0; w < 8; ++w) {
    softmax_int8_poly_ref(x, out, N, K);
    bench_sink_u64(out[w & (N - 1)]);
  }

  // ---------------- ROI ----------------
  m5_reset_stats(0, 0);

  uint64_t checksum = 0;
  for (uint64_t r = 0; r < iters; ++r) {
    // perturb to avoid identical repetitions
    int idx = (int)(r & (uint64_t)(N - 1));
    x[idx] = (int8_t)(x[idx] + 1);

    softmax_int8_poly_ref(x, out, N, K);

    checksum += out[idx];
  }

  bench_sink_u64(checksum);

  m5_dump_stats(0, 0);
  m5_exit(0);

  free(x);
  free(out);
  return 0;
}
