// softmax_int_mac_ilp.c — Integer-only softmax (Barrett + deg-2 poly) with MAC ILP
// Build (example):
//   riscv64-linux-gnu-gcc -O3 -march=rv64gc -mabi=lp64d \
//     -DLANES=8 -fomit-frame-pointer -frename-registers \
//     softmax_int_mac_ilp.c -o softmax_int_mac_ilp.riscv
// Run (gem5):
//   build/RISCV/gem5.opt configs/deprecated/example/se.py --cpu-type=O3CPU --caches --l2cache \
//     -c ./softmax_int_mac_ilp.riscv

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>   // only for demo main; core kernel is integer

#ifndef LANES
#define LANES 4          // Choose 1,2,4,8 (watch register pressure)
#endif

// ------------------- polynomial coefs & ln2 -------------------
static const double A_coef = 0.3585;   // a
static const double B_coef = 1.353;    // b
static const double C_coef = 0.344;    // c
static const double LN2   = 0.6931471805599453094;

// ------------------- custom MAC (operand-encoded) -------------------
// rd := rd + rs1*rs2  (low 64-bit), mapped by your decoder to a pipelined MAC FU
#define MACACC_R(dest, x, y) \
  __asm__ (".insn r 0x5B, 0x02, 0x00, %0, %1, %2\n" \
           : "+&r"(dest) : "r"(x), "r"(y))

// ---- helpers to scalarize lanes into true registers (no stack arrays) ----
#if   LANES==1
  #define LDECL(x) uint64_t x##0
  #define LLOAD(base,off,arr) x##off = (uint64_t)(arr[(base)+(off)])
  #define YDECL(y) uint64_t y##0
  #define YLOAD(base,off,arr) y##off = (uint64_t)(arr[(base)+(off)])
  #define VDECL(vs)  int64_t vs##0
  #define VSET(k,expr) vs##k = (int64_t)(expr)
  #define TDECL(t) int64_t t##0
  #define TSET(k,expr) t##k = (int64_t)(expr)
  #define ADECL(a) uint64_t a##0=0
  #define MACLANE(k) MACACC_R(a##k, t##k, t##k)
  #define STORE_VAPPROX(base,k,val) vapprox[(base)+(k)] = (int64_t)(val)
  #define REDUCE_ACC64(sum) (sum += a0)
#elif LANES==2
  #define LDECL(x) uint64_t x##0,x##1
  #define LLOAD(base,off,arr) x##off = (uint64_t)(arr[(base)+(off)])
  #define YDECL(y) uint64_t y##0,y##1
  #define YLOAD(base,off,arr) y##off = (uint64_t)(arr[(base)+(off)])
  #define VDECL(vs)  int64_t vs##0,vs##1
  #define VSET(k,expr) vs##k = (int64_t)(expr)
  #define TDECL(t) int64_t t##0,t##1
  #define TSET(k,expr) t##k = (int64_t)(expr)
  #define ADECL(a) uint64_t a##0=0,a##1=0
  #define MACLANE(k) MACACC_R(a##k, t##k, t##k)
  #define STORE_VAPPROX(base,k,val) vapprox[(base)+(k)] = (int64_t)(val)
  #define REDUCE_ACC64(sum) (sum += a0 + a1)
#elif LANES==4
  #define LDECL(x) uint64_t x##0,x##1,x##2,x##3
  #define LLOAD(base,off,arr) x##off = (uint64_t)(arr[(base)+(off)])
  #define YDECL(y) uint64_t y##0,y##1,y##2,y##3
  #define YLOAD(base,off,arr) y##off = (uint64_t)(arr[(base)+(off)])
  #define VDECL(vs)  int64_t vs##0,vs##1,vs##2,vs##3
  #define VSET(k,expr) vs##k = (int64_t)(expr)
  #define TDECL(t) int64_t t##0,t##1,t##2,t##3
  #define TSET(k,expr) t##k = (int64_t)(expr)
  #define ADECL(a) uint64_t a##0=0,a##1=0,a##2=0,a##3=0
  #define MACLANE(k) MACACC_R(a##k, t##k, t##k)
  #define STORE_VAPPROX(base,k,val) vapprox[(base)+(k)] = (int64_t)(val)
  #define REDUCE_ACC64(sum) (sum += a0 + a1 + a2 + a3)
