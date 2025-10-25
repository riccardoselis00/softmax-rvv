# Custom Vector Instructions for Softmax on RISC-V in gem5

## Delving Into the Project

Here is the project's organization overview:

```bash
├── analysis.ipynb
├── benches
│   ├── assembly
│   ├── codes
│   └── executables
├── configs
│   └── sum.py
├── doc
├── extern
│   └── Tiny-Vedas
├── m5out
├── readme.md
├── results
│   ├── results_OpLat-3_pipeline-false_count-1
│   ├── results_OpLat-4_pipeline-false_count-1
│   └── results_OpLat-4_pipeline-true_count-1
└── tools
    ├── parse_stats.py
    ├── plot_stats.py
    └── __pycache__
```

## Creating the gem5 executable `gem5.opt`
```bash
scons build/RISCV/gem5.opt -j22
```


## Creating the c code executable 

### from  .c $\to$ .s

```bash
riscv64-linux-gnu-gcc -O0 -march=rv64gc -mabi=lp64d -S benches/codes/code.c -o benches/assembly/assembly.s
```

### from .s $\to$ ELF

```bash
riscv64-linux-gnu-gcc -static -march=rv64gc -mabi=lp64d benches/assembly/assembly.s -o benches/executables/exe
```


## Run the simulation with gem5


```bash
build/RISCV/gem5.opt   --outdir=../softmax-rvv/results/res-folder   configs/deprecated/example/se.py   --cpu-type=O3CPU --caches --l2cache   -c ../softmax-rvv/benches/executables/exe
```
------------------

## Aggregate Data

### Scalar Baseline Softmax Version

```bash
python3 tools/parse_stats.py results/softmax_scalar/N_64000/stats_N64000.txt results/softmax_scalar/N_16000/stats_N16000.txt results/softmax_scalar/N_32000/stats_N32000.txt results/softmax_scalar/N_128000/stats_N128000.txt -o results/softmax_scalar/sofmax_scalar_stats.csv
```
## Analize a metric from different Versions

```bash
python3 tools/plot_stats.py   board.cache_hierarchy.membus.respLayer1.utilization   results/softmax_scalar/sofmax_scalar_stats.csv   results/softmax_vect_baseline/sofmax_scalar_stats.csv   results/softmax_vect_enhanced/sofmax_scalar_stats.csv
```




