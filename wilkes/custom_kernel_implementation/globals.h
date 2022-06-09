#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <immintrin.h>
#ifdef _WIN32
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#define likely(x) (x)
#define unlikely(x) (x)
#else
#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)
#endif

// the __rdtsc intrinsic is defined automatically by either intrin.h or
// x86intrin.h unsigned long long rdtsc();
#define rdtsc __rdtsc

// MM256_FMADD_PD instruction define.
#ifdef _WIN32
#define FMADD_PS(src1, src2, dest) _mm256_fmadd_ps((src1), (src2), (dest))
#else
#define FMADD_PS(src1, src2, dest)                                    \
    __asm__ __volatile__("vfmadd231ps %[xsrc1], %[xsrc2], %[xdest]\n" \
                         : [xdest] "+x"(dest)                         \
                         : [xsrc1] "x"(src1), [xsrc2] "x"(src2));
#endif

#define NUM_COLS_IN_ONE_PANEL 256

typedef struct Globals {
    int in_c, in_r, out_c;
    int num_c;
    __m256 r[3];  // 3 rows loaded column wise
    __m256 out0, out1, out2, out3, out4, out5, out6, out7, out8,
      out9;             // 10 independant FMA chains
    __m256 k0, k1, k2;  // broadcasted values for each row of kernel
    double sum;
    unsigned long long st3, et3, st5, et5;
} Globals;

#define INIT_GLOBALS(GLOB)                \
    (GLOB).in_c = 0;                      \
    (GLOB).in_r = 0;                      \
    (GLOB).out_c = 0;                     \
    (GLOB).num_c = NUM_COLS_IN_ONE_PANEL; \
    (GLOB).sum = 0.0f

#define GLOBALS_TO_LOCALS(GLOB) \
    in_c = (GLOB).in_c;         \
    in_r = (GLOB).in_r;         \
    out_c = (GLOB).out_c;       \
    num_c = (GLOB).num_c;       \
    r[0] = (GLOB).r[0];         \
    r[1] = (GLOB).r[1];         \
    r[2] = (GLOB).r[2];         \
    out0 = (GLOB).out0;         \
    out1 = (GLOB).out1;         \
    out2 = (GLOB).out2;         \
    out3 = (GLOB).out3;         \
    out4 = (GLOB).out4;         \
    out5 = (GLOB).out5;         \
    out6 = (GLOB).out6;         \
    out7 = (GLOB).out7;         \
    out8 = (GLOB).out8;         \
    out9 = (GLOB).out9;         \
    k0 = (GLOB).k0;             \
    k1 = (GLOB).k1;             \
    k2 = (GLOB).k2;             \
    sum = (GLOB).sum;           \
    st3 = (GLOB).st3;           \
    et3 = (GLOB).et3;           \
    st5 = (GLOB).st5;           \
    et5 = (GLOB).et5

#define DECLARE_AND_GLOBALS_TO_LOCALS(GLOB) \
    int in_c;                               \
    int in_r;                               \
    int out_c;                              \
    int num_c;                              \
    __m256 r[3];                            \
    __m256 out0;                            \
    __m256 out1;                            \
    __m256 out2;                            \
    __m256 out3;                            \
    __m256 out4;                            \
    __m256 out5;                            \
    __m256 out6;                            \
    __m256 out7;                            \
    __m256 out8;                            \
    __m256 out9;                            \
    __m256 k0;                              \
    __m256 k1;                              \
    __m256 k2;                              \
    double sum;                             \
    unsigned long long st3;                 \
    unsigned long long et3;                 \
    unsigned long long st5;                 \
    unsigned long long et5;                 \
    GLOBALS_TO_LOCALS(GLOB)

#define LOCALS_TO_GLOBALS(GLOB) \
    (GLOB).in_c = in_c;         \
    (GLOB).in_r = in_r;         \
    (GLOB).out_c = out_c;       \
    (GLOB).num_c = num_c;       \
    (GLOB).r[0] = r[0];         \
    (GLOB).r[1] = r[1];         \
    (GLOB).r[2] = r[2];         \
    (GLOB).out0 = out0;         \
    (GLOB).out1 = out1;         \
    (GLOB).out2 = out2;         \
    (GLOB).out3 = out3;         \
    (GLOB).out4 = out4;         \
    (GLOB).out5 = out5;         \
    (GLOB).out6 = out6;         \
    (GLOB).out7 = out7;         \
    (GLOB).out8 = out8;         \
    (GLOB).out9 = out9;         \
    (GLOB).k0 = k0;             \
    (GLOB).k1 = k1;             \
    (GLOB).k2 = k2;             \
    (GLOB).sum = sum;           \
    (GLOB).st3 = st3;           \
    (GLOB).et3 = et3;           \
    (GLOB).st5 = st5;           \
    (GLOB).et5 = et5

/*
 * A replacement for getline, which isn't available on all systems.
 */

/* MSVC apparently doesn't have C's my_getline
// This is a poor man's implementation of it using fgets.
// See the man page at
// http://pubs.opengroup.org/onlinepubs/9699919799/functions/getdelim.html
*/
size_t my_getline(char** buf, size_t* bufLen, FILE* f);

#ifdef __cplusplus
}  // end extern "C"
#endif
