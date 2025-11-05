// macacc_hotloop.c — compute-bound RV64 MAC microbench (operand-encoded + ILP + reuse)
// Build (example): riscv64-linux-gnu-gcc -O3 -march=rv64gc -mabi=lp64d \
//                  -DLANES=8 -DREUSE=64 -fomit-frame-pointer -frename-registers \
//                  macacc_hotloop.c -o macacc_hotloop.riscv
// Run under gem5:  ... -c ./macacc_hotloop.riscv -o "1048576"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef LANES
#define LANES 8          // 1,2,4,8… pick to match op latency / FU count (watch register pressure)
#endif
#ifndef REUSE
#define REUSE 64         // MACs per loaded (x,y); raises compute intensity so MAC FUs dominate
#endif

// Operand-encoded custom instruction: rd := rd + rs1*rs2 (low 64-bit)
#define MACACC_R(dest, x, y) \
  __asm__ (".insn r 0x5B, 0x02, 0x00, %0, %1, %2\n" \
           : "+&r"(dest) : "r"(x), "r"(y))

static inline void macacc_hotloop(const uint64_t* __restrict a,
                                  const uint64_t* __restrict b,
                                  size_t n,
                                  uint64_t* acc_out)
{
    // ---- lane accumulators (scalars => stay in registers) ----
#if   LANES==1
    uint64_t A0=0;
#elif LANES==2
    uint64_t A0=0,A1=0;
#elif LANES==4
    uint64_t A0=0,A1=0,A2=0,A3=0;
#elif LANES==8
    uint64_t A0=0,A1=0,A2=0,A3=0,A4=0,A5=0,A6=0,A7=0;
#else
# error "Choose LANES in {1,2,4,8} for this simple demo"
#endif

    size_t i = 0;
    size_t n_vec = (n / LANES) * LANES;

    for (; i < n_vec; i += LANES) {
        // ---- load one (x,y) pair per lane into scalars (in registers) ----
#if   LANES==1
        uint64_t x0=a[i+0], y0=b[i+0];
#elif LANES==2
        uint64_t x0=a[i+0], y0=b[i+0];
        uint64_t x1=a[i+1], y1=b[i+1];
#elif LANES==4
        uint64_t x0=a[i+0], y0=b[i+0];
        uint64_t x1=a[i+1], y1=b[i+1];
        uint64_t x2=a[i+2], y2=b[i+2];
        uint64_t x3=a[i+3], y3=b[i+3];
#elif LANES==8
        uint64_t x0=a[i+0], y0=b[i+0];
        uint64_t x1=a[i+1], y1=b[i+1];
        uint64_t x2=a[i+2], y2=b[i+2];
        uint64_t x3=a[i+3], y3=b[i+3];
        uint64_t x4=a[i+4], y4=b[i+4];
        uint64_t x5=a[i+5], y5=b[i+5];
        uint64_t x6=a[i+6], y6=b[i+6];
        uint64_t x7=a[i+7], y7=b[i+7];
#endif

        // ---- reuse the loaded values many times: compute >> memory ----
#pragma GCC unroll 8
        for (int r=0; r<REUSE; ++r) {
#if   LANES==1
            MACACC_R(A0, x0, y0);
#elif LANES==2
            MACACC_R(A0, x0, y0);  MACACC_R(A1, x1, y1);
#elif LANES==4
            MACACC_R(A0, x0, y0);  MACACC_R(A1, x1, y1);
            MACACC_R(A2, x2, y2);  MACACC_R(A3, x3, y3);
#elif LANES==8
            MACACC_R(A0, x0, y0);  MACACC_R(A1, x1, y1);
            MACACC_R(A2, x2, y2);  MACACC_R(A3, x3, y3);
            MACACC_R(A4, x4, y4);  MACACC_R(A5, x5, y5);
            MACACC_R(A6, x6, y6);  MACACC_R(A7, x7, y7);
#endif
        }
    }

    // tail (no reuse, small)
    uint64_t acc_tail = 0;
    for (; i < n; ++i) {
        MACACC_R(acc_tail, a[i], b[i]);
    }

    // reduce lanes
    uint64_t acc = acc_tail;
#if   LANES==1
    acc += A0;
#elif LANES==2
    acc += A0 + A1;
#elif LANES==4
    acc += A0 + A1 + A2 + A3;
#elif LANES==8
    acc += A0 + A1 + A2 + A3 + A4 + A5 + A6 + A7;
#endif
    *acc_out = acc;
}

int main(int argc, char **argv) {
    // Keep arrays reasonably small by default so they fit in cache → avoid memory bottlenecks.
    size_t N = (argc > 1) ? (size_t)strtoull(argv[1], 0, 10) : (1u << 16); // 65,536 by default

    // 64-byte alignment helps cachelines
    size_t bytes = N*sizeof(uint64_t);
    size_t padded = (bytes + 63) & ~((size_t)63);
    uint64_t *x = (uint64_t*)aligned_alloc(64, padded);
    uint64_t *y = (uint64_t*)aligned_alloc(64, padded);
    if (!x || !y) { puts("alloc_fail"); return 1; }

    // deterministic init
    for (size_t i=0;i<N;++i){
        x[i] = i*1315423911u + 0x9E3779B97F4A7C15ull;
        y[i] = (~i)*2654435761u + 0xD2B74407B1CE6E93ull;
    }

    uint64_t acc = 0;
    macacc_hotloop(x, y, N, &acc);

    printf("N=%zu LANES=%d REUSE=%d acc=%llu\n",
           N, LANES, REUSE, (unsigned long long)acc);

    free(x); free(y);
    return 0;
}
