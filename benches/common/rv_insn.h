#pragma once
#include <stdint.h>

#if !defined(__riscv)
#error "rv_insn.h is for RISC-V builds only (custom .insn)."
#endif

// Custom MACACC instruction: 8x int8 lanes packed in 64-bit
// rd[i] := sat_int8( rd[i] + rs1[i] * rs2[i] )
#define MACACC_R(dest, x, y) \
  __asm__ volatile (".insn r 0x5B, 0x02, 0x00, %0, %1, %2" \
                    : "+&r"(dest) : "r"(x), "r"(y): "memory")

// Custom PDIV8 instruction: 8x int8 lanes packed in 64-bit
// rd[i] := sat_int8( rs1[i] / rs2[i] ), div-by-zero -> 0
#define PDIV8_R(dest, x, y) \
  __asm__ volatile (".insn r 0x5B, 0x03, 0x00, %0, %1, %2" \
                    : "=r"(dest) : "r"(x), "r"(y))

// If you prefer the name "CustomDiv", keep an alias:
#define CUSTOMDIV8_R(dest, x, y) PDIV8_R(dest, x, y)
