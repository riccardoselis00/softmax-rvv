#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
//#include "gem5/m5ops.h"   // adjust include path at compile time

static void softmax_scalar(float* __restrict y,
                           const float* __restrict x,
                           size_t n)
{
    // 1) reduce-max for stability
    float m = -INFINITY;
    for (size_t i = 0; i < n; ++i) if (x[i] > m) m = x[i];

    // 2) exp(x-m) and reduce-sum
    double s = 0.0;
    for (size_t i = 0; i < n; ++i) {
        float u = expf(x[i] - m);
        y[i] = u;           // store unnormalized for now
        s += u;
    }

    // 3) normalize
    float invs = (float)(1.0 / s);
    for (size_t i = 0; i < n; ++i) y[i] *= invs;
}

int main(int argc, char** argv)
{
    size_t n = (argc > 1) ? strtoull(argv[1], NULL, 10) : 1024;
    float* x = (float*)malloc(n * sizeof(float));
    float* y = (float*)malloc(n * sizeof(float));
    if (!x || !y) return 1;

    // deterministic input
    for (size_t i = 0; i < n; ++i) x[i] = (float)((i * 1315423911u) % 1000) / 37.0f;

   // m5_reset_stats(0,0);
    softmax_scalar(y, x, n);
   // m5_dump_stats(0,0);

    // touch output so nothing is optimized away
    printf("scalar y[0]=%g\n", (double)y[0]);

    free(x); free(y);
  //  m5_exit(0);
    return 0;
}

