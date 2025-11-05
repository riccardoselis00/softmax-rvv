// softmax_int_native.c
// Integer-only softmax baseline (Barrett + degree-2 poly).
// int_softmax_baseline uses only native operations (casts, integer math).
// Optional float comparator uses exp() and requires -lm to link.

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>   // only used by float comparator; remove if you don't want libm
#include <stdlib.h>

#ifndef USE_INT128
#warning "This code expects __int128 for wide intermediates. Use GCC/Clang."
#endif

// ---- parameters (algorithm constants) ----
static const double A_coef = 0.3585;   // a
static const double B_coef = 1.353;    // b
static const double C_coef = 0.344;    // c

// ln(2) as a compile-time constant (no log() call)
static const double LN2 = 0.693147180559945309417232121458176568;

// // Simple float softmax for comparison (optional - uses libm exp)
// void float_softmax(const double *x, int N, double *out) {
//     double m = x[0];
//     for (int i=1;i<N;i++) if (x[i] > m) m = x[i];
//     double sum = 0.0;
//     for (int i=0;i<N;i++) {
//         out[i] = exp(x[i] - m); // needs -lm if used
//         sum += out[i];
//     }
//     if (sum == 0.0) { for (int i=0;i<N;i++) out[i] = 0.0; return; }
//     for (int i=0;i<N;i++) out[i] /= sum;
// }

// Integer-only softmax approximation (no libm inside).
// Inputs:
//   v[]    : integer quantized logits (signed), length N
//   N      : length
//   M      : number of bits used for v (e.g., 8 for int8). Must be <= 30 in this simple baseline.
//   S      : scale (real) such that real_value ≈ S * v
// Output:
//   out8[] : output probabilities 0..255 (uint8_t)
int int_softmax_baseline(const int32_t *v, int N, int M, double S, uint8_t *out8) {
    if (N <= 0) return -1;
    if (M <= 0 || M > 30) return -2; // safety: (1 << (2*M)) should fit in __int128

    // 1) find max(v)
    int32_t vmax = v[0];
    for (int i=1;i<N;i++) if (v[i] > vmax) vmax = v[i];

    // 2) vstable = v - vmax  (all <= 0)
    int32_t *vstable = (int32_t*)malloc(sizeof(int32_t)*N);
    if (!vstable) return -3;
    for (int i=0;i<N;i++) vstable[i] = v[i] - vmax;

    // 3) compute integer constants WITHOUT floor()/log() calls:
    //    vln2 = floor( ln(2) / S )  --> replaced by cast (positive)
    int64_t vln2 = (int64_t)(LN2 / S);   // casting positive double -> trunc == floor
    if (vln2 <= 0) vln2 = 1; // safety

    // Barrett mu = floor( 2^(2*M) / vln2 )
    __int128 base = (__int128)1 << (2*M);         // 2^(2M)
    __int128 mu128 = base / (__int128)vln2;       // integer division => floor
    if (mu128 <= 0) mu128 = 1;
    long long mu = (long long)mu128;

    // vb = floor( b / S )  and  vc = floor( c/(a*S^2) )
    // replaced by casts (positive values)
    int64_t vb = (int64_t)(B_coef / S);
    double denom = A_coef * S * S;
    if (denom <= 0.0) denom = 1e-12;
    int64_t vc = (int64_t)(C_coef / denom);

    // vapprox array (wide integer intermediate)
    int64_t *vapprox = (int64_t*)malloc(sizeof(int64_t)*N);
    if (!vapprox) { free(vstable); return -4; }

    // 5) compute Barrett remainder and polynomial (pure integer ops)
    for (int i=0;i<N;i++) {
        int32_t vs = vstable[i]; // <= 0

        // q = floor( vs * mu / 2^(2M) )
        __int128 prod = (__int128)vs * (__int128)mu;
        __int128 denomShift = (__int128)1 << (2*M);
        int64_t q = (int64_t)(prod / denomShift); // integer division -> floor for positive denom

        // vcorr = vs - q * vln2
        int64_t vcorr = (int64_t)vs - (int64_t)q * (int64_t)vln2;

        // polynomial: (vcorr + vb)^2 + vc
        __int128 t = (__int128)vcorr + (__int128)vb;
        __int128 t2 = t * t;
        __int128 numer = t2 + (__int128)vc;

        // shift by q (multiply by 2^{-q})
        int64_t q_shift = (q < 0) ? 0 : q;
        __int128 approx_shifted;
        if (q_shift >= 127) {
            approx_shifted = 0;
        } else {
            approx_shifted = numer >> q_shift;
        }

        if (approx_shifted < 0) approx_shifted = 0;
        vapprox[i] = (int64_t)approx_shifted;
    }

    // 6) sum and normalize to 0..255 (pure integer)
    __int128 sum = 0;
    for (int i=0;i<N;i++) sum += (__int128)vapprox[i];

    if (sum == 0) {
        int idx_max = 0;
        for (int i=1;i<N;i++) if (v[i] > v[idx_max]) idx_max = i;
        for (int i=0;i<N;i++) out8[i] = 0;
        out8[idx_max] = 255;
    } else {
        for (int i=0;i<N;i++) {
            __int128 num = (__int128)vapprox[i] * 255 + (sum/2); // rounding
            int64_t p = (int64_t)(num / sum);
            if (p < 0) p = 0;
            if (p > 255) p = 255;
            out8[i] = (uint8_t)p;
        }
    }

    free(vstable);
    free(vapprox);
    return 0;
}

// ----------- small demo / test --------------
int main(void) {
    int N = 6;
    int32_t v[6] = { 10, 6, -4, 0, 3, 9 }; // quantized logits
    int M = 8;
    double S = 0.02;

    uint8_t out8[6];
    int rc = int_softmax_baseline(v, N, M, S, out8);
    if (rc != 0) { printf("error %d\n", rc); return 1; }

    // printf("Integer softmax (0..255):\n");
    // for (int i=0;i<N;i++) printf("%3u ", out8[i]);
    // printf("\n");

    // // Optional: compare with float softmax (needs -lm). Remove this block if you want zero libm dependency.
    // double xf[6];
    // for (int i=0;i<N;i++) xf[i] = S * (double)v[i];
    // double yf[6];
    // float_softmax(xf, N, yf);
    // printf("Float softmax (probabilities):\n");
    // for (int i=0;i<N;i++) printf("%.6f ", yf[i]);
    // printf("\n");

    // printf("Integer softmax (float approx):\n");
    // for (int i=0;i<N;i++) printf("%.6f ", (double)out8[i] / 255.0);
    // printf("\n");

    return 0;
}
