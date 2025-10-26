// exp-bench-word-ilp.c — MAC-heavy exp approximation with Horner + ILP (via `.word`)
// Usage: ./exp-bench.riscv <N_elems> [K_lanes (1..8)]
// Notes:
//  - One simple algorithm: Horner per lane (p = p*r + c) → ~5 MACs/elem/lane.
//  - K independent lanes per iteration → strong ILP to light up `count` and pipelining.
//  - Emits MACs using fixed `.word` encodings for each lane's (rd=rs1, rs2). rs3 is shared (f13).

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifndef RESTRICT
#  if defined(__GNUC__)
#    define RESTRICT __restrict__
#  else
#    define RESTRICT
#  endif
#endif

// ----- Polynomial for 2^r on r in [-0.5, 0.5] : degree-5 (fast, MAC-friendly)
static const float c0 = 1.0f;
static const float c1 = 0.69314718055994531f;          // ln(2)
static const float c2 = 0.24022650695910070f;          // (ln2)^2/2
static const float c3 = 0.05550410866482158f;          // (ln2)^3/6
static const float c4 = 0.009618129107628477f;         // (ln2)^4/24
static const float c5 = 0.0013333558146428447f;        // (ln2)^5/120

static const float LOG2E = 1.44269504088896341f; // log2(e)

// ----- Pre-encoded `.word` for MAC: rd=rs1 (lane accumulator), rs2=lane's r, rs3=f13 (coeff)
// Encoding: word = (rs3<<27) | (funct2=0<<25) | (rs2<<20) | (rs1<<15) | (rm=7<<12) | (rd<<7) | (opcode=0x5B)
// Here rs3=f13 for all, rm=7 (dyn rounding), funct2=0 (your MAC), opcode=0x5B.

// Lane mapping (K up to 8):
//   Lane 0: rd=rs1=f15, rs2=f8
//   Lane 1: rd=rs1=f16, rs2=f9
//   Lane 2: rd=rs1=f17, rs2=f10
//   Lane 3: rd=rs1=f18, rs2=f11
//   Lane 4: rd=rs1=f19, rs2=f12
//   Lane 5: rd=rs1=f20, rs2=f24
//   Lane 6: rd=rs1=f21, rs2=f25
//   Lane 7: rd=rs1=f22, rs2=f26

#define DO_MAC_F15_RS2_F8(acc, creg)  asm volatile(".word 0x6887F7DB" : "+f"(acc) : "f"(creg) : "memory")
#define DO_MAC_F16_RS2_F9(acc, creg)  asm volatile(".word 0x6898785B" : "+f"(acc) : "f"(creg) : "memory")
#define DO_MAC_F17_RS2_F10(acc,creg)  asm volatile(".word 0x68A8F8DB" : "+f"(acc) : "f"(creg) : "memory")
#define DO_MAC_F18_RS2_F11(acc,creg)  asm volatile(".word 0x68B9795B" : "+f"(acc) : "f"(creg) : "memory")
#define DO_MAC_F19_RS2_F12(acc,creg)  asm volatile(".word 0x68C9F9DB" : "+f"(acc) : "f"(creg) : "memory")
#define DO_MAC_F20_RS2_F24(acc,creg)  asm volatile(".word 0x698A7A5B" : "+f"(acc) : "f"(creg) : "memory")
#define DO_MAC_F21_RS2_F25(acc,creg)  asm volatile(".word 0x699AFADB" : "+f"(acc) : "f"(creg) : "memory")
#define DO_MAC_F22_RS2_F26(acc,creg)  asm volatile(".word 0x69AB7B5B" : "+f"(acc) : "f"(creg) : "memory")

// Dispatch per lane (fallthrough switch to emit K MACs per step)
#define DO_STEP_K(K, creg, acc0,acc1,acc2,acc3,acc4,acc5,acc6,acc7) do { \
    switch (K) { \
      case 8: DO_MAC_F22_RS2_F26(acc7, creg); \
      case 7: DO_MAC_F21_RS2_F25(acc6, creg); \
      case 6: DO_MAC_F20_RS2_F24(acc5, creg); \
      case 5: DO_MAC_F19_RS2_F12(acc4, creg); \
      case 4: DO_MAC_F18_RS2_F11(acc3, creg); \
      case 3: DO_MAC_F17_RS2_F10(acc2, creg); \
      case 2: DO_MAC_F16_RS2_F9 (acc1, creg); \
      case 1: DO_MAC_F15_RS2_F8 (acc0, creg); \
      default: break; \
    } \
} while(0)

// // Range reduction: exp(x) = 2^k * 2^r, with y = x*log2(e), k=nearest(y), r=y-k in [-0.5, 0.5]
// static inline __attribute__((always_inline))
// void range_reduce(float x, int *k_out, float *r_out) {
//     float y = x * LOG2E;
//     int   k = (int)lrintf(y);
//     float r = y - (float)k;
//     *k_out = k; *r_out = r;
// }

// Fast nearest integer, ties-away (no libm)
static inline __attribute__((always_inline))
int fast_round_nearest(float y) {
    return (int)(y >= 0.0f ? y + 0.5f : y - 0.5f);
}

