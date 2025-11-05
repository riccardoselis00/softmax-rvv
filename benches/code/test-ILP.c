// macacc_mini_ilp_insn.c — RV64 MAC benchmark using .insn (operand-encoded) + ILP
// Build: riscv64-linux-gnu-gcc -O3 -march=rv64gc -mabi=lp64d macacc_mini_ilp_insn.c -o macacc_mini_ilp_insn.riscv
// Run (gem5): build/RISCV/gem5.opt configs/deprecated/example/se.py --cpu-type=O3CPU --caches --l2cache -c ./macacc_mini_ilp_insn.riscv
//
// Notes:
//  - Custom integer MAC: acc := acc + a*b
//  - Encoding fields are filled from %0/%1/%2 (NO fixed a5/a4/a3), enabling true ILP.
//  - No timing in software; rely on gem5 stats. A single printf keeps work from being DCE'd.

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef LANES
#define LANES 4             // Choose 2–8 based on MAC latency, FU count, and register pressure
#endif

// Operand-encoded custom instruction (opcode/funct* per your design):
// Replace 0x5B/0x2/0x00 if you changed the decoder.
#define MACACC_R(dest, x, y)                                                     \
  __asm__ (".insn r 0x5B, 0x02, 0x00, %0, %1, %2\n"                              \
           : "+&r"(dest)     /* read-modify-write accumulator (early-clobber) */ \
           : "r"(x), "r"(y)  /* multiplicand, multiplier */                      \
           : /* no clobbers */ )

static inline void macacc_block_ilp(const uint64_t* __restrict a,
                                    const uint64_t* __restrict b,
                                    size_t n,
                                    uint64_t* acc_out)
{
    // Multiple independent accumulators to expose ILP and feed multiple MAC units.
    uint64_t A[LANES];
    for (int k = 0; k < LANES; ++k) A[k] = 0;

    size_t i = 0;
    size_t n_vec = (n / LANES) * LANES;

    // Main ILP loop (unrolled by LANES)
#pragma GCC ivdep
#pragma GCC unroll 8
    for (; i < n_vec; i += LANES) {
        // Load a/b for each lane, then issue MACs back-to-back.
        // The compiler will register-allocate x/y/A across different regs
        // and schedule to cover MAC pipeline latency.
        uint64_t x[LANES], y[LANES];
        for (int k = 0; k < LANES; ++k) {
            x[k] = a[i + k];
            y[k] = b[i + k];
        }
        for (int k = 0; k < LANES; ++k) {
            MACACC_R(A[k], x[k], y[k]);
        }
    }

    // Tail
    for (; i < n; ++i) {
        uint64_t t = 0;
        MACACC_R(t, a[i], b[i]);   // scalar fallback
        A[0] += t;                  // fold tail into lane 0
    }

    // Reduce lanes
    uint64_t acc = 0;
    for (int k = 0; k < LANES; ++k) acc += A[k];
    *acc_out = acc;
}

int main(int argc, char **argv) {
    size_t N = (argc > 1) ? (size_t)strtoull(argv[1], 0, 10) : (1u << 20); // default 1M

    uint64_t *x = (uint64_t*)aligned_alloc(64, N*sizeof(uint64_t));
    uint64_t *y = (uint64_t*)aligned_alloc(64, N*sizeof(uint64_t));
    if (!x || !y) { puts("alloc_fail"); return 1; }

    for (size_t i=0;i<N;++i){
        x[i] = i*1315423911u + 0x9E3779B97F4A7C15ull;
        y[i] = (~i)*2654435761u + 0xD2B74407B1CE6E93ull;
    }

    uint64_t acc = 0;
    macacc_block_ilp(x, y, N, &acc);

    // Minimal output to keep work live; use gem5 metrics for analysis
    printf("N=%zu LANES=%d acc=%llu\n", N, LANES, (unsigned long long)acc);

    free(x); free(y);
    return 0;
}
