// macf_depchain_word.c — minimal MACFp latency microbench using `.word`
#include <stdint.h>
#include <stdlib.h>

static volatile float sink;

static __attribute__((noinline)) float macf_s_word(float a, float b, float c) {
    // Pin operands to the exact FP registers your encoding expects:
    //   rd=rs1 -> fa5,   rs2 -> fa4,   rs3 -> fa3
    register float rs3 asm("fa3") = c;
    register float rs2 asm("fa4") = b;
    register float rd  asm("fa5") = a;  // also acts as rs1 and the destination

    // Emit the pre-encoded instruction word
    asm volatile (".word 0x68E7F7DB"
                  : "+f"(rd)          // rd is written by the instruction
                  : "f"(rs2), "f"(rs3)
                  : "memory");
    return rd;
}

int main(int argc, char **argv) {
    // N MACs in strict dependency → runtime ≈ N * opLat (+ small overhead)
    uint64_t N = (argc > 1) ? strtoull(argv[1], 0, 10) : 10000000ULL;

    float acc = 0.0f;
    const float a = 1.001f, b = 0.999f;

    for (uint64_t i = 0; i < N; ++i) {
        // acc = acc * a + b  (via your custom MAC)
        acc = macf_s_word(acc, a, b);
    }

    sink = acc; // keep it live
    return 0;
}
