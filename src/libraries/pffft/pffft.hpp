/* Copyright (c) 2013  Julien Pommier ( pommier@modartt.com ) 

   Based on original fortran 77 code from FFTPACKv4 from NETLIB,
   authored by Dr Paul Swarztrauber of NCAR, in 1985.

   As confirmed by the NCAR fftpack software curators, the following
   FFTPACKv5 license applies to FFTPACKv4 sources. My changes are
   released under the same terms.

   FFTPACK license:

   http://www.cisl.ucar.edu/css/software/fftpack5/ftpk.html

   Copyright (c) 2004 the University Corporation for Atmospheric
   Research ("UCAR"). All rights reserved. Developed by NCAR's
   Computational and Information Systems Laboratory, UCAR,
   www.cisl.ucar.edu.

   Redistribution and use of the Software in source and binary forms,
   with or without modification, is permitted provided that the
   following conditions are met:

   - Neither the names of NCAR's Computational and Information Systems
   Laboratory, the University Corporation for Atmospheric Research,
   nor the names of its sponsors or contributors may be used to
   endorse or promote products derived from this Software without
   specific prior written permission.  

   - Redistributions of source code must retain the above copyright
   notices, this list of conditions, and the disclaimer below.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions, and the disclaimer below in the
   documentation and/or other materials provided with the
   distribution.

   THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE CONTRIBUTORS OR COPYRIGHT
   HOLDERS BE LIABLE FOR ANY CLAIM, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE
   SOFTWARE.
*/

/*
   PFFFT : a Pretty Fast FFT.

   This is basically an adaptation of the single precision fftpack
   (v4) as found on netlib taking advantage of SIMD instruction found
   on cpus such as intel x86 (SSE1), powerpc (Altivec), and arm (NEON).
   
   For architectures where no SIMD instruction is available, the code
   falls back to a scalar version.  

   Restrictions: 

   - 1D transforms only, with 32-bit single precision.

   - supports only transforms for inputs of length N of the form
   N=(2^a)*(3^b)*(5^c), a >= 5, b >=0, c >= 0 (32, 48, 64, 96, 128,
   144, 160, etc are all acceptable lengths). Performance is best for
   128<=N<=8192.

   - all (float*) pointers in the functions below are expected to
   have an "simd-compatible" alignment, that is 16 bytes on x86 and
   powerpc CPUs.
  
   You can allocate such buffers with the functions
   pffft_aligned_malloc / pffft_aligned_free (or with stuff like
   posix_memalign..)

*/

#ifndef A4BB9DC5_4A64_497C_B70B_714424DB70F7
#define A4BB9DC5_4A64_497C_B70B_714424DB70F7

#include "swan.hpp"

#include <stddef.h> // for size_t

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PFFFT_SIMD_DISABLE
// Detects compiler bugs with respect to simd instruction.
void validate_pffft_simd();
#endif

/* direction of the transform */
typedef enum { PFFFT_FORWARD,
               PFFFT_BACKWARD } pffft_direction_t;

/* type of transform */
typedef enum { PFFFT_REAL,
               PFFFT_COMPLEX } pffft_transform_t;

/* opaque struct holding internal stuff (precomputed twiddle factors)
   this struct can be shared by many threads as it contains only
   read-only data.
*/
typedef struct PFFFT_Setup {
    int N;
    int Ncvec; // nb of complex simd vectors (N/4 if PFFFT_COMPLEX, N/8 if PFFFT_REAL)
    int ifac[15];
    pffft_transform_t transform;
    float *data;    // allocated room for twiddle coefs
    float *e;       // points into 'data' , N/4*3 elements
    float *twiddle; // points into 'data', N/4 elements
} PFFFT_Setup;

/*
  prepare for performing transforms of size N -- the returned
  PFFFT_Setup structure is read-only so it can safely be shared by
  multiple concurrent threads.
*/
PFFFT_Setup *pffft_new_setup(platform_t platform, int N, pffft_transform_t transform, size_t v4sf_size);
void pffft_destroy_setup(PFFFT_Setup *);

int decompose(int n, int *ifac, const int *ntryh);
void rffti1_ps(int n, float *wa, int *ifac);
void cffti1_ps(int n, float *wa, int *ifac);

#ifdef __cplusplus
}
#endif

/* detect compiler flavour */
#if defined(_MSC_VER)
#define COMPILER_MSVC
#elif defined(__GNUC__)
#define COMPILER_GCC
#endif

#if defined(COMPILER_GCC)
#define ALWAYS_INLINE(return_type) inline return_type __attribute__((always_inline))
#define NEVER_INLINE(return_type) return_type __attribute__((noinline))
#define RESTRICT __restrict
#define VLA_ARRAY_ON_STACK(type__, varname__, size__) type__ varname__[size__];
#define VLA_ARRAY_ON_STACK_FREE(varname__)
#elif defined(COMPILER_MSVC)
#include <malloc.h>
#define ALWAYS_INLINE(return_type) __forceinline return_type
#define NEVER_INLINE(return_type) __declspec(noinline) return_type
#define RESTRICT __restrict
#define VLA_ARRAY_ON_STACK(type__, varname__, size__) type__ *varname__ = (type__ *)_malloca(size__ * sizeof(type__))
#define VLA_ARRAY_ON_STACK_FREE(varname__) _freea(varname__)
#endif

/* 
   vector support macros: the rest of the code is independant of
   SSE/Altivec/NEON -- adding support for other platforms with 4-element
   vectors should be limited to these macros 
*/

typedef struct pffft_config_s : config_t {
    // 2048 fft
    int N;
    uint32_t block_count;
    PFFFT_Setup *scalar_setup;
    PFFFT_Setup *neon_setup;
    pffft_direction_t direction;
} pffft_config_t;

typedef struct pffft_input_s : input_t {
    float **input_buff;
    float *work;
} pffft_input_t;

typedef struct pffft_output_s : output_t {
    float **output_buff;
} pffft_output_t;

#endif /* A4BB9DC5_4A64_497C_B70B_714424DB70F7 */
