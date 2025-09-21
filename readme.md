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
N=dim sh -c '../gem5/build/ALL/gem5.opt --outdir results/softmax_version/-N_${N} --stats-file stats_N${N}.txt configs/script.py --bin "$(pwd)/benches/exe" --n ${N}'
```
## Aggregate Data

### Scalar Baseline Softmax Version

```bash
python3 tools/parse_stats.py results/softmax_scalar/N_64000/stats_N64000.txt results/softmax_scalar/N_16000/stats_N16000.txt results/softmax_scalar/N_32000/stats_N32000.txt results/softmax_scalar/N_128000/stats_N128000.txt -o results/softmax_scalar/sofmax_scalar_stats.csv
```
## Analize a metric from different Versions

```bash
python3 tools/plot_stats.py   board.cache_hierarchy.membus.respLayer1.utilization   results/softmax_scalar/sofmax_scalar_stats.csv   results/softmax_vect_baseline/sofmax_scalar_stats.csv   results/softmax_vect_enhanced/sofmax_scalar_stats.csv
```



