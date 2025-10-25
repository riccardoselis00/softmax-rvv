static volatile float sink;

static __attribute__((noinline)) float macf_s(float a, float b, float c) {
    // Pin inputs to the exact FP regs expected by the encoded word
    register float rs3 asm("fa3") = c;   // rs3
    register float rs2 asm("fa4") = b;   // rs2
    register float rd1 asm("fa5") = a;   // rs1 and also rd

    // Emit the pre-encoded instruction word
    asm volatile (".word 0x68E7F7DB"
                  : "+f"(rd1)       // rd is written by the instruction
                  : "f"(rs2), "f"(rs3)
                  : "memory");

    return rd1; // result came back in fa5
}


int main() {
    float a=2.0f, b=3.0f, c=4.0f;
    float d = macf_s(a,b,c);
    sink = d;                 // make the result visible to the program
    return 0;
}
