#include <stdint.h>
#include <stdio.h>

#include "bench.h"
#include "ops.h"
#include "poly_horner.h"

// Random packed int8 lanes in [lo, hi]
static uint64_t random_packed_i8_range(int lo, int hi)
{
    uint64_t v = 0;
    for (int lane = 0; lane < 8; ++lane) {
        // deterministic RNG from bench.c
        uint64_t r = bench_rand_u64();
        int span = (hi - lo + 1);
        int x = lo + (int)(r % (uint64_t)span);
        int8_t xi = (int8_t)x;
        v |= ((uint64_t)(uint8_t)xi) << (8 * lane);
    }
    return v;
}

static void dump_u64(const char* name, uint64_t x)
{
    printf("  %s = 0x%016llx\n", name, (unsigned long long)x);
}

int main(void)
{
    const ops_vtable_t* ref = ops_get(OPS_IMPL_REF);

#if defined(SOFT_HAVE_CUSTOM_OPS) && (SOFT_HAVE_CUSTOM_OPS == 1)
    const ops_vtable_t* hw  = ops_get(OPS_IMPL_CUSTOM);
    if (hw == ref) {
        printf("SKIP: custom ops backend not available (ops_get(custom)==ref)\n");
        return 0;
    }
#else
    printf("SKIP: built without custom ops backend (SOFT_HAVE_CUSTOM_OPS=0)\n");
    return 0;
#endif

    printf("Testing poly_horner (REF vs CUSTOM) ...\n");

    // Seed deterministic RNG
    bench_rng_seed(12345);

    // Example polynomials:
    // 1) P(z)=z^2+z
    const int8_t p1[3] = {0, 1, 1};
    const int degree1 = 2;

    // 2) A slightly larger one (still int8):
    // P(z)= -3 + 5z -2z^2 + 1z^3
    const int8_t p2[4] = {-3, 5, -2, 1};
    const int degree2 = 3;

    int errors = 0;
    const int NUM_TESTS = 10000;

    for (int i = 0; i < NUM_TESTS; ++i) {
        // Keep z in a moderate range to avoid constant saturation,
        // but still allow some saturation to happen.
        uint64_t z = random_packed_i8_range(-8, 7);

        uint64_t r1 = poly_horner_eval_i8x8(ref, z, p1, degree1);
        uint64_t h1 = poly_horner_eval_i8x8(hw,  z, p1, degree1);

        uint64_t r2 = poly_horner_eval_i8x8(ref, z, p2, degree2);
        uint64_t h2 = poly_horner_eval_i8x8(hw,  z, p2, degree2);

        if (r1 != h1 || r2 != h2) {
            errors++;
            printf("Mismatch at test %d\n", i);
            dump_u64("z", z);
            if (r1 != h1) {
                printf("Poly1 (z^2+z):\n");
                dump_u64("ref", r1);
                dump_u64("hw ", h1);
            }
            if (r2 != h2) {
                printf("Poly2 (-3 + 5z -2z^2 + z^3):\n");
                dump_u64("ref", r2);
                dump_u64("hw ", h2);
            }
            if (errors > 10) {
                printf("Too many errors, stopping.\n");
                break;
            }
        }
    }

    if (errors == 0) {
        printf("Correctness: POLY_HORNER PASSED (%d tests)\n", NUM_TESTS);
        return 0;
    } else {
        printf("Correctness: POLY_HORNER FAILED (%d errors)\n", errors);
        return 1;
    }
}
