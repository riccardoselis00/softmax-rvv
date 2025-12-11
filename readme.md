1. Generate the risc-v executable:
```bash
cd ~/Desktop/softmax-
riscv64-linux-gnu-gcc -O2 -static -march=rv64gc ./benches/code/mac/poly-horner-2.c -o ./benches/executable/poly
-horner-2
```
2. Run the gem5 simulation with the generated executable:
```bash
build/RISCV/gem5.opt --outdir=../softmax-rvv/results/poly-horner/poly-1/.   configs/deprecated/example/se.py   --cpu-type=RiscvMinorCPU   --caches --l2cache   -c ../softmax-rvv/benches/executable/poly-horner-2
```
