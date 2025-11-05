// macacc_mini_word.c — minimal RV64 MAC benchmark using .word (fixed registers)
// Build: riscv64-linux-gnu-gcc -O3 -march=rv64gc -mabi=lp64d macacc_mini_word.c -o macacc_mini_word.riscv
// Run (gem5): build/RISCV/gem5.opt configs/deprecated/example/se.py --cpu-type=O3CPU --caches --l2cache -c ./macacc_mini_word.riscv

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static inline uint64_t rdcycle(void) { uint64_t c; asm volatile("rdcycle %0":"=r"(c)); return c; }

// Integer MACACC (rd := rd + rs1*rs2), fixed regs: rd=a5, rs1=a4, rs2=a3
// Encoding: opcode=0x5B, funct3=0x2, funct7=0x00, rd=15, rs1=14, rs2=13 => 0x00D727DB

static inline uint64_t macacc_step(uint64_t acc, uint64_t a, uint64_t b) {
    // register uint64_t rd asm("a5") = acc;  // rd (in/out)
    // register uint64_t r1 asm("a4") = a;    // rs1
    // register uint64_t r2 asm("a3") = b;    // rs2
    // asm volatile (".word 0x00D727DB"       // macacc a5,a4,a3  (CUSTOM_2, funct3=0x2)
    //               : "+r"(rd) : "r"(r1), "r"(r2) : "memory");

    // Alternative: without fixed registers (slightly slower)

    register uint64_t r1 asm("a4") = a;
    register uint64_t r2 asm("a3") = b;
    uint64_t rd;
    asm volatile (".word 0x00D727DB"
              : "=r"(rd)
              : "r"(r1), "r"(r2)
              : "memory");

    return rd;
}

int main(int argc, char **argv) {
    size_t N = (argc > 1) ? (size_t)strtoull(argv[1], 0, 10) : (1u << 20); // 1M
    uint64_t *x = (uint64_t*)aligned_alloc(64, N*sizeof(uint64_t));
    uint64_t *y = (uint64_t*)aligned_alloc(64, N*sizeof(uint64_t));
    if (!x || !y) { puts("alloc_fail"); return 1; }
    for (size_t i=0;i<N;++i){ x[i]=i*1315423911u+0x9E3779B97F4A7C15ull; y[i]=(~i)*2654435761u+0xD2B74407B1CE6E93ull; }

    volatile uint64_t sink = 0;

    // mul+add
    uint64_t t0 = rdcycle();
    uint64_t acc0 = 0;
    for (size_t i=0;i<N;++i) acc0 += x[i]*y[i];
    uint64_t t1 = rdcycle();

    // macacc (.word)
    uint64_t t2 = rdcycle();
    uint64_t acc1 = 0;
    for (size_t i=0;i<N;++i) acc1 = macacc_step(acc1, x[i], y[i]);
    uint64_t t3 = rdcycle();

    sink ^= acc0 ^ acc1;
    uint64_t cyc_muladd = t1 - t0, cyc_macacc = t3 - t2;
    double speedup = cyc_macacc ? (double)cyc_muladd / (double)cyc_macacc : 0.0;

    // ONE print
    printf("N=%zu mul+add=%llu macacc=%llu speedup=%.3fx acc0=%llu acc1=%llu sink=%llu\n",
           N, (unsigned long long)cyc_muladd, (unsigned long long)cyc_macacc,
           speedup, (unsigned long long)acc0, (unsigned long long)acc1, (unsigned long long)sink);

    free(x); free(y);
    return 0;
}
