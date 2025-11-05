// softmax_int_mac_ilp.c — Integer-only softmax (Barrett + deg-2 poly) with MAC ILP
// This version ensures that the custom MAC instructions are emitted and cannot
// be optimized away.  It exposes instruction-level parallelism via the LANES
// parameter and accumulates a hash of the MAC outputs to force the compiler
// to keep the inline assembly.  The kernel is marked noinline and used so
// that it is always emitted into the generated assembly.

// Build (example):
//   riscv64-linux-gnu-gcc -O3 -march=rv64gc -mabi=lp64d \
//     -DLANES=8 -fomit-frame-pointer -frename-registers \
//     benches/code/test-MAC-exp.c -o test-MAC-exp.riscv
// The inline assembly uses a custom opcode (0x5B, funct3=0x02, funct7=0x00)
// which your decoder must recognize as a pipelined multiply-accumulate (MAC).

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

// -----------------------------------------------------------------------------
// Configuration parameters
//
// LANES controls the level of ILP: how many independent accumulators are used.
// Valid values are 1, 2, 4, or 8.  Larger values improve ILP up to the number
// of MAC functional units available but increase register pressure.  You can
// override this at compile time with -DLANES=N.
#ifndef LANES
#define LANES 4
#endif

// Polynomial coefficients for the degree‑2 approximation used in softmax.
static const double A_coef = 0.3585;
static const double B_coef = 1.353;
static const double C_coef = 0.344;
static const double LN2   = 0.6931471805599453094;

// -----------------------------------------------------------------------------
// Custom MAC instruction (operand‑encoded)
//
// rd := rd + rs1 * rs2 (low 64 bits).  The assembly uses the .insn pseudo‑op
// so that the actual registers are filled in by the compiler.  It is marked
// early‑clobber ("+&r") so that GCC understands the destination register is
// overwritten before reading the inputs.
#define MACACC_R(dest, x, y) \
  __asm__(".insn r 0x5B, 0x02, 0x00, %0, %1, %2" \
          : "+&r"(dest) : "r"(x), "r"(y))

// -----------------------------------------------------------------------------
// Lane helper macros
//
// These macros declare lane variables and provide helpers to perform the MAC
// operation and accumulate the results.  They expand differently depending on
// the compile‑time value of LANES.  They are used inside the main loop of the
// softmax kernel to ensure that per‑lane data lives in registers and not on
// the stack (which would kill ILP).
#if   LANES==1
  #define VDECL(vs)  int64_t vs0
  #define VSET(k,expr) vs##k = (int64_t)(expr)
  #define TDECL(t) int64_t t0
  #define TSET(k,expr) t##k = (int64_t)(expr)
  #define ADECL(a) uint64_t a0 = 0
  #define MACLANE(k,hash) do { MACACC_R(a##k, t##k, t##k); hash ^= a##k; } while (0)
  #define STORE_VAPPROX(base,k,val) vapprox[(base)+(k)] = (int64_t)(val)
  #define REDUCE_ACC64(sum) (sum += a0)
#elif LANES==2
  #define VDECL(vs)  int64_t vs0, vs1
  #define VSET(k,expr) vs##k = (int64_t)(expr)
  #define TDECL(t) int64_t t0, t1
  #define TSET(k,expr) t##k = (int64_t)(expr)
  #define ADECL(a) uint64_t a0 = 0, a1 = 0
  #define MACLANE(k,hash) do { MACACC_R(a##k, t##k, t##k); hash ^= a##k; } while (0)
  #define STORE_VAPPROX(base,k,val) vapprox[(base)+(k)] = (int64_t)(val)
  #define REDUCE_ACC64(sum) (sum += a0 + a1)
