#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// -----------------------------------------------------------------------------
// Custom MACACC instruction: 8x int8 lanes, packed in 64-bit
// rd[i] := sat_int8( rd[i] + rs1[i] * rs2[i] )
// -----------------------------------------------------------------------------
#define MACACC_R(dest, x, y) \
  __asm__ volatile (".insn r 0x5B, 0x02, 0x00, %0, %1, %2" \
                    : "+&r"(dest) : "r"(x), "r"(y) : "memory")

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

// Pack 8 int8 to lanes of a 64-bit word
static inline uint64_t pack8_i8(const int8_t v[8])
{
    uint64_t r = 0;
    for (int lane = 0; lane < 8; ++lane) {
        r |= ((uint64_t)(uint8_t)v[lane] << (8 * lane));
    }
    return r;
}

// Unpack 8 int8 from lanes of a 64-bit word
static inline void unpack8_i8(uint64_t x, int8_t out[8])
{
    for (int lane = 0; lane < 8; ++lane) {
        out[lane] = (int8_t)((x >> (8 * lane)) & 0xFF);
    }
}

// -----------------------------------------------------------------------------
// Polynomial: P(z) = 127 + 7*z  (per lane, int8 arithmetic with saturation)
//   Intended domain: z in [-16, 0]  (delta = x - max_x, clipped)
//   This yields values in [15, 127], monotone in z, all positive.
//   Horner form: P(z) = (c2*z + c1)*z + c0 with c2=0, c1=7, c0=127
// -----------------------------------------------------------------------------

// Hardware version: uses MACACC (packed, 8 lanes)
// Marked noinline so the compiler doesn't get too clever.
__attribute__((noinline))
static uint64_t poly_hw(uint64_t z_vec)
{
    const int8_t c0 = 127;
    const int8_t c1 = 7;
    const int8_t c2 = 0;

    uint64_t Z   = z_vec;
    uint64_t ONE = splat_i8(1);

    uint64_t C0 = splat_i8(c0);
    uint64_t C1 = splat_i8(c1);
    uint64_t C2 = splat_i8(c2);

    // Horner:
    // acc = c2
    uint64_t acc = C2;

    // acc = acc * z
    acc = mul_packed_mac_hw(acc, Z);   // acc = c2*z = 0

    // acc = acc + c1
    MACACC_R(acc, C1, ONE);            // acc = 7

    // acc = acc * z
    acc = mul_packed_mac_hw(acc, Z);   // acc = 7*z

    // acc = acc + c0
    MACACC_R(acc, C0, ONE);            // acc = 7*z + 127

    return acc;
}

// Reference version: same Horner scheme, but in pure C using macacc_ref
static uint64_t poly_ref(uint64_t z_vec)
{
    const int8_t c0 = 127;
    const int8_t c1 = 7;
    const int8_t c2 = 0;

    uint64_t Z   = z_vec;
    uint64_t ONE = splat_i8(1);

    uint64_t C0 = splat_i8(c0);
    uint64_t C1 = splat_i8(c1);
    uint64_t C2 = splat_i8(c2);

    uint64_t acc = C2;

    acc = mul_packed_mac_ref(acc, Z);      // acc = 0
    acc = macacc_ref(acc, C1, ONE);        // acc = 7
    acc = mul_packed_mac_ref(acc, Z);      // acc = 7*z
    acc = macacc_ref(acc, C0, ONE);        // acc = 7*z + 127

    return acc;
}

// -----------------------------------------------------------------------------
// Small helper to build random packed int8 vector with z in [-16, 0]
// (this matches the domain we want for the polynomial).
// -----------------------------------------------------------------------------
static uint64_t random_packed_z(void)
{
    uint64_t z_vec = 0;

    for (int lane = 0; lane < 8; ++lane) {
        int8_t z = (int8_t)((rand() % 17) - 16);  // in [-16, 0]
        z_vec |= ((uint64_t)(uint8_t)z << (lane * 8));
    }

    return z_vec;
}

// Volatile sink to force side-effect in the benchmark loop
volatile uint64_t sink;

// -----------------------------------------------------------------------------
// SOFTMAX IMPLEMENTATION (baseline, integer, using poly_hw as "exp-like")
// -----------------------------------------------------------------------------

// Clip delta into [-16, 0] range
static inline int8_t clip_delta(int8_t d)
{
    if (d < -16) d = -16;
    if (d > 0)   d = 0;
    return d;
}

