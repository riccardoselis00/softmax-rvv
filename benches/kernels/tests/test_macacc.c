#include <stdint.h>
#include <stdio.h>

#include "ops.h"
#include "bench.h"

static uint64_t pack_i8(const int8_t v[8]) {
  uint64_t out = 0;
  for (int i = 0; i < 8; ++i) out |= ((uint64_t)(uint8_t)v[i]) << (8 * i);
  return out;
}

static int run_one(const ops_vtable_t* ref, const ops_vtable_t* hw,
                   uint64_t acc, uint64_t a, uint64_t b, int verbose)
{
  uint64_t r = ref->macacc8(acc, a, b);
  uint64_t h = hw->macacc8(acc, a, b);
  if (r != h) {
    if (verbose) {
      printf("MACACC mismatch!\n");
      printf("  acc = 0x%016llx\n", (unsigned long long)acc);
      printf("  a   = 0x%016llx\n", (unsigned long long)a);
      printf("  b   = 0x%016llx\n", (unsigned long long)b);
      printf("  ref = 0x%016llx\n", (unsigned long long)r);
      printf("  hw  = 0x%016llx\n", (unsigned long long)h);
    }
    return 1;
  }
  return 0;
}

int main(void)
{
  const ops_vtable_t* ref = ops_get(OPS_IMPL_REF);

#if !defined(SOFT_HAVE_CUSTOM_OPS) || (SOFT_HAVE_CUSTOM_OPS != 1)
  printf("SKIP: built without custom ops backend (SOFT_HAVE_CUSTOM_OPS=0).\n");
  return 0;
#else
  const ops_vtable_t* hw = ops_get(OPS_IMPL_CUSTOM);
  if (hw == ref) {
    printf("SKIP: custom backend not available (ops_get(custom)==ref).\n");
    return 0;
  }
#endif

  int errors = 0;

  { // saturation top
    int8_t accv[8] = {127,127,127,127,127,127,127,127};
    int8_t av[8]   = {1,1,1,1,1,1,1,1};
    int8_t bv[8]   = {1,1,1,1,1,1,1,1};
    errors += run_one(ref, hw, pack_i8(accv), pack_i8(av), pack_i8(bv), 1);
  }

  bench_rng_seed(12345);
  const int N = 10000;
  for (int i = 0; i < N; ++i) {
    uint64_t acc = bench_rand_u64();
    uint64_t a   = bench_rand_u64();
    uint64_t b   = bench_rand_u64();
    errors += run_one(ref, hw, acc, a, b, 0);
    if (errors > 50) break;
  }

  if (errors == 0) {
    printf("Correctness: MACACC PASSED (%d random tests)\n", N);
    return 0;
  }
  printf("Correctness: MACACC FAILED (%d errors)\n", errors);
  return 1;
}