#elif LANES==4
  #define VDECL(vs)  int64_t vs0, vs1, vs2, vs3
  #define VSET(k,expr) vs##k = (int64_t)(expr)
  #define TDECL(t) int64_t t0, t1, t2, t3
  #define TSET(k,expr) t##k = (int64_t)(expr)
  #define ADECL(a) uint64_t a0 = 0, a1 = 0, a2 = 0, a3 = 0
  #define MACLANE(k,hash) do { MACACC_R(a##k, t##k, t##k); hash ^= a##k; } while (0)
  #define STORE_VAPPROX(base,k,val) vapprox[(base)+(k)] = (int64_t)(val)
  #define REDUCE_ACC64(sum) (sum += a0 + a1 + a2 + a3)
#elif LANES==8
  #define VDECL(vs)  int64_t vs0, vs1, vs2, vs3, vs4, vs5, vs6, vs7
  #define VSET(k,expr) vs##k = (int64_t)(expr)
  #define TDECL(t) int64_t t0, t1, t2, t3, t4, t5, t6, t7
  #define TSET(k,expr) t##k = (int64_t)(expr)
  #define ADECL(a) uint64_t a0 = 0, a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0, a6 = 0, a7 = 0
  #define MACLANE(k,hash) do { MACACC_R(a##k, t##k, t##k); hash ^= a##k; } while (0)
  #define STORE_VAPPROX(base,k,val) vapprox[(base)+(k)] = (int64_t)(val)
  #define REDUCE_ACC64(sum) (sum += a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7)
#else
  #error "Invalid LANES value: choose 1, 2, 4, or 8."
#endif

