// macacc_test.c
// Benchmark + correctness tester for custom 8-lane int8 MACACC instruction

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MACACC_R(dest, x, y) \
    __asm__ (".insn r 0x5B, 0x02, 0x00, %0, %1, %2" \
             : "+&r"(dest) : "r"(x), "r"(y))

// -----------------------------------------------------------------------------
// Reference C implementation of 8×int8 MAC with saturation
// -----------------------------------------------------------------------------
static inline uint64_t macacc_ref(uint64_t acc, uint64_t a, uint64_t b)
{
    uint64_t out = 0;

    for (int lane = 0; lane < 8; ++lane) {
        int shift = lane * 8;

        int8_t acc_lane = (int8_t)((acc >> shift) & 0xFF);
        int8_t a_lane   = (int8_t)((a   >> shift) & 0xFF);
        int8_t b_lane   = (int8_t)((b   >> shift) & 0xFF);

        int16_t sum = acc_lane + a_lane * b_lane;

        if (sum > 127) sum = 127;
        if (sum < -128) sum = -128;

        out |= ((uint64_t)((uint8_t)sum) << shift);
    }
    return out;
}

// -----------------------------------------------------------------------------
// Hardware version calling MACACC
// -----------------------------------------------------------------------------
static inline uint64_t macacc_hw(uint64_t acc, uint64_t a, uint64_t b)
{
    MACACC_R(acc, a, b);
    return acc;
}

static inline uint64_t rdcycle()
{
    uint64_t c;
    __asm__ volatile ("rdcycle %0" : "=r"(c));
    return c;
}

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
int main()
{
    printf("Testing custom MACACC instruction...\n");

    int errors = 0;
    const int NUM_TESTS = 10000;

    for (int i = 0; i < NUM_TESTS; i++) {
        uint64_t acc = ((uint64_t)rand() << 32) ^ rand();
        uint64_t a   = ((uint64_t)rand() << 32) ^ rand();
        uint64_t b   = ((uint64_t)rand() << 32) ^ rand();

        uint64_t ref = macacc_ref(acc, a, b);
        uint64_t hw  = macacc_hw(acc, a, b);

        if (ref != hw) {
            errors++;
            printf("Mismatch %d:\n", i);
            printf("  acc = 0x%016llx\n", (unsigned long long)acc);
            printf("  a   = 0x%016llx\n", (unsigned long long)a);
            printf("  b   = 0x%016llx\n", (unsigned long long)b);
            printf("  ref = 0x%016llx\n", (unsigned long long)ref);
            printf("  hw  = 0x%016llx\n", (unsigned long long)hw);
        }
    }

    if (errors == 0)
        printf("Correctness: ALL TESTS PASSED ✔️\n");
    else
        printf("Correctness: %d errors ❌\n", errors);

    // Small benchmark
    uint64_t acc = 0;
    uint64_t a   = 0x0102030405060708ULL;
    uint64_t b   = 0x0807060504030201ULL;

    const int N = 1000000;

    uint64_t start = rdcycle();
    for (int i = 0; i < N; ++i)
        acc = macacc_hw(acc, a, b);
    uint64_t end = rdcycle();

    printf("Benchmark: %d MACACC ops → %llu cycles\n", N, (unsigned long long)(end - start));
    printf("Final acc = 0x%016llx\n", (unsigned long long)acc);

    return 0;
}