#elif LANES==8
  #define LDECL(x) uint64_t x##0,x##1,x##2,x##3,x##4,x##5,x##6,x##7
  #define LLOAD(base,off,arr) x##off = (uint64_t)(arr[(base)+(off)])
  #define YDECL(y) uint64_t y##0,y##1,y##2,y##3,y##4,y##5,y##6,y##7
  #define YLOAD(base,off,arr) y##off = (uint64_t)(arr[(base)+(off)])
  #define VDECL(vs)  int64_t vs##0,vs##1,vs##2,vs##3,vs##4,vs##5,vs##6,vs##7
  #define VSET(k,expr) vs##k = (int64_t)(expr)
  #define TDECL(t) int64_t t##0,t##1,t##2,t##3,t##4,t##5,t##6,t##7
  #define TSET(k,expr) t##k = (int64_t)(expr)
  #define ADECL(a) uint64_t a##0=0,a##1=0,a##2=0,a##3=0,a##4=0,a##5=0,a##6=0,a##7=0
  #define MACLANE(k) MACACC_R(a##k, t##k, t##k)
  #define STORE_VAPPROX(base,k,val) vapprox[(base)+(k)] = (int64_t)(val)
  #define REDUCE_ACC64(sum) (sum += a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7)
#else
  #error "Set LANES to 1,2,4,8"
#endif