// -----------------------------------------------------------------------------
// MAC‑ILP softmax kernel (non‑inline)
//
// This function approximates softmax using only integer operations.  It uses
// Barrett reduction to avoid expensive division and a degree‑2 polynomial
// approximation of exp().  The heavy computation — squaring a per‑lane value —
// is performed using the custom MAC instruction to expose ILP and stress
// multiple MAC units.  A hash of all MAC outputs is accumulated in mac_hash_out
// to prevent the compiler from eliminating the inline assembly.  The
// __attribute__((noinline, used)) ensures the function is emitted into the
// object file even if its result appears unused.
__attribute__((noinline, used))
int int_softmax_mac_ilp(const int32_t * __restrict v, int N, int M, double S,
                        uint8_t * __restrict out8,
                        uint64_t * __restrict mac_hash_out)
{
    if (N <= 0) return -1;
    if (M <= 0 || M > 30) return -2;

    // 1) Compute vmax (max of input vector)
    int32_t vmax = v[0];
    for (int i = 1; i < N; i++) if (v[i] > vmax) vmax = v[i];

    // 2) Allocate and compute vstable = v - vmax
    int32_t *vstable = (int32_t *)malloc((size_t)N * sizeof(int32_t));
    if (!vstable) return -3;
    for (int i = 0; i < N; i++) vstable[i] = v[i] - vmax;

    // 3) Compute integer constants for Barrett reduction and polynomial
    int64_t vln2 = (int64_t)(LN2 / S);
    if (vln2 <= 0) vln2 = 1;

    __int128 base = (__int128)1 << (2 * M);
    __int128 mu128 = base / (__int128)vln2;
    if (mu128 <= 0) mu128 = 1;
    int64_t mu = (int64_t)mu128;

    int64_t vb = (int64_t)(B_coef / S);
    double denom = A_coef * S * S;
    if (denom <= 0.0) denom = 1e-12;
    int64_t vc = (int64_t)(C_coef / denom);

    // 4) Allocate vapprox: stores per‑element approximate exp values (64‑bit)
    int64_t *vapprox = (int64_t *)malloc((size_t)N * sizeof(int64_t));
    if (!vapprox) {
        free(vstable);
        return -4;
    }

    // Hash of all MAC outputs to keep the inline assembly from being dead‑code
    // eliminated.  Initialized to zero at start.
    uint64_t mac_hash = 0;

    // 5) Main loop: process elements in blocks of LANES.  Each iteration
    // computes per‑lane t^2 via MAC and then scales and stores the result.
    int i = 0;
    int n_vec = (N / LANES) * LANES;

    for (; i < n_vec; i += LANES) {
        // Declare per‑lane temporaries that live in registers
        VDECL(vs);
        TDECL(t);
        ADECL(a);

        // Lane 0
        {
            int32_t vsk = vstable[i + 0];
            __int128 prod = (__int128)vsk * (__int128)mu;
            int64_t q = (int64_t)(prod >> (2 * M));
            int64_t vcorr = (int64_t)vsk - q * vln2;
            VSET(0, vsk);
            TSET(0, vcorr + vb);
            MACLANE(0, mac_hash);
            uint64_t numer = (uint64_t)a0 + (uint64_t)vc;
            int64_t q_shift = (q < 0) ? 0 : q;
            uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
            STORE_VAPPROX(i, 0, (int64_t)appr);
        }

#if LANES >= 2
        {
            int32_t vsk = vstable[i + 1];
            __int128 prod = (__int128)vsk * (__int128)mu;
            int64_t q = (int64_t)(prod >> (2 * M));
            int64_t vcorr = (int64_t)vsk - q * vln2;
            VSET(1, vsk);
            TSET(1, vcorr + vb);
            MACLANE(1, mac_hash);
            uint64_t numer = (uint64_t)a1 + (uint64_t)vc;
            int64_t q_shift = (q < 0) ? 0 : q;
            uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
            STORE_VAPPROX(i, 1, (int64_t)appr);
        }
#endif
#if LANES >= 4
        {
            int32_t vsk = vstable[i + 2];
            __int128 prod = (__int128)vsk * (__int128)mu;
            int64_t q = (int64_t)(prod >> (2 * M));
            int64_t vcorr = (int64_t)vsk - q * vln2;
            VSET(2, vsk);
            TSET(2, vcorr + vb);
            MACLANE(2, mac_hash);
            uint64_t numer = (uint64_t)a2 + (uint64_t)vc;
            int64_t q_shift = (q < 0) ? 0 : q;
            uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
            STORE_VAPPROX(i, 2, (int64_t)appr);
        }
        {
            int32_t vsk = vstable[i + 3];
            __int128 prod = (__int128)vsk * (__int128)mu;
            int64_t q = (int64_t)(prod >> (2 * M));
            int64_t vcorr = (int64_t)vsk - q * vln2;
            VSET(3, vsk);
            TSET(3, vcorr + vb);
            MACLANE(3, mac_hash);
            uint64_t numer = (uint64_t)a3 + (uint64_t)vc;
            int64_t q_shift = (q < 0) ? 0 : q;
            uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
            STORE_VAPPROX(i, 3, (int64_t)appr);
        }
#endif
#if LANES >= 8
        {
            int32_t vsk = vstable[i + 4];
            __int128 prod = (__int128)vsk * (__int128)mu;
            int64_t q = (int64_t)(prod >> (2 * M));
            int64_t vcorr = (int64_t)vsk - q * vln2;
            VSET(4, vsk);
            TSET(4, vcorr + vb);
            MACLANE(4, mac_hash);
            uint64_t numer = (uint64_t)a4 + (uint64_t)vc;
            int64_t q_shift = (q < 0) ? 0 : q;
            uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
            STORE_VAPPROX(i, 4, (int64_t)appr);
        }
        {
            int32_t vsk = vstable[i + 5];
            __int128 prod = (__int128)vsk * (__int128)mu;
            int64_t q = (int64_t)(prod >> (2 * M));
            int64_t vcorr = (int64_t)vsk - q * vln2;
            VSET(5, vsk);
            TSET(5, vcorr + vb);
            MACLANE(5, mac_hash);
            uint64_t numer = (uint64_t)a5 + (uint64_t)vc;
            int64_t q_shift = (q < 0) ? 0 : q;
            uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
            STORE_VAPPROX(i, 5, (int64_t)appr);
        }
        {
            int32_t vsk = vstable[i + 6];
            __int128 prod = (__int128)vsk * (__int128)mu;
            int64_t q = (int64_t)(prod >> (2 * M));
            int64_t vcorr = (int64_t)vsk - q * vln2;
            VSET(6, vsk);
            TSET(6, vcorr + vb);
            MACLANE(6, mac_hash);
            uint64_t numer = (uint64_t)a6 + (uint64_t)vc;
            int64_t q_shift = (q < 0) ? 0 : q;
            uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
            STORE_VAPPROX(i, 6, (int64_t)appr);
        }
        {
            int32_t vsk = vstable[i + 7];
            __int128 prod = (__int128)vsk * (__int128)mu;
            int64_t q = (int64_t)(prod >> (2 * M));
            int64_t vcorr = (int64_t)vsk - q * vln2;
            VSET(7, vsk);
            TSET(7, vcorr + vb);
            MACLANE(7, mac_hash);
            uint64_t numer = (uint64_t)a7 + (uint64_t)vc;
            int64_t q_shift = (q < 0) ? 0 : q;
            uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
            STORE_VAPPROX(i, 7, (int64_t)appr);
        }
#endif
    }

    // Tail loop: process any remaining elements one by one
    for (; i < N; ++i) {
        int32_t vs = vstable[i];
        __int128 prod = (__int128)vs * (__int128)mu;
        int64_t q = (int64_t)(prod >> (2 * M));
        int64_t vcorr = (int64_t)vs - q * vln2;
        int64_t t = vcorr + vb;
        uint64_t acc = 0;
        MACACC_R(acc, t, t);
        mac_hash ^= acc;               // incorporate tail MAC into hash
        uint64_t numer = acc + (uint64_t)vc;
        int64_t q_shift = (q < 0) ? 0 : q;
        uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
        vapprox[i] = (int64_t)appr;
    }

    // 6) Sum and normalize to 0..255 (exact 128‑bit accumulation)
    __int128 sum = 0;
    for (int j = 0; j < N; j++) sum += (__int128)vapprox[j];

    if (sum == 0) {
        int idx_max = 0;
        for (int j = 1; j < N; j++) if (v[j] > v[idx_max]) idx_max = j;
        for (int j = 0; j < N; j++) out8[j] = 0;
        out8[idx_max] = 255;
    } else {
        for (int j = 0; j < N; j++) {
            __int128 num = (__int128)vapprox[j] * 255 + (sum >> 1);
            int64_t p = (int64_t)(num / sum);
            if (p < 0) p = 0;
            if (p > 255) p = 255;
            out8[j] = (uint8_t)p;
        }
    }

    // Write mac_hash to output if requested
    if (mac_hash_out) *mac_hash_out = mac_hash;

    free(vstable);
    free(vapprox);
    return 0;
}

