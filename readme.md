# Custom Vector Instructions for Softmax on RISC-V in gem5

## Delving Into the Project

## Compile into RISC-V executable

```bash
cd ~/Desktop/softmax-rvv
riscv64-linux-gnu-gcc -O3 -static -s -o benches/exe benches/code.c -lm
```
## Run the simulation with gem5

```bash
cd ~/Desktop/softmax-rvv
../gem5/build/ALL/gem5.opt   --outdir results/res-folder   configs/script.py --bin $(pwd)/benches/exe
```
