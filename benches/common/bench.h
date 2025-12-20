#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Read cycle counter on RISC-V, otherwise returns 0.
uint64_t bench_rdcycle(void);

// Wall-clock seconds (portable fallback).
double bench_seconds(void);

// Prevent dead-code elimination (use to keep results "live").
void bench_sink_u64(uint64_t x);

// Simple deterministic RNG to avoid repeating rand() boilerplate.
void bench_rng_seed(uint64_t seed);
uint64_t bench_rand_u64(void);

#ifdef __cplusplus
}
#endif
