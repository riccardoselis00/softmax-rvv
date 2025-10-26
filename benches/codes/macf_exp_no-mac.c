// exp-bench-nofma.c — exp approximation with Horner + ILP, forcing NO FMA (fmul.s + fadd.s)
// Usage: ./exp-bench-nofma.riscv <N_elems> [K_lanes (default 8)]
// Design:
//  - Range reduction: exp(x) = 2^k * 2^r, y=x*log2(e); k≈nearest(y); r=y−k in [-0.5,0.5]
//  - Degree-5 Horner on 2^r: p = ((((c5*r + c4)*r + c3)*r + c2)*r + c1)*r + c0
//  - K independent lanes per iter (ILP)
//  - Each Horner step is TWO FP ops via inline asm: fmul.s then fadd.s (no fusion possible)
//  - libm-free: custom nearest-int + scalbnf-lite (bit hack)

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef RESTRICT
#  if defined(__GNUC__)
#    define RESTRICT __restrict__
#  else
#    define RESTRICT
#  endif
#endif

// ===== Polynomial for 2^r on r in [-0.5, 0.5] : degree-5 (fast)
static const float c0 = 1.0f;
static const float c1 = 0.69314718055994531f;          // ln(2)
static const float c2 = 0.24022650695910070f;          // (ln2)^2/2
static const float c3 = 0.05550410866482158f;          // (ln2)^3/6
static const float c4 = 0.009618129107628477f;         // (ln2)^4/24
static const float c5 = 0.0013333558146428447f;        // (ln2)^5/120

static const float LOG2E = 1.44269504088896341f;       // log2(e)

// ===== libm-free helpers

// Nearest integer, ties-away (good enough for DSE; fast)
static inline __attribute__((always_inline))
int fast_round_nearest(float y) {
    return (int)(y >= 0.0f ? y + 0.5f : y - 0.5f);
}

// Fast scalbnf-like: multiply x by 2^k (normalized inputs; saturates to +/-INF on overflow)
static inline __attribute__((always_inline))
float fast_scalbnf(float x, int k) {
    union { float f; uint32_t u; } v = { x };
    uint32_t sign = v.u & 0x80000000u;
    uint32_t exp  = (v.u >> 23) & 0xFFu;
    uint32_t man  = v.u & 0x007FFFFFu;

    if (exp == 0) {
        // Subnormal/zero: cheap path — return 0 (fine for our DSE input range)
        v.u = sign; 
        return v.f;
    }
    int e = (int)exp - 127 + k;
    if (e <= 0) { v.u = sign; return v.f; }                      // underflow → +/-0
    if (e >= 255-127) { v.u = sign | 0x7F800000u; return v.f; }  // overflow → +/-INF

    v.u = sign | ((uint32_t)(e + 127) << 23) | man;
    return v.f;
}

// Range reduction: exp(x) = 2^k * 2^r
static inline __attribute__((always_inline))
void range_reduce(float x, int *k_out, float *r_out) {
    float y = x * LOG2E;
    int   k = fast_round_nearest(y);      // nearest integer (no libm)
    float r = y - (float)k;               // r in ~[-0.5, 0.5]
    *k_out = k; *r_out = r;
}

// ===== Inline asm FP ops — force NO fusion
static inline __attribute__((always_inline))
float fmul_s(float a, float b) {
    float d;
    asm volatile("fmul.s %0, %1, %2" : "=f"(d) : "f"(a), "f"(b));
    return d;
}
static inline __attribute__((always_inline))
float fadd_s(float a, float b) {
    float d;
    asm volatile("fadd.s %0, %1, %2" : "=f"(d) : "f"(a), "f"(b));
    return d;
}

// One Horner step with separate mul + add: p = p*r + c
static inline __attribute__((always_inline))
float horner_step_nofma(float p, float r, float c) {
    float t = fmul_s(p, r);
    return fadd_s(t, c);
}

// ===== K-lane ILP Horner kernel (no FMA)
__attribute__((noinline))
void bench_exp_horner_nofma(const float* RESTRICT in, float* RESTRICT out,
                            size_t N, int K)
{
    if (K < 1) K = 1;
    if (K > 16) K = 16;

    size_t i = 0;
    for (; i + (size_t)K <= N; i += (size_t)K) {
        int   k[16];
        float r[16];
        for (int lane = 0; lane < K; ++lane)
            range_reduce(in[i + lane], &k[lane], &r[lane]);

        // Horner init
        float p[16];
        for (int lane = 0; lane < K; ++lane) p[lane] = c5;

        // 5 Horner steps (each: fmul.s + fadd.s)
        for (int lane = 0; lane < K; ++lane) p[lane] = horner_step_nofma(p[lane], r[lane], c4);
        for (int lane = 0; lane < K; ++lane) p[lane] = horner_step_nofma(p[lane], r[lane], c3);
        for (int lane = 0; lane < K; ++lane) p[lane] = horner_step_nofma(p[lane], r[lane], c2);
        for (int lane = 0; lane < K; ++lane) p[lane] = horner_step_nofma(p[lane], r[lane], c1);
        for (int lane = 0; lane < K; ++lane) p[lane] = horner_step_nofma(p[lane], r[lane], c0);

        // Scale by 2^k (libm-free)
        for (int lane = 0; lane < K; ++lane)
            out[i + lane] = fast_scalbnf(p[lane], k[lane]);
    }

    // Tail
    for (; i < N; ++i) {
        int k; float r; range_reduce(in[i], &k, &r);
        float p = c5;
        p = horner_step_nofma(p, r, c4);
        p = horner_step_nofma(p, r, c3);
        p = horner_step_nofma(p, r, c2);
        p = horner_step_nofma(p, r, c1);
        p = horner_step_nofma(p, r, c0);
        out[i] = fast_scalbnf(p, k);
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
        fprintf(stderr, "Usage: %s <N_elems> [K_lanes (default 8)]\n", argv[0]);
        return 1;
    }
    size_t N = (size_t)strtoull(argv[1], 0, 10);
    int K = (argc >= 3) ? atoi(argv[2]) : 8;
    if (K < 1) K = 1; if (K > 16) K = 16;

    float* in  = (float*)aligned_alloc(64, N * sizeof(float));
    float* out = (float*)aligned_alloc(64, N * sizeof(float));
    if (!in || !out) { fprintf(stderr, "alloc failed\n"); return 2; }

    uint32_t s = 1u;
    for (size_t i = 0; i < N; ++i) in[i] = lcg32f(&s);

    bench_exp_horner_nofma(in, out, N, K);

    // keep results live
    volatile float sink = 0.0f;
    for (size_t i = 0; i < (N > 1024 ? 1024 : N); ++i) sink += out[i]*1e-38f;

    free(in); free(out);
    return 0;
}