// ------------------- MAC-ILP softmax kernel -------------------
static inline int int_softmax_mac_ilp(const int32_t * __restrict v,
                                      int N, int M, double S,
                                      uint8_t * __restrict out8)
{
    if (N <= 0) return -1;
    if (M <= 0 || M > 30) return -2;

    // 1) vmax
    int32_t vmax = v[0];
    for (int i=1;i<N;i++) if (v[i] > vmax) vmax = v[i];

    // 2) vstable
    int32_t *vstable = (int32_t*)malloc((size_t)N*sizeof(int32_t));
    if (!vstable) return -3;
    for (int i=0;i<N;i++) vstable[i] = v[i] - vmax;

    // 3) integer constants
    int64_t vln2 = (int64_t)(LN2 / S);
    if (vln2 <= 0) vln2 = 1;

    __int128 base = (__int128)1 << (2*M);
    __int128 mu128 = base / (__int128)vln2;
    if (mu128 <= 0) mu128 = 1;
    long long mu = (long long)mu128;

    int64_t vb = (int64_t)(B_coef / S);
    double denom = A_coef * S * S; if (denom <= 0.0) denom = 1e-12;
    int64_t vc = (int64_t)(C_coef / denom);

    // 4) vapprox output (64-bit per element; we still sum in 128-bit)
    int64_t *vapprox = (int64_t*)malloc((size_t)N*sizeof(int64_t));
    if (!vapprox) { free(vstable); return -4; }

    // 5) main loop (ILP = LANES), compute:
    //    vs -> q (Barrett) -> vcorr -> t = vcorr + vb
    //    t2 via MAC: acc := acc + t*t (low 64-bit)   // compute-bound section
    //    numer = t2 + vc (wrap 64-bit) -> shift by q -> clamp
    int i = 0;
    int n_vec = (N / LANES) * LANES;

    for (; i < n_vec; i += LANES) {
        // Pre-declare per-lane temporaries (stay in regs)
        VDECL(vs);
        TDECL(t);
        ADECL(a); // lane accumulators for t^2 (low-64)
        // (We don't actually need x/y pairs here; keep macro symmetry harmless.)

        // Load & compute per lane
        // (1) vs = vstable[i+k]
        // (2) q = floor(vs*mu / 2^(2M))
        // (3) vcorr = vs - q*vln2
        // (4) t = vcorr + vb
        // (5) MAC for t^2: a_k += t*t

        // lane 0
        {
            int32_t vsk = vstable[i+0];
            __int128 prod = (__int128)vsk * (__int128)mu;
            int64_t q = (int64_t)(prod >> (2*M));
            int64_t vcorr = (int64_t)vsk - q * vln2;
            VSET(0, vsk);
            TSET(0, vcorr + vb);
            MACLANE(0);
            // numer & shift
            uint64_t numer = (uint64_t)a0 + (uint64_t)vc;
            int64_t q_shift = (q < 0) ? 0 : q;
            uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
            STORE_VAPPROX(i,0,(int64_t)appr);
        }

#if LANES>=2
        {
            int32_t vsk = vstable[i+1];
            __int128 prod = (__int128)vsk * (__int128)mu;
            int64_t q = (int64_t)(prod >> (2*M));
            int64_t vcorr = (int64_t)vsk - q * vln2;
            VSET(1, vsk);
            TSET(1, vcorr + vb);
            MACLANE(1);
            uint64_t numer = (uint64_t)a1 + (uint64_t)vc;
            int64_t q_shift = (q < 0) ? 0 : q;
            uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
            STORE_VAPPROX(i,1,(int64_t)appr);
        }
#endif
#if LANES>=4
        {
            int32_t vsk = vstable[i+2];
            __int128 prod = (__int128)vsk * (__int128)mu;
            int64_t q = (int64_t)(prod >> (2*M));
            int64_t vcorr = (int64_t)vsk - q * vln2;
            VSET(2, vsk);
            TSET(2, vcorr + vb);
            MACLANE(2);
            uint64_t numer = (uint64_t)a2 + (uint64_t)vc;
            int64_t q_shift = (q < 0) ? 0 : q;
            uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
            STORE_VAPPROX(i,2,(int64_t)appr);
        }
        {
            int32_t vsk = vstable[i+3];
            __int128 prod = (__int128)vsk * (__int128)mu;
            int64_t q = (int64_t)(prod >> (2*M));
            int64_t vcorr = (int64_t)vsk - q * vln2;
            VSET(3, vsk);
            TSET(3, vcorr + vb);
            MACLANE(3);
            uint64_t numer = (uint64_t)a3 + (uint64_t)vc;
            int64_t q_shift = (q < 0) ? 0 : q;
            uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
            STORE_VAPPROX(i,3,(int64_t)appr);
        }
#endif
#if LANES>=8
        {
            int32_t vsk = vstable[i+4];
            __int128 prod = (__int128)vsk * (__int128)mu;
            int64_t q = (int64_t)(prod >> (2*M));
            int64_t vcorr = (int64_t)vsk - q * vln2;
            VSET(4, vsk);
            TSET(4, vcorr + vb);
            MACLANE(4);
            uint64_t numer = (uint64_t)a4 + (uint64_t)vc;
            int64_t q_shift = (q < 0) ? 0 : q;
            uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
            STORE_VAPPROX(i,4,(int64_t)appr);
        }
        {
            int32_t vsk = vstable[i+5];
            __int128 prod = (__int128)vsk * (__int128)mu;
            int64_t q = (int64_t)(prod >> (2*M));
            int64_t vcorr = (int64_t)vsk - q * vln2;
            VSET(5, vsk);
            TSET(5, vcorr + vb);
            MACLANE(5);
            uint64_t numer = (uint64_t)a5 + (uint64_t)vc;
            int64_t q_shift = (q < 0) ? 0 : q;
            uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
            STORE_VAPPROX(i,5,(int64_t)appr);
        }
        {
            int32_t vsk = vstable[i+6];
            __int128 prod = (__int128)vsk * (__int128)mu;
            int64_t q = (int64_t)(prod >> (2*M));
            int64_t vcorr = (int64_t)vsk - q * vln2;
            VSET(6, vsk);
            TSET(6, vcorr + vb);
            MACLANE(6);
            uint64_t numer = (uint64_t)a6 + (uint64_t)vc;
            int64_t q_shift = (q < 0) ? 0 : q;
            uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
            STORE_VAPPROX(i,6,(int64_t)appr);
        }
        {
            int32_t vsk = vstable[i+7];
            __int128 prod = (__int128)vsk * (__int128)mu;
            int64_t q = (int64_t)(prod >> (2*M));
            int64_t vcorr = (int64_t)vsk - q * vln2;
            VSET(7, vsk);
            TSET(7, vcorr + vb);
            MACLANE(7);
            uint64_t numer = (uint64_t)a7 + (uint64_t)vc;
            int64_t q_shift = (q < 0) ? 0 : q;
            uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
            STORE_VAPPROX(i,7,(int64_t)appr);
        }
#endif
    }

    // tail (scalar)
    for (; i < N; ++i) {
        int32_t vs = vstable[i];
        __int128 prod = (__int128)vs * (__int128)mu;
        int64_t q = (int64_t)(prod >> (2*M));
        int64_t vcorr = (int64_t)vs - q * vln2;
        int64_t t = vcorr + vb;
        uint64_t acc = 0;              // t^2 via MAC (low 64)
        MACACC_R(acc, t, t);
        uint64_t numer = acc + (uint64_t)vc;
        int64_t q_shift = (q < 0) ? 0 : q;
        uint64_t appr = (q_shift >= 63) ? 0ull : (numer >> q_shift);
        vapprox[i] = (int64_t)appr;
    }

    // 6) sum & normalize (same as baseline; exact 128-bit sum)
    __int128 sum = 0;
    for (int j=0;j<N;j++) sum += (__int128)vapprox[j];

    if (sum == 0) {
        int idx_max = 0;
        for (int j=1;j<N;j++) if (v[j] > v[idx_max]) idx_max = j;
        for (int j=0;j<N;j++) out8[j] = 0;
        out8[idx_max] = 255;
    } else {
        for (int j=0;j<N;j++) {
            __int128 num = (__int128)vapprox[j] * 255 + (sum>>1); // round
            int64_t p = (int64_t)(num / sum);
            if (p < 0) p = 0; if (p > 255) p = 255;
            out8[j] = (uint8_t)p;
        }
    }

    free(vstable);
    free(vapprox);
    return 0;
}

// ------------------- demo -------------------
int main(void) {
    int N = 16;
    int32_t v[16] = { 10, 6, -4, 0, 3, 9, 1, -3, 7, -2, 5, 4, -1, 2, 8, 11 };
    int M = 8;
    double S = 0.02;

    uint8_t out8[16];
    int rc = int_softmax_mac_ilp(v, N, M, S, out8);
    if (rc != 0) { printf("error %d\n", rc); return 1; }

    // minimal print to keep the work live
    printf("N=%d LANES=%d ok\n", N, LANES);
    return 0;
}
