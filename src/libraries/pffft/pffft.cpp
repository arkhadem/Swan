#include "pffft.hpp"
#include "math.h"
#include "swan.hpp"
#include <stdio.h>
#include <stdlib.h>

int decompose(int n, int *ifac, const int *ntryh) {
    int nl = n, nf = 0, i, j = 0;
    for (j = 0; ntryh[j]; ++j) {
        int ntry = ntryh[j];
        while (nl != 1) {
            int nq = nl / ntry;
            int nr = nl - ntry * nq;
            if (nr == 0) {
                ifac[2 + nf++] = ntry;
                nl = nq;
                if (ntry == 2 && nf != 1) {
                    for (i = 2; i <= nf; ++i) {
                        int ib = nf - i + 2;
                        ifac[ib + 1] = ifac[ib];
                    }
                    ifac[2] = 2;
                }
            } else
                break;
        }
    }
    ifac[0] = n;
    ifac[1] = nf;
    return nf;
}

void rffti1_ps(int n, float *wa, int *ifac) {
    const int ntryh[] = {4, 2, 3, 5, 0};
    int k1, j, ii;

    int nf = decompose(n, ifac, ntryh);
    float argh = (2 * M_PI) / n;
    int is = 0;
    int nfm1 = nf - 1;
    int l1 = 1;
    for (k1 = 1; k1 <= nfm1; k1++) {
        int ip = ifac[k1 + 1];
        int ld = 0;
        int l2 = l1 * ip;
        int ido = n / l2;
        int ipm = ip - 1;
        for (j = 1; j <= ipm; ++j) {
            float argld;
            int i = is, fi = 0;
            ld += l1;
            argld = ld * argh;
            for (ii = 3; ii <= ido; ii += 2) {
                i += 2;
                fi += 1;
                wa[i - 2] = cos(fi * argld);
                wa[i - 1] = sin(fi * argld);
            }
            is += ido;
        }
        l1 = l2;
    }
} /* rffti1 */

void cffti1_ps(int n, float *wa, int *ifac) {
    const int ntryh[] = {5, 3, 4, 2, 0};
    int k1, j, ii;

    int nf = decompose(n, ifac, ntryh);
    float argh = (2 * M_PI) / (float)n;
    int i = 1;
    int l1 = 1;
    for (k1 = 1; k1 <= nf; k1++) {
        int ip = ifac[k1 + 1];
        int ld = 0;
        int l2 = l1 * ip;
        int ido = n / l2;
        int idot = ido + ido + 2;
        int ipm = ip - 1;
        for (j = 1; j <= ipm; j++) {
            float argld;
            int i1 = i, fi = 0;
            wa[i - 1] = 1;
            wa[i] = 0;
            ld += l1;
            argld = ld * argh;
            for (ii = 4; ii <= idot; ii += 2) {
                i += 2;
                fi += 1;
                wa[i - 1] = cos(fi * argld);
                wa[i] = sin(fi * argld);
            }
            if (ip > 5) {
                wa[i1 - 1] = wa[i - 1];
                wa[i1] = wa[i];
            }
        }
        l1 = l2;
    }
} /* cffti1 */

void print_setup(platform_t platform, PFFFT_Setup *setup) {
    printf("N: %d, Ncvec: %d, transorm: %s\n", setup->N, setup->Ncvec, setup->transform == PFFFT_REAL ? "real" : "complex");
    printf("ifac = {");
    for (int i = 0; i < 15; i++) {
        printf("%d ", setup->ifac[i]);
    }
    printf("\n");
    int SIMD_SZ = 0;
    if (platform == platform_t::NEON_PLAT) {
        SIMD_SZ = 4;
    } else if (platform == platform_t::SCALAR_PLAT) {
        SIMD_SZ = 1;
    } else {
        SWAN_ASSERT(false)
    }
    for (int i = 0; i < 2 * setup->Ncvec * SIMD_SZ; i++) {
        printf("data[%d] = %f\n", i, setup->data[i]);
    }
}

PFFFT_Setup *pffft_new_setup(platform_t platform, int N, pffft_transform_t transform, size_t v4sf_size) {
    PFFFT_Setup *s = (PFFFT_Setup *)malloc(sizeof(PFFFT_Setup));
    int k, m;
    int SIMD_SZ = 0;
    if (platform == platform_t::NEON_PLAT) {
        SIMD_SZ = 4;
    } else if (platform == platform_t::SCALAR_PLAT) {
        SIMD_SZ = 1;
    } else {
        SWAN_ASSERT(false)
    }
    /* unfortunately, the fft size must be a multiple of 16 for complex FFTs 
     and 32 for real FFTs -- a lot of stuff would need to be rewritten to
     handle other cases (or maybe just switch to a scalar fft, I don't know..) */
    if (transform == PFFFT_REAL) {
        SWAN_ASSERT((N % (2 * SIMD_SZ * SIMD_SZ)) == 0 && N > 0)
    }
    if (transform == PFFFT_COMPLEX) {
        SWAN_ASSERT((N % (SIMD_SZ * SIMD_SZ)) == 0 && N > 0)
    }
    //SWAN_ASSERT((N % 32) == 0)
    s->N = N;
    s->transform = transform;
    /* nb of complex simd vectors */
    s->Ncvec = (transform == PFFFT_REAL ? N / 2 : N) / SIMD_SZ;
    s->data = (float *)malloc(2 * s->Ncvec * v4sf_size);
    s->e = s->data;
    s->twiddle = s->data + ((size_t)((2 * s->Ncvec * (SIMD_SZ - 1)) / SIMD_SZ) * v4sf_size / sizeof(float));

    if (transform == PFFFT_REAL) {
        for (k = 0; k < s->Ncvec; ++k) {
            int i = k / SIMD_SZ;
            int j = k % SIMD_SZ;
            for (m = 0; m < SIMD_SZ - 1; ++m) {
                float A = -2 * M_PI * (m + 1) * k / N;
                s->e[(2 * (i * 3 + m) + 0) * SIMD_SZ + j] = cos(A);
                s->e[(2 * (i * 3 + m) + 1) * SIMD_SZ + j] = sin(A);
            }
        }
        rffti1_ps(N / SIMD_SZ, s->twiddle, s->ifac);
    } else {
        for (k = 0; k < s->Ncvec; ++k) {
            int i = k / SIMD_SZ;
            int j = k % SIMD_SZ;
            for (m = 0; m < SIMD_SZ - 1; ++m) {
                float A = -2 * M_PI * (m + 1) * k / N;
                s->e[(2 * (i * 3 + m) + 0) * SIMD_SZ + j] = cos(A);
                s->e[(2 * (i * 3 + m) + 1) * SIMD_SZ + j] = sin(A);
            }
        }
        cffti1_ps(N / SIMD_SZ, s->twiddle, s->ifac);
    }

    /* check that N is decomposable with allowed prime factors */
    for (k = 0, m = 1; k < s->ifac[1]; ++k) {
        m *= s->ifac[2 + k];
    }
    if (m != N / SIMD_SZ) {
        pffft_destroy_setup(s);
        s = 0;
    }
    // print_setup(platform, s);
    return s;
}

void pffft_destroy_setup(PFFFT_Setup *s) {
    free(s->data);
    free(s);
}