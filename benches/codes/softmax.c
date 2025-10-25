// softmax.c
#include <math.h>
#include <stdio.h>

#define N 4

int main(void) {
    double x[N] = {1.0, 2.0, 3.0, 4.0};
    double y[N];
    double sum = 0.0;

    // exponentiate and sum
    for (int i = 0; i < N; i++) {
        y[i] = exp(x[i]);
        sum += y[i];
    }

    // normalize
    for (int i = 0; i < N; i++) {
        y[i] /= sum;
        printf("%.6f\n", y[i]);
    }
    return 0;
}

