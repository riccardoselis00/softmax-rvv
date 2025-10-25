static volatile float sink;

static inline __attribute__((noinline)) float macf_s(float a, float b, float c) {
    float d;
    asm volatile(".insn r4 0x5b, 0x0, %0, %1, %2, %3, 7"
                 : "=f"(d)
                 : "f"(a), "f"(b), "f"(c)
                 : "memory");
    return d;
}

int main() {
    float a=2.0f, b=3.0f, c=4.0f;
    float d = macf_s(a,b,c);
    sink = d;                 // make the result visible to the program
    return 0;
}
