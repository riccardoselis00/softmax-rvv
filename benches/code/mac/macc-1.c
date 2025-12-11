#include <stdint.h>
#include <stdio.h>

volatile uint64_t results[4];

#define MACACC_R(dest, x, y) \
  __asm__ (".insn r 0x5B, 0x02, 0x00, %0, %1, %2" \
           : "+&r"(dest) : "r"(x), "r"(y))

int main(void)
{
    uint64_t acc = 0x0001020304050607ULL;
    uint64_t a   = 0x0102030405060708ULL;
    uint64_t b   = 0x1111111111111111ULL;

    results[0] = acc;
    results[1] = a;
    results[2] = b;

    // Execute your custom MACACC instruction once
    MACACC_R(acc, a, b);

    results[3] = acc;

    // Print for debugging (optional)
    printf("Initial acc = 0x%016llx\n", (unsigned long long)results[0]);
    printf("a          = 0x%016llx\n", (unsigned long long)results[1]);
    printf("b          = 0x%016llx\n", (unsigned long long)results[2]);
    printf("Final acc  = 0x%016llx\n", (unsigned long long)results[3]);

    return 0;
}