// Baseline softmax over N elements (N must be multiple of 8 for now).
// x: int8 logits, out: int8 probabilities ~[0,255]
void softmax_int8_poly_macacc(const int8_t *x, int8_t *out, int N)
{
    // 1) Find max for numerical stability
    int8_t max_x = x[0];
    for (int i = 1; i < N; ++i) {
        if (x[i] > max_x) max_x = x[i];
    }

    // 2) Compute deltas, approximate exp, accumulate sum of numerators
    int32_t sum_num = 0;

    for (int i = 0; i < N; i += 8) {
        int8_t delta[8];
        int8_t num[8];

        // Compute delta = x_i - max_x and clip to [-16,0]
        for (int lane = 0; lane < 8; ++lane) {
            int idx = i + lane;
            delta[lane] = clip_delta((int8_t)(x[idx] - max_x));
        }

        // Pack and call polynomial exp approximation using MACACC
        uint64_t z_vec  = pack8_i8(delta);
        uint64_t numvec = poly_hw(z_vec);
        unpack8_i8(numvec, num);

        // Accumulate sum and store numerators
        // HERE: in the future, PSUM8 custom instruction could replace
        //       this small scalar loop and do a packed reduction in 1 inst.
        for (int lane = 0; lane < 8; ++lane) {
            int idx = i + lane;
            uint8_t nu = (uint8_t)num[lane];  // num is guaranteed >= 0
            out[idx] = (int8_t)nu;            // store raw numerator
            sum_num += nu;
        }
    }

    if (sum_num <= 0) {
        // Fallback: if everything underflows (unlikely with this poly), set uniform
        int8_t val = (int8_t)(255 / N);
        for (int i = 0; i < N; ++i) out[i] = val;
        return;
    }

    // 3) Normalize: p_i ≈ num_i / sum_num scaled to [0,255]
    // Baseline: use integer division (slow but simple).
    // In the future:
    //    - compute recip = QRECIP(sum_num)
    //    - then p_i = (num_i * recip) >> shift  (no division in the loop).
    const int SCALE = 255;

    for (int i = 0; i < N; ++i) {
        uint8_t num_i = (uint8_t)out[i];
        int32_t scaled = (int32_t)num_i * SCALE;
        int32_t p = scaled / sum_num;   // <-- HERE: candidate for QRECIP-based optimization
        if (p > 255) p = 255;
        out[i] = (int8_t)p;
    }
}

// -----------------------------------------------------------------------------
// Simple tests for softmax_int8_poly_macacc
// -----------------------------------------------------------------------------

static void print_vec_i8(const char *label, const int8_t *v, int N)
{
    printf("%s:", label);
    for (int i = 0; i < N; ++i) {
        printf(" %4d", v[i]);
    }
    printf("\n");
}

static void test_softmax_case(const char *name, const int8_t *x, int N)
{
    int8_t out[16]; // enough for N<=16; we will use N=8 in tests
    softmax_int8_poly_macacc(x, out, N);

    printf("\n[Softmax test: %s]\n", name);
    print_vec_i8("logits", x, N);
    print_vec_i8("probs ", out, N);

    // Basic checks: sum of outputs is in a reasonable range around 255
    int sum_out = 0;
    for (int i = 0; i < N; ++i)
        sum_out += (uint8_t)out[i];

    printf("sum_out = %d\n", sum_out);

    if (sum_out < 220 || sum_out > 290) {
        printf("WARNING: sum_out is far from 255 (approx softmax sum)\n");
    }

    // Check monotonicity: if x[i] > x[j], then out[i] >= out[j]
    int monotonic_ok = 1;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (x[i] > x[j] && out[i] < out[j]) {
                monotonic_ok = 0;
            }
        }
    }

    if (!monotonic_ok) {
        printf("WARNING: monotonicity check failed (approximation too rough).\n");
    } else {
        printf("Monotonicity check passed.\n");
    }
}

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
int main(void)
{
    srand(12345);

    const int NUM_TESTS = 10000;
    int errors = 0;

    printf("Testing polynomial P(z) = 127 + 7*z via MACACC (Horner)...\n");
    printf("Running %d random packed tests...\n", NUM_TESTS);

    // Correctness phase for polynomial vs reference
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
    const int NUM_ITER = 100;
    uint64_t z_vec = random_packed_z();
    uint64_t acc_sum = 0;

    uint64_t start = rdcycle();
    for (int i = 0; i < NUM_ITER; ++i) {
        // change z_vec every iteration so poly_hw's input isn't constant
        z_vec += 0x0101010101010101ULL;

        uint64_t out = poly_hw(z_vec);

        // force an observable side-effect
        sink = out;

        // avoid trivial algebraic simplifications like XOR with constant
        acc_sum += (out & 0xFFu);
    }
    uint64_t end = rdcycle();

    printf("Benchmark: %d poly evaluations via MACACC\n", NUM_ITER);
    printf("  cycles  = %llu\n", (unsigned long long)(end - start));
    printf("  acc_sum = 0x%016llx\n", (unsigned long long)acc_sum);

    // -----------------------------------------------------------------
    // Softmax tests (N=8, baseline integer softmax with MACACC polynomial)
    // -----------------------------------------------------------------
    //static const int N = 8;
    #define N 8

    int8_t x_uniform[N] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int8_t x_biased[N]  = { 0, -8, -8, -8, -8, -8, -8, -8 };
    int8_t x_incr[N]    = { -8, -4, -2, 0, 1, 2, 3, 4 };

    test_softmax_case("uniform (all equal)", x_uniform, N);
    test_softmax_case("one dominant (index 0)", x_biased, N);
    test_softmax_case("increasing logits", x_incr, N);

    return (errors == 0) ? 0 : 1;
}
