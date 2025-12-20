#include "ops.h"

static inline int8_t lane_i8(uint64_t x, int lane) {
  return (int8_t)((x >> (lane * 8)) & 0xFF);
}

static inline uint64_t pack_lane_u8(uint64_t out, int lane, int16_t v_sat) {
  // v_sat is guaranteed in [-128,127]
  uint64_t shift = (uint64_t)(lane * 8);
  out |= ((uint64_t)((uint8_t)v_sat) << shift);
  return out;
}

// EXACT semantics from your macc-2.c
static uint64_t macacc8_ref(uint64_t acc, uint64_t a, uint64_t b) {
  uint64_t out = 0;
  for (int lane = 0; lane < 8; ++lane) {
    int8_t acc_lane = lane_i8(acc, lane);
    int8_t a_lane   = lane_i8(a, lane);
    int8_t b_lane   = lane_i8(b, lane);

    int16_t sum = (int16_t)acc_lane + (int16_t)a_lane * (int16_t)b_lane;

    if (sum > 127)  sum = 127;
    if (sum < -128) sum = -128;

    out = pack_lane_u8(out, lane, sum);
  }
  return out;
}

// EXACT semantics from your cdiv-1.c
static uint64_t pdiv8_ref(uint64_t a, uint64_t b) {
  uint64_t out = 0;
  for (int lane = 0; lane < 8; ++lane) {
    int8_t a_lane = lane_i8(a, lane);
    int8_t b_lane = lane_i8(b, lane);

    int16_t q;
    if (b_lane == 0) q = 0;                           // div-by-zero -> 0
    else             q = (int16_t)a_lane / (int16_t)b_lane;

    if (q > 127)  q = 127;
    if (q < -128) q = -128;

    out = pack_lane_u8(out, lane, q);
  }
  return out;
}

static const ops_vtable_t VREF = {
  .macacc8 = macacc8_ref,
  .pdiv8   = pdiv8_ref
};

#if defined(SOFT_HAVE_CUSTOM_OPS) && (SOFT_HAVE_CUSTOM_OPS == 1)
extern const ops_vtable_t VCUSTOM;
#endif

const ops_vtable_t* ops_get(ops_impl_t impl) {
#if defined(SOFT_HAVE_CUSTOM_OPS) && (SOFT_HAVE_CUSTOM_OPS == 1)
  if (impl == OPS_IMPL_CUSTOM) return &VCUSTOM;
#endif
  return &VREF;
}
