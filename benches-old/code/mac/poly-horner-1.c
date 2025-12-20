#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// -----------------------------------------------------------------------------
// Custom MACACC instruction: 8x int8 lanes, packed in 64-bit
// rd[i] := sat_int8( rd[i] + rs1[i] * rs2[i] )
// -----------------------------------------------------------------------------
#define MACACC_R(dest, x, y) \
  __asm__ (".insn r 0x5B, 0x02, 0x00, %0, %1, %2" \
           : "+&r"(dest) : "r"(x), "r"(y))

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------

// Broadcast a signed int8 value to all 8 bytes of a 64-bit word
static inline uint64_t splat_i8(int8_t v)
{
    uint64_t b = (uint8_t)v;  // low byte
    uint64_t r = b;
    r |= (r << 8);
    r |= (r << 16);
    r |= (r << 32);
    return r;
}

// Saturate 16-bit to int8
static inline int8_t sat_int8(int16_t x)
{
    if (x > 127)  return 127;
    if (x < -128) return -128;
    return (int8_t)x;
}

// Reference lane-wise MAC: acc := sat(acc + a*b)
static inline int8_t mac_lane_ref(int8_t acc, int8_t a, int8_t b)
{
    int16_t prod = (int16_t)a * (int16_t)b;
    int16_t sum  = (int16_t)acc + prod;
    return sat_int8(sum);
}

// Packed reference MAC over 8 lanes
static uint64_t macacc_ref(uint64_t acc, uint64_t a, uint64_t b)
{
    uint64_t out = 0;

    for (int lane = 0; lane < 8; ++lane) {
        int shift = lane * 8;

        int8_t acc_lane = (int8_t)((acc >> shift) & 0xFF);
        int8_t a_lane   = (int8_t)((a   >> shift) & 0xFF);
        int8_t b_lane   = (int8_t)((b   >> shift) & 0xFF);

        int8_t res_lane = mac_lane_ref(acc_lane, a_lane, b_lane);

        out |= ((uint64_t)(uint8_t)res_lane << shift);
    }

    return out;
}

// Use MACACC to compute a*b (lane-wise) by starting from acc = 0
static inline uint64_t mul_packed_mac_hw(uint64_t a, uint64_t b)
{
    uint64_t acc = 0;
    MACACC_R(acc, a, b);  // acc := 0 + a*b
    return acc;
}

static inline uint64_t mul_packed_mac_ref(uint64_t a, uint64_t b)
{
    uint64_t acc = 0;
    return macacc_ref(acc, a, b);
}

// Read cycle counter (RV64)
static inline uint64_t rdcycle(void)
{
    uint64_t c;
    __asm__ volatile ("rdcycle %0" : "=r"(c));
    return c;
}

// -----------------------------------------------------------------------------
// Polynomial: P(z) = z^2 + z  (per lane, int8 arithmetic with saturation)
//
// Horner form: P(z) = (c2*z + c1)*z + c0
//   with c2 = 1, c1 = 1, c0 = 0
// -----------------------------------------------------------------------------


// Hardware version: uses MACACC (packed, 8 lanes)
static uint64_t poly_hw(uint64_t z_vec)
{
    const int8_t c0 = 0;
    const int8_t c1 = 1;
    const int8_t c2 = 1;

    uint64_t Z   = z_vec;
    uint64_t ONE = splat_i8(1);

    uint64_t C0 = splat_i8(c0);
    uint64_t C1 = splat_i8(c1);
    uint64_t C2 = splat_i8(c2);

    // Horner:
    // acc = c2
    uint64_t acc = C2;

    // acc = acc * z
    acc = mul_packed_mac_hw(acc, Z);

    // acc = acc + c1
    MACACC_R(acc, C1, ONE);

    // acc = acc * z
    acc = mul_packed_mac_hw(acc, Z);

    // acc = acc + c0  (c0=0, but this still uses MACACC)
    MACACC_R(acc, C0, ONE);

    return acc;
}

// Reference version: same Horner scheme, but in pure C using macacc_ref
static uint64_t poly_ref(uint64_t z_vec)
{
    const int8_t c0 = 0;
    const int8_t c1 = 1;
    const int8_t c2 = 1;

    uint64_t Z   = z_vec;
    uint64_t ONE = splat_i8(1);

    uint64_t C0 = splat_i8(c0);
    uint64_t C1 = splat_i8(c1);
    uint64_t C2 = splat_i8(c2);

    uint64_t acc = C2;

    acc = mul_packed_mac_ref(acc, Z);
    acc = macacc_ref(acc, C1, ONE);
    acc = mul_packed_mac_ref(acc, Z);
    acc = macacc_ref(acc, C0, ONE);

    return acc;
}

// -----------------------------------------------------------------------------
// Small helper to build random packed int8 vector in a safe range
// (here z in [-8, 7] to avoid too much saturation in this test)
// -----------------------------------------------------------------------------
static uint64_t random_packed_z(void)
{
    uint64_t z_vec = 0;

    for (int lane = 0; lane < 8; ++lane) {
        int8_t z = (int8_t)((rand() % 16) - 8);  // in [-8, 7]
        z_vec |= ((uint64_t)(uint8_t)z << (lane * 8));
    }

    return z_vec;
}

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
int main(void)
{
    srand(12345);

    const int NUM_TESTS = 10000;
    int errors = 0;

    printf("Testing polynomial P(z) = z^2 + z via MACACC (Horner)...\n");
    printf("Running %d random packed tests...\n", NUM_TESTS);

    for (int i = 0; i < NUM_TESTS; ++i) {
        uint64_t z_vec = random_packed_z();

        uint64_t ref = poly_ref(z_vec);
        uint64_t hw  = poly_hw(z_vec);

        if (ref != hw) {
            ++errors;
            printf("Mismatch at test %d\n", i);
            printf("  z_vec = 0x%016llx\n", (unsigned long long)z_vec);
            printf("  ref   = 0x%016llx\n", (unsigned long long)ref);
            printf("  hw    = 0x%016llx\n", (unsigned long long)hw);
            if (errors > 10) {
                printf("Too many errors, stopping.\n");
                break;
            }
        }
    }

    if (errors == 0) {
        printf("Correctness: ALL POLY TESTS PASSED ✔️\n");
    } else {
        printf("Correctness: %d tests FAILED ❌\n", errors);
    }

    // -----------------------------------------------------------------
    // Micro-benchmark: many polynomial evaluations using MACACC
    // -----------------------------------------------------------------
    const int NUM_ITER = 1000000;
    uint64_t z_vec = random_packed_z();
    uint64_t acc_sum = 0;

    uint64_t start = rdcycle();
    for (int i = 0; i < NUM_ITER; ++i) {
        uint64_t out = poly_hw(z_vec);
        acc_sum ^= out;  // prevent dead-code elimination
    }
    uint64_t end = rdcycle();

    printf("Benchmark: %d poly evaluations via MACACC\n", NUM_ITER);
    printf("  cycles  = %llu\n", (unsigned long long)(end - start));
    printf("  acc_sum = 0x%016llx\n", (unsigned long long)acc_sum);

    return (errors == 0) ? 0 : 1;
}
