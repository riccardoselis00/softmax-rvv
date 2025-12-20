// pdiv8_test.c
// Benchmark + correctness tester for custom 8-lane int8 packed divide instruction

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// -----------------------------------------------------------------------------
// Custom PDIV8 instruction: 8x int8 lanes, packed in 64-bit
// rd[i] := sat_int8( rs1[i] / rs2[i] ), with div-by-zero -> 0
// Update FUNCT3/encoding if your decoder differs.
// -----------------------------------------------------------------------------
#define PDIV8_R(dest, x, y) \
    __asm__ (".insn r 0x5B, 0x03, 0x00, %0, %1, %2" \
             : "=r"(dest) : "r"(x), "r"(y))

// -----------------------------------------------------------------------------
// Reference C implementation of 8×int8 division with saturation
// -----------------------------------------------------------------------------
static inline uint64_t pdiv8_ref(uint64_t a, uint64_t b)
{
    uint64_t out = 0;

    for (int lane = 0; lane < 8; ++lane) {
        int shift = lane * 8;

        int8_t a_lane = (int8_t)((a >> shift) & 0xFF);
        int8_t b_lane = (int8_t)((b >> shift) & 0xFF);

        int16_t q;

        if (b_lane == 0) {
            // Must match HW behavior: here we choose q=0 for div-by-zero
            q = 0;
        } else {
            q = (int16_t)a_lane / (int16_t)b_lane;
        }

        // Saturate to int8 range [-128, 127]
        if (q > 127)  q = 127;
        if (q < -128) q = -128;

        out |= ((uint64_t)((uint8_t)q) << shift);
    }

    return out;
}

// -----------------------------------------------------------------------------
// Hardware version calling PDIV8
// -----------------------------------------------------------------------------
static inline uint64_t pdiv8_hw(uint64_t a, uint64_t b)
{
    uint64_t q;
    PDIV8_R(q, a, b);
    return q;
}

static inline uint64_t rdcycle(void)
{
    uint64_t c;
    __asm__ volatile ("rdcycle %0" : "=r"(c));
    return c;
}

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
int main(void)
{
    printf("Testing custom PDIV8 instruction (8x int8 lanes)...\n");

    srand(12345);

    int errors = 0;
    const int NUM_TESTS = 10000;

    // ------------------------------
    // Correctness tests vs reference
    // ------------------------------
    for (int i = 0; i < NUM_TESTS; ++i) {
        // Random 64-bit packed values
        uint64_t a = ((uint64_t)rand() << 32) ^ (uint64_t)rand();
        uint64_t b = ((uint64_t)rand() << 32) ^ (uint64_t)rand();

        uint64_t ref = pdiv8_ref(a, b);
        uint64_t hw  = pdiv8_hw(a, b);

        if (ref != hw) {
            errors++;
            printf("Mismatch %d:\n", i);
            printf("  a   = 0x%016llx\n", (unsigned long long)a);
            printf("  b   = 0x%016llx\n", (unsigned long long)b);
            printf("  ref = 0x%016llx\n", (unsigned long long)ref);
            printf("  hw  = 0x%016llx\n", (unsigned long long)hw);
            if (errors > 10) {
                printf("Too many errors, stopping.\n");
                break;
            }
        }
    }

    if (errors == 0)
        printf("Correctness: ALL PDIV8 TESTS PASSED ✔️\n");
    else
        printf("Correctness: %d errors ❌\n", errors);

    // ------------------------------
    // Micro-benchmark
    // ------------------------------
    uint64_t a = 0x0807060504030201ULL;  // lanes: 1..8
    uint64_t b = 0x0101010101010101ULL;  // divide by 1 in all lanes

    uint64_t acc = 0;   // accumulate result so compiler can't kill loop
    const int N = 1000000;

    uint64_t start = rdcycle();
    for (int i = 0; i < N; ++i) {
        uint64_t q = pdiv8_hw(a, b);
        acc ^= q;       // cheap dependency to keep q "live"
        // optionally vary 'a' slightly to avoid being too trivial:
        a += 0x0101010101010101ULL;
    }
    uint64_t end = rdcycle();

    printf("Benchmark: %d PDIV8 ops → %llu cycles\n",
           N, (unsigned long long)(end - start));
    printf("Final acc = 0x%016llx\n", (unsigned long long)acc);

    return (errors == 0) ? 0 : 1;
}
