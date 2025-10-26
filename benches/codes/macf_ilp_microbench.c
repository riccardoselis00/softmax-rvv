// macf_ilp_word.c — ILP microbench using `.word` with K independent accumulators
#include <stdint.h>
#include <stdlib.h>

static volatile float sink;

// RISC-V R4-type encode (matches your 0x68E7F7DB pattern):
// word = rs3<<27 | funct2<<25 | rs2<<20 | rs1<<15 | rm<<12 | rd<<7 | opcode
#define MACF_ENC(rd, rs1, rs2, rs3) \
  ( ((uint32_t)(rs3) << 27) | (0u << 25) | ((uint32_t)(rs2) << 20) | \
    ((uint32_t)(rs1) << 15) | (7u << 12) | ((uint32_t)(rd) << 7) | 0x5B )

// Pre-encoded words for rd=rs1 in f15..f22, rs2=f14, rs3=f13:
#define W_F15 0x68E7F7DBu  // rd=rs1=f15 (fa5),  rs2=f14 (fa4), rs3=f13 (fa3)
#define W_F16 0x68E8785Bu  // rd=rs1=f16 (fa6)
#define W_F17 0x68E8F8DBu  // rd=rs1=f17 (fa7)
#define W_F18 0x68E9795Bu  // rd=rs1=f18 (fs2)
#define W_F19 0x68E9F9DBu  // rd=rs1=f19 (fs3)
#define W_F20 0x68EA7A5Bu  // rd=rs1=f20 (fs4)
#define W_F21 0x68EAFADBu  // rd=rs1=f21 (fs5)
#define W_F22 0x68EB7B5Bu  // rd=rs1=f22 (fs6)

// One MAC on a specific accumulator register (outputs mark the modified reg)
#define DO_MAC_F15(acc, breg, creg) asm volatile(".word "  "0x68E7F7DB" : "+f"(acc) : "f"(breg), "f"(creg) : "memory")
#define DO_MAC_F16(acc, breg, creg) asm volatile(".word "  "0x68E8785B" : "+f"(acc) : "f"(breg), "f"(creg) : "memory")
#define DO_MAC_F17(acc, breg, creg) asm volatile(".word "  "0x68E8F8DB" : "+f"(acc) : "f"(breg), "f"(creg) : "memory")
#define DO_MAC_F18(acc, breg, creg) asm volatile(".word "  "0x68E9795B" : "+f"(acc) : "f"(breg), "f"(creg) : "memory")
#define DO_MAC_F19(acc, breg, creg) asm volatile(".word "  "0x68E9F9DB" : "+f"(acc) : "f"(breg), "f"(creg) : "memory")
#define DO_MAC_F20(acc, breg, creg) asm volatile(".word "  "0x68EA7A5B" : "+f"(acc) : "f"(breg), "f"(creg) : "memory")
#define DO_MAC_F21(acc, breg, creg) asm volatile(".word "  "0x68EAFADB" : "+f"(acc) : "f"(breg), "f"(creg) : "memory")
#define DO_MAC_F22(acc, breg, creg) asm volatile(".word "  "0x68EB7B5B" : "+f"(acc) : "f"(breg), "f"(creg) : "memory")

int main(int argc, char** argv) {
    // Total MACs across all accumulators:
    uint64_t N = (argc > 1) ? strtoull(argv[1], 0, 10) : 8000000ULL;
    int K = (argc > 2) ? atoi(argv[2]) : 8;   // number of parallel accumulators (1..8)
    if (K < 1) K = 1; if (K > 8) K = 8;

    // Shared operands in fixed FP regs matching the encodings:
    const float a = 1.001f, b = 0.999f; // we only need b (rs2) and c (rs3) here; a is implicit via rs1=rd
    register float creg asm("f13") = b; // we treat "c" as +b to keep it simple → rs3=f13
    register float breg asm("f14") = a; // and "b" as multiplicand → rs2=f14
    // (So MAC computes: rd = rd * rs2 + rs3 ≡ acc = acc * a + b)

    // K independent accumulators pinned to f15..f22
    register float acc0 asm("f15") = 0.0f;
    register float acc1 asm("f16") = 1.0f;
    register float acc2 asm("f17") = 2.0f;
    register float acc3 asm("f18") = 3.0f;
    register float acc4 asm("f19") = 4.0f;
    register float acc5 asm("f20") = 5.0f;
    register float acc6 asm("f21") = 6.0f;
    register float acc7 asm("f22") = 7.0f;

    // Each loop issues up to K independent MACs → exposes FU throughput and `count`
    uint64_t iters = N / (uint64_t)K;
    for (uint64_t i = 0; i < iters; ++i) {
        switch (K) {
            case 8: DO_MAC_F22(acc7, breg, creg);
            case 7: DO_MAC_F21(acc6, breg, creg);
            case 6: DO_MAC_F20(acc5, breg, creg);
            case 5: DO_MAC_F19(acc4, breg, creg);
            case 4: DO_MAC_F18(acc3, breg, creg);
            case 3: DO_MAC_F17(acc2, breg, creg);
            case 2: DO_MAC_F16(acc1, breg, creg);
            case 1: DO_MAC_F15(acc0, breg, creg);
        }
        // (Intentional fallthrough to emit K MACs per iteration.)
    }

    // Keep results live
    float sum = 0.0f;
    if (K >= 1) sum += acc0;
    if (K >= 2) sum += acc1;
    if (K >= 3) sum += acc2;
    if (K >= 4) sum += acc3;
    if (K >= 5) sum += acc4;
    if (K >= 6) sum += acc5;
    if (K >= 7) sum += acc6;
    if (K >= 8) sum += acc7;
    sink = sum;

    return 0;
}
