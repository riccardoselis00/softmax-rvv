#pragma once
#include <stdint.h>

typedef enum {
  OPS_IMPL_REF = 0,
  OPS_IMPL_CUSTOM = 1
} ops_impl_t;

// Base ops API: everything (poly/softmax/...) calls ONLY these.
typedef struct {
  // MACACC: acc <- sat(acc + a*b) lane-wise
  uint64_t (*macacc8)(uint64_t acc, uint64_t a, uint64_t b);

  // CustomDiv / PDIV8: q <- sat(a/b) lane-wise, div0->0
  uint64_t (*pdiv8)(uint64_t a, uint64_t b);
} ops_vtable_t;

// Return requested backend if available; otherwise fall back to REF.
const ops_vtable_t* ops_get(ops_impl_t impl);
