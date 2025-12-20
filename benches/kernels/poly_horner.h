#pragma once
#include <stdint.h>
#include <stddef.h>

#include "ops.h"

// Evaluate polynomial per-lane in packed int8 (8 lanes in uint64_t)
// Horner form with saturation at each step (via ops->macacc8):
//   P(z) = c0 + c1*z + ... + cD*z^D
//
// coeffs[k] is ck (int8 scalar coefficient broadcast to all lanes)
// degree D must be >= 0
uint64_t poly_horner_eval_i8x8(const ops_vtable_t* ops,
                               uint64_t z_vec,
                               const int8_t* coeffs,
                               int degree);

// Convenience example: P(z)=z^2+z  (coeffs: c0=0, c1=1, c2=1)
uint64_t poly_z2_plus_z_i8x8(const ops_vtable_t* ops, uint64_t z_vec);
