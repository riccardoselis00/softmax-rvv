#include "ops.h"
#include "rv_insn.h"

// Hardware MACACC wrapper (matches your macc-2.c)
static uint64_t macacc8_custom(uint64_t acc, uint64_t a, uint64_t b) {
  MACACC_R(acc, a, b);
  return acc;
}

// Hardware PDIV8 wrapper (matches your cdiv-1.c)
static uint64_t pdiv8_custom(uint64_t a, uint64_t b) {
  uint64_t q;
  PDIV8_R(q, a, b);
  return q;
}

const ops_vtable_t VCUSTOM = {
  .macacc8 = macacc8_custom,
  .pdiv8   = pdiv8_custom
};
