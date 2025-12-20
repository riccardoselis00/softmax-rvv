#include "poly_horner.h"

// Broadcast a signed int8 value to all 8 bytes of a 64-bit word
static inline uint64_t splat_i8(int8_t v)
{
    uint64_t b = (uint8_t)v;
    uint64_t r = b;
    r |= (r << 8);
    r |= (r << 16);
    r |= (r << 32);
    return r;
}

// Packed multiply: (a*b) lane-wise with sat_int8, obtained via macacc8(acc=0)
static inline uint64_t packed_mul_i8x8(const ops_vtable_t* ops, uint64_t a, uint64_t b)
{
    return ops->macacc8(0ULL, a, b);
}

// Packed add of a constant vector: acc := sat(acc + c_vec*1)
static inline uint64_t packed_add_i8x8(const ops_vtable_t* ops, uint64_t acc, uint64_t c_vec)
{
    const uint64_t ONE = splat_i8(1);
    return ops->macacc8(acc, c_vec, ONE);
}

uint64_t poly_horner_eval_i8x8(const ops_vtable_t* ops,
                               uint64_t z_vec,
                               const int8_t* coeffs,
                               int degree)
{
    if (!ops || !coeffs || degree < 0) return 0;

    // acc = cD
    uint64_t acc = splat_i8(coeffs[degree]);

    // Horner: acc = ((cD*z + cD-1)*z + ... + c0)
    for (int k = degree - 1; k >= 0; --k) {
        acc = packed_mul_i8x8(ops, acc, z_vec);             // acc *= z
        acc = packed_add_i8x8(ops, acc, splat_i8(coeffs[k])); // acc += ck
    }
    return acc;
}

uint64_t poly_z2_plus_z_i8x8(const ops_vtable_t* ops, uint64_t z_vec)
{
    // P(z)=z^2+z => coeffs: c0=0, c1=1, c2=1
    const int8_t coeffs[3] = {0, 1, 1};
    return poly_horner_eval_i8x8(ops, z_vec, coeffs, 2);
}
