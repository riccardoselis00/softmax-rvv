#define _POSIX_C_SOURCE 200809L
#include "bench.h"
#include <time.h>


static volatile uint64_t g_sink;
static uint64_t g_rng = 0x123456789abcdef0ULL;

void bench_sink_u64(uint64_t x) { g_sink ^= x; }

void bench_rng_seed(uint64_t seed) {
  g_rng = (seed ? seed : 0x123456789abcdef0ULL);
}

// xorshift64*
uint64_t bench_rand_u64(void) {
  uint64_t x = g_rng;
  x ^= x >> 12;
  x ^= x << 25;
  x ^= x >> 27;
  g_rng = x;
  return x * 2685821657736338717ULL;
}

uint64_t bench_rdcycle(void) {
#if defined(__riscv)
  uint64_t c;
  __asm__ volatile ("rdcycle %0" : "=r"(c));
  return c;
#else
  return 0;
#endif
}

double bench_seconds(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec + 1e-9 * (double)ts.tv_nsec;
}