// -----------------------------------------------------------------------------
// Demo main
//
// This main function exercises the MAC‑ILP softmax.  It allocates input
// vectors, calls the kernel, and prints out a checksum of the output and the
// MAC hash.  This ensures that the kernel’s inline assembly is executed and
// cannot be optimized away.
int main(void) {
    int N = 48;
    int32_t v[48] = { 10, 6, -4, 0, 3, 9, 1, -3, 7, -2, 5, 4, -1, 2, 8, 11, 10, 6, -4, 0, 3, 9, 1, -3, 7, -2, 5, 4, -1, 2, 8, 11 , 10, 6, -4, 0, 3, 9, 1, -3, 7, -2, 5, 4, -1, 2, 8, 11 };
    int M = 8;
    double S = 0.02;

    uint8_t out8[48];
    uint64_t mac_hash = 0;
    int rc = int_softmax_mac_ilp(v, N, M, S, out8, &mac_hash);
    if (rc != 0) {
        printf("error %d\n", rc);
        return 1;
    }

    // Compute a simple checksum of the output to make the results observable
    uint64_t out_sum = 0;
    for (int i = 0; i < N; i++) out_sum += out8[i];

    printf("N=%d LANES=%d mac_hash=%llu out_sum=%llu\n",
           N, LANES, (unsigned long long)mac_hash, (unsigned long long)out_sum);
    return 0;
}