// Range reduction: exp(x) = 2^k * 2^r
static inline __attribute__((always_inline))
void range_reduce(float x, int *k_out, float *r_out) {
    float y = x * LOG2E;
    int   k = fast_round_nearest(y);
    float r = y - (float)k;      // r ∈ [-0.5, 0.5]
    *k_out = k; *r_out = r;
}


static volatile float sink;

// ILP Horner kernel: K lanes per iteration, each lane: p = (((((c5*r)+c4)*r)+c3)*r + c2)*r + c1)*r + c0
__attribute__((noinline))
void bench_exp_horner_word_ilp(const float* RESTRICT in, float* RESTRICT out,
                               size_t N, int K)
{
    if (K < 1) K = 1; if (K > 8) K = 8;
    size_t i = 0;

    for (; i + (size_t)K <= N; i += (size_t)K) {
        // 1) Range reduction (scalar code; light)
        int   k[8];
        float r[8];
        for (int lane = 0; lane < K; ++lane) {
            range_reduce(in[i + lane], &k[lane], &r[lane]);
        }

        // 2) Pin rs2 (r) per lane to distinct FP regs (f8,f9,f10,f11,f12,f24,f25,f26)
        register float r0 asm("f8")  = (K>=1)? r[0]:0.0f;
        register float r1 asm("f9")  = (K>=2)? r[1]:0.0f;
        register float r2 asm("f10") = (K>=3)? r[2]:0.0f;
        register float r3 asm("f11") = (K>=4)? r[3]:0.0f;
        register float r4 asm("f12") = (K>=5)? r[4]:0.0f;
        register float r5 asm("f24") = (K>=6)? r[5]:0.0f;
        register float r6 asm("f25") = (K>=7)? r[6]:0.0f;
        register float r7 asm("f26") = (K>=8)? r[7]:0.0f;
        (void)r0;(void)r1;(void)r2;(void)r3;(void)r4;(void)r5;(void)r6;(void)r7;

        // 3) Lane accumulators p in rd=rs1 regs (f15..f22) ← start at c5
        register float p0 asm("f15") = c5;
        register float p1 asm("f16") = c5;
        register float p2 asm("f17") = c5;
        register float p3 asm("f18") = c5;
        register float p4 asm("f19") = c5;
        register float p5 asm("f20") = c5;
        register float p6 asm("f21") = c5;
        register float p7 asm("f22") = c5;

        // 4) Broadcast coefficients in rs3=f13 and do K MACs per step
        register float creg asm("f13");

        creg = c4; DO_STEP_K(K, creg, p0,p1,p2,p3,p4,p5,p6,p7); // p = p*r + c4
        creg = c3; DO_STEP_K(K, creg, p0,p1,p2,p3,p4,p5,p6,p7); // p = p*r + c3
        creg = c2; DO_STEP_K(K, creg, p0,p1,p2,p3,p4,p5,p6,p7); // p = p*r + c2
        creg = c1; DO_STEP_K(K, creg, p0,p1,p2,p3,p4,p5,p6,p7); // p = p*r + c1
        creg = c0; DO_STEP_K(K, creg, p0,p1,p2,p3,p4,p5,p6,p7); // p = p*r + c0

        // 5) Scale by 2^k and store
        if (K>=1) out[i+0] = scalbnf(p0, k[0]);
        if (K>=2) out[i+1] = scalbnf(p1, k[1]);
        if (K>=3) out[i+2] = scalbnf(p2, k[2]);
        if (K>=4) out[i+3] = scalbnf(p3, k[3]);
        if (K>=5) out[i+4] = scalbnf(p4, k[4]);
        if (K>=6) out[i+5] = scalbnf(p5, k[5]);
        if (K>=7) out[i+6] = scalbnf(p6, k[6]);
        if (K>=8) out[i+7] = scalbnf(p7, k[7]);
    }

    // Tail (scalar)
    for (; i < N; ++i) {
        int k; float r;
        range_reduce(in[i], &k, &r);
        // Scalar Horner (no ILP, still using normal ops for tail)
        float p = (((((c5*r)+c4)*r + c3)*r + c2)*r + c1)*r + c0;
        out[i] = scalbnf(p, k);
    }
}

// Simple input generator in [-10, 10]
static inline float lcg32f(uint32_t *s) {
    *s = *s * 1664525u + 1013904223u;
    float u = (float)(*s) * (1.0f/4294967296.0f);
    return (u * 20.0f) - 10.0f;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <N_elems> [K_lanes (1..8, default 8)]\n", argv[0]);
        return 1;
    }
    size_t N = (size_t)strtoull(argv[1], 0, 10);
    int K = (argc >= 3) ? atoi(argv[2]) : 8;
    if (K < 1) K = 1; if (K > 8) K = 8;

    float* in  = (float*)aligned_alloc(64, N*sizeof(float));
    float* out = (float*)aligned_alloc(64, N*sizeof(float));
    if (!in || !out) { fprintf(stderr, "alloc failed\n"); return 2; }

    uint32_t s = 1u;
    for (size_t i=0;i<N;++i) in[i] = lcg32f(&s);

    bench_exp_horner_word_ilp(in, out, N, K);

    // keep result live
    volatile float sink = 0.0f;
    for (size_t i=0;i<(N>1024?1024:N); ++i) sink += out[i]*1e-38f;

    free(in); free(out);
    return 0;
}
