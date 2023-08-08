/* Copyright (c) 2013  Julien Pommier ( pommier@modartt.com )

   Based on original fortran 77 code from FFTPACKv4 from NETLIB
   (http://www.netlib.org/fftpack), authored by Dr Paul Swarztrauber
   of NCAR, in 1985.

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


   PFFFT : a Pretty Fast FFT.

   This file is largerly based on the original FFTPACK implementation, modified in
   order to take advantage of SIMD instructions of modern CPUs.
*/

#include "pffft.hpp"
#include "swan.hpp"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

// fallback mode for situations where SSE/Altivec are not available, use scalar mode instead
#define v4sf float
#define SIMD_SZ 1
#define VZERO() 0.f
#define VMUL(a, b) ((a) * (b))
#define VADD(a, b) ((a) + (b))
#define VMADD(a, b, c) ((a) * (b) + (c))
#define VSUB(a, b) ((a) - (b))
#define LD_PS1(p) (p)
#define VALIGNED(ptr) ((((uintptr_t)(ptr)) & 0x3) == 0)

// shortcuts for complex multiplcations
#define VCPLXMUL(ar, ai, br, bi)     \
    {                                \
        v4sf tmp;                    \
        tmp = VMUL(ar, bi);          \
        ar = VMUL(ar, br);           \
        ar = VSUB(ar, VMUL(ai, bi)); \
        ai = VMUL(ai, br);           \
        ai = VADD(ai, tmp);          \
    }
#define VCPLXMULCONJ(ar, ai, br, bi) \
    {                                \
        v4sf tmp;                    \
        tmp = VMUL(ar, bi);          \
        ar = VMUL(ar, br);           \
        ar = VADD(ar, VMUL(ai, bi)); \
        ai = VMUL(ai, br);           \
        ai = VSUB(ai, tmp);          \
    }
// multiply a scalar with a vector
#define SVMUL(f, v) VMUL(LD_PS1(f), v)

/*
  passf2 and passb2 has been merged here, fsign = -1 for passf2, +1 for passb2
*/
static NEVER_INLINE(void) passf2_ps(int ido, int l1, const v4sf *cc, v4sf *ch, const float *wa1, float fsign) {
    int k, i;
    int l1ido = l1 * ido;
    if (ido <= 2) {
        for (k = 0; k < l1ido; k += ido, ch += ido, cc += 2 * ido) {
            ch[0] = VADD(cc[0], cc[ido + 0]);
            ch[l1ido] = VSUB(cc[0], cc[ido + 0]);
            ch[1] = VADD(cc[1], cc[ido + 1]);
            ch[l1ido + 1] = VSUB(cc[1], cc[ido + 1]);
        }
    } else {
        for (k = 0; k < l1ido; k += ido, ch += ido, cc += 2 * ido) {
            for (i = 0; i < ido - 1; i += 2) {
                v4sf tr2 = VSUB(cc[i + 0], cc[i + ido + 0]);
                v4sf ti2 = VSUB(cc[i + 1], cc[i + ido + 1]);
                v4sf wr = LD_PS1(wa1[i]), wi = VMUL(LD_PS1(fsign), LD_PS1(wa1[i + 1]));
                ch[i] = VADD(cc[i + 0], cc[i + ido + 0]);
                ch[i + 1] = VADD(cc[i + 1], cc[i + ido + 1]);
                VCPLXMUL(tr2, ti2, wr, wi);
                ch[i + l1ido] = tr2;
                ch[i + l1ido + 1] = ti2;
            }
        }
    }
}

/*
  passf3 and passb3 has been merged here, fsign = -1 for passf3, +1 for passb3
*/
static NEVER_INLINE(void) passf3_ps(int ido, int l1, const v4sf *cc, v4sf *ch,
                                    const float *wa1, const float *wa2, float fsign) {
    static const float taur = -0.5f;
    float taui = 0.866025403784439f * fsign;
    int i, k;
    v4sf tr2, ti2, cr2, ci2, cr3, ci3, dr2, di2, dr3, di3;
    int l1ido = l1 * ido;
    float wr1, wi1, wr2, wi2;
    SWAN_ASSERT(ido > 2)
    for (k = 0; k < l1ido; k += ido, cc += 3 * ido, ch += ido) {
        for (i = 0; i < ido - 1; i += 2) {
            tr2 = VADD(cc[i + ido], cc[i + 2 * ido]);
            cr2 = VADD(cc[i], SVMUL(taur, tr2));
            ch[i] = VADD(cc[i], tr2);
            ti2 = VADD(cc[i + ido + 1], cc[i + 2 * ido + 1]);
            ci2 = VADD(cc[i + 1], SVMUL(taur, ti2));
            ch[i + 1] = VADD(cc[i + 1], ti2);
            cr3 = SVMUL(taui, VSUB(cc[i + ido], cc[i + 2 * ido]));
            ci3 = SVMUL(taui, VSUB(cc[i + ido + 1], cc[i + 2 * ido + 1]));
            dr2 = VSUB(cr2, ci3);
            dr3 = VADD(cr2, ci3);
            di2 = VADD(ci2, cr3);
            di3 = VSUB(ci2, cr3);
            wr1 = wa1[i], wi1 = fsign * wa1[i + 1], wr2 = wa2[i], wi2 = fsign * wa2[i + 1];
            VCPLXMUL(dr2, di2, LD_PS1(wr1), LD_PS1(wi1));
            ch[i + l1ido] = dr2;
            ch[i + l1ido + 1] = di2;
            VCPLXMUL(dr3, di3, LD_PS1(wr2), LD_PS1(wi2));
            ch[i + 2 * l1ido] = dr3;
            ch[i + 2 * l1ido + 1] = di3;
        }
    }
} /* passf3 */

static NEVER_INLINE(void) passf4_ps(int ido, int l1, const v4sf *cc, v4sf *ch,
                                    const float *wa1, const float *wa2, const float *wa3, float fsign) {
    /* isign == -1 for forward transform and +1 for backward transform */

    int i, k;
    v4sf ci2, ci3, ci4, cr2, cr3, cr4, ti1, ti2, ti3, ti4, tr1, tr2, tr3, tr4;
    int l1ido = l1 * ido;
    if (ido == 2) {
        for (k = 0; k < l1ido; k += ido, ch += ido, cc += 4 * ido) {
            tr1 = VSUB(cc[0], cc[2 * ido + 0]);
            tr2 = VADD(cc[0], cc[2 * ido + 0]);
            ti1 = VSUB(cc[1], cc[2 * ido + 1]);
            ti2 = VADD(cc[1], cc[2 * ido + 1]);
            ti4 = VMUL(VSUB(cc[1 * ido + 0], cc[3 * ido + 0]), LD_PS1(fsign));
            tr4 = VMUL(VSUB(cc[3 * ido + 1], cc[1 * ido + 1]), LD_PS1(fsign));
            tr3 = VADD(cc[ido + 0], cc[3 * ido + 0]);
            ti3 = VADD(cc[ido + 1], cc[3 * ido + 1]);

            ch[0 * l1ido + 0] = VADD(tr2, tr3);
            ch[0 * l1ido + 1] = VADD(ti2, ti3);
            ch[1 * l1ido + 0] = VADD(tr1, tr4);
            ch[1 * l1ido + 1] = VADD(ti1, ti4);
            ch[2 * l1ido + 0] = VSUB(tr2, tr3);
            ch[2 * l1ido + 1] = VSUB(ti2, ti3);
            ch[3 * l1ido + 0] = VSUB(tr1, tr4);
            ch[3 * l1ido + 1] = VSUB(ti1, ti4);
        }
    } else {
        for (k = 0; k < l1ido; k += ido, ch += ido, cc += 4 * ido) {
            for (i = 0; i < ido - 1; i += 2) {
                float wr1, wi1, wr2, wi2, wr3, wi3;
                tr1 = VSUB(cc[i + 0], cc[i + 2 * ido + 0]);
                tr2 = VADD(cc[i + 0], cc[i + 2 * ido + 0]);
                ti1 = VSUB(cc[i + 1], cc[i + 2 * ido + 1]);
                ti2 = VADD(cc[i + 1], cc[i + 2 * ido + 1]);
                tr4 = VMUL(VSUB(cc[i + 3 * ido + 1], cc[i + 1 * ido + 1]), LD_PS1(fsign));
                ti4 = VMUL(VSUB(cc[i + 1 * ido + 0], cc[i + 3 * ido + 0]), LD_PS1(fsign));
                tr3 = VADD(cc[i + ido + 0], cc[i + 3 * ido + 0]);
                ti3 = VADD(cc[i + ido + 1], cc[i + 3 * ido + 1]);

                ch[i] = VADD(tr2, tr3);
                cr3 = VSUB(tr2, tr3);
                ch[i + 1] = VADD(ti2, ti3);
                ci3 = VSUB(ti2, ti3);

                cr2 = VADD(tr1, tr4);
                cr4 = VSUB(tr1, tr4);
                ci2 = VADD(ti1, ti4);
                ci4 = VSUB(ti1, ti4);
                wr1 = wa1[i], wi1 = fsign * wa1[i + 1];
                VCPLXMUL(cr2, ci2, LD_PS1(wr1), LD_PS1(wi1));
                wr2 = wa2[i], wi2 = fsign * wa2[i + 1];
                ch[i + l1ido] = cr2;
                ch[i + l1ido + 1] = ci2;

                VCPLXMUL(cr3, ci3, LD_PS1(wr2), LD_PS1(wi2));
                wr3 = wa3[i], wi3 = fsign * wa3[i + 1];
                ch[i + 2 * l1ido] = cr3;
                ch[i + 2 * l1ido + 1] = ci3;

                VCPLXMUL(cr4, ci4, LD_PS1(wr3), LD_PS1(wi3));
                ch[i + 3 * l1ido] = cr4;
                ch[i + 3 * l1ido + 1] = ci4;
            }
        }
    }
} /* passf4 */

/*
  passf5 and passb5 has been merged here, fsign = -1 for passf5, +1 for passb5
*/
static NEVER_INLINE(void) passf5_ps(int ido, int l1, const v4sf *cc, v4sf *ch,
                                    const float *wa1, const float *wa2,
                                    const float *wa3, const float *wa4, float fsign) {
    static const float tr11 = .309016994374947f;
    const float ti11 = .951056516295154f * fsign;
    static const float tr12 = -.809016994374947f;
    const float ti12 = .587785252292473f * fsign;

    /* Local variables */
    int i, k;
    v4sf ci2, ci3, ci4, ci5, di3, di4, di5, di2, cr2, cr3, cr5, cr4, ti2, ti3,
        ti4, ti5, dr3, dr4, dr5, dr2, tr2, tr3, tr4, tr5;

    float wr1, wi1, wr2, wi2, wr3, wi3, wr4, wi4;

#define cc_ref(a_1, a_2) cc[(a_2 - 1) * ido + a_1 + 1]
#define ch_ref(a_1, a_3) ch[(a_3 - 1) * l1 * ido + a_1 + 1]

    SWAN_ASSERT(ido > 2)
    for (k = 0; k < l1; ++k, cc += 5 * ido, ch += ido) {
        for (i = 0; i < ido - 1; i += 2) {
            ti5 = VSUB(cc_ref(i, 2), cc_ref(i, 5));
            ti2 = VADD(cc_ref(i, 2), cc_ref(i, 5));
            ti4 = VSUB(cc_ref(i, 3), cc_ref(i, 4));
            ti3 = VADD(cc_ref(i, 3), cc_ref(i, 4));
            tr5 = VSUB(cc_ref(i - 1, 2), cc_ref(i - 1, 5));
            tr2 = VADD(cc_ref(i - 1, 2), cc_ref(i - 1, 5));
            tr4 = VSUB(cc_ref(i - 1, 3), cc_ref(i - 1, 4));
            tr3 = VADD(cc_ref(i - 1, 3), cc_ref(i - 1, 4));
            ch_ref(i - 1, 1) = VADD(cc_ref(i - 1, 1), VADD(tr2, tr3));
            ch_ref(i, 1) = VADD(cc_ref(i, 1), VADD(ti2, ti3));
            cr2 = VADD(cc_ref(i - 1, 1), VADD(SVMUL(tr11, tr2), SVMUL(tr12, tr3)));
            ci2 = VADD(cc_ref(i, 1), VADD(SVMUL(tr11, ti2), SVMUL(tr12, ti3)));
            cr3 = VADD(cc_ref(i - 1, 1), VADD(SVMUL(tr12, tr2), SVMUL(tr11, tr3)));
            ci3 = VADD(cc_ref(i, 1), VADD(SVMUL(tr12, ti2), SVMUL(tr11, ti3)));
            cr5 = VADD(SVMUL(ti11, tr5), SVMUL(ti12, tr4));
            ci5 = VADD(SVMUL(ti11, ti5), SVMUL(ti12, ti4));
            cr4 = VSUB(SVMUL(ti12, tr5), SVMUL(ti11, tr4));
            ci4 = VSUB(SVMUL(ti12, ti5), SVMUL(ti11, ti4));
            dr3 = VSUB(cr3, ci4);
            dr4 = VADD(cr3, ci4);
            di3 = VADD(ci3, cr4);
            di4 = VSUB(ci3, cr4);
            dr5 = VADD(cr2, ci5);
            dr2 = VSUB(cr2, ci5);
            di5 = VSUB(ci2, cr5);
            di2 = VADD(ci2, cr5);
            wr1 = wa1[i], wi1 = fsign * wa1[i + 1], wr2 = wa2[i], wi2 = fsign * wa2[i + 1];
            wr3 = wa3[i], wi3 = fsign * wa3[i + 1], wr4 = wa4[i], wi4 = fsign * wa4[i + 1];
            VCPLXMUL(dr2, di2, LD_PS1(wr1), LD_PS1(wi1));
            ch_ref(i - 1, 2) = dr2;
            ch_ref(i, 2) = di2;
            VCPLXMUL(dr3, di3, LD_PS1(wr2), LD_PS1(wi2));
            ch_ref(i - 1, 3) = dr3;
            ch_ref(i, 3) = di3;
            VCPLXMUL(dr4, di4, LD_PS1(wr3), LD_PS1(wi3));
            ch_ref(i - 1, 4) = dr4;
            ch_ref(i, 4) = di4;
            VCPLXMUL(dr5, di5, LD_PS1(wr4), LD_PS1(wi4));
            ch_ref(i - 1, 5) = dr5;
            ch_ref(i, 5) = di5;
        }
    }
#undef ch_ref
#undef cc_ref
}

static NEVER_INLINE(void) radf2_ps(int ido, int l1, const v4sf *RESTRICT cc, v4sf *RESTRICT ch, const float *wa1) {
    static const float minus_one = -1.f;
    int i, k, l1ido = l1 * ido;
    for (k = 0; k < l1ido; k += ido) {
        v4sf a = cc[k], b = cc[k + l1ido];
        ch[2 * k] = VADD(a, b);
        ch[2 * (k + ido) - 1] = VSUB(a, b);
    }
    if (ido < 2)
        return;
    if (ido != 2) {
        for (k = 0; k < l1ido; k += ido) {
            for (i = 2; i < ido; i += 2) {
                v4sf tr2 = cc[i - 1 + k + l1ido], ti2 = cc[i + k + l1ido];
                v4sf br = cc[i - 1 + k], bi = cc[i + k];
                VCPLXMULCONJ(tr2, ti2, LD_PS1(wa1[i - 2]), LD_PS1(wa1[i - 1]));
                ch[i + 2 * k] = VADD(bi, ti2);
                ch[2 * (k + ido) - i] = VSUB(ti2, bi);
                ch[i - 1 + 2 * k] = VADD(br, tr2);
                ch[2 * (k + ido) - i - 1] = VSUB(br, tr2);
            }
        }
        if (ido % 2 == 1)
            return;
    }
    for (k = 0; k < l1ido; k += ido) {
        ch[2 * k + ido] = SVMUL(minus_one, cc[ido - 1 + k + l1ido]);
        ch[2 * k + ido - 1] = cc[k + ido - 1];
    }
} /* radf2 */

static NEVER_INLINE(void) radb2_ps(int ido, int l1, const v4sf *cc, v4sf *ch, const float *wa1) {
    static const float minus_two = -2;
    int i, k, l1ido = l1 * ido;
    v4sf a, b, c, d, tr2, ti2;
    for (k = 0; k < l1ido; k += ido) {
        a = cc[2 * k];
        b = cc[2 * (k + ido) - 1];
        ch[k] = VADD(a, b);
        ch[k + l1ido] = VSUB(a, b);
    }
    if (ido < 2)
        return;
    if (ido != 2) {
        for (k = 0; k < l1ido; k += ido) {
            for (i = 2; i < ido; i += 2) {
                a = cc[i - 1 + 2 * k];
                b = cc[2 * (k + ido) - i - 1];
                c = cc[i + 0 + 2 * k];
                d = cc[2 * (k + ido) - i + 0];
                ch[i - 1 + k] = VADD(a, b);
                tr2 = VSUB(a, b);
                ch[i + 0 + k] = VSUB(c, d);
                ti2 = VADD(c, d);
                VCPLXMUL(tr2, ti2, LD_PS1(wa1[i - 2]), LD_PS1(wa1[i - 1]));
                ch[i - 1 + k + l1ido] = tr2;
                ch[i + 0 + k + l1ido] = ti2;
            }
        }
        if (ido % 2 == 1)
            return;
    }
    for (k = 0; k < l1ido; k += ido) {
        a = cc[2 * k + ido - 1];
        b = cc[2 * k + ido];
        ch[k + ido - 1] = VADD(a, a);
        ch[k + ido - 1 + l1ido] = SVMUL(minus_two, b);
    }
} /* radb2 */

static void radf3_ps(int ido, int l1, const v4sf *RESTRICT cc, v4sf *RESTRICT ch,
                     const float *wa1, const float *wa2) {
    static const float taur = -0.5f;
    static const float taui = 0.866025403784439f;
    int i, k, ic;
    v4sf ci2, di2, di3, cr2, dr2, dr3, ti2, ti3, tr2, tr3, wr1, wi1, wr2, wi2;
    for (k = 0; k < l1; k++) {
        cr2 = VADD(cc[(k + l1) * ido], cc[(k + 2 * l1) * ido]);
        ch[3 * k * ido] = VADD(cc[k * ido], cr2);
        ch[(3 * k + 2) * ido] = SVMUL(taui, VSUB(cc[(k + l1 * 2) * ido], cc[(k + l1) * ido]));
        ch[ido - 1 + (3 * k + 1) * ido] = VADD(cc[k * ido], SVMUL(taur, cr2));
    }
    if (ido == 1)
        return;
    for (k = 0; k < l1; k++) {
        for (i = 2; i < ido; i += 2) {
            ic = ido - i;
            wr1 = LD_PS1(wa1[i - 2]);
            wi1 = LD_PS1(wa1[i - 1]);
            dr2 = cc[i - 1 + (k + l1) * ido];
            di2 = cc[i + (k + l1) * ido];
            VCPLXMULCONJ(dr2, di2, wr1, wi1);

            wr2 = LD_PS1(wa2[i - 2]);
            wi2 = LD_PS1(wa2[i - 1]);
            dr3 = cc[i - 1 + (k + l1 * 2) * ido];
            di3 = cc[i + (k + l1 * 2) * ido];
            VCPLXMULCONJ(dr3, di3, wr2, wi2);

            cr2 = VADD(dr2, dr3);
            ci2 = VADD(di2, di3);
            ch[i - 1 + 3 * k * ido] = VADD(cc[i - 1 + k * ido], cr2);
            ch[i + 3 * k * ido] = VADD(cc[i + k * ido], ci2);
            tr2 = VADD(cc[i - 1 + k * ido], SVMUL(taur, cr2));
            ti2 = VADD(cc[i + k * ido], SVMUL(taur, ci2));
            tr3 = SVMUL(taui, VSUB(di2, di3));
            ti3 = SVMUL(taui, VSUB(dr3, dr2));
            ch[i - 1 + (3 * k + 2) * ido] = VADD(tr2, tr3);
            ch[ic - 1 + (3 * k + 1) * ido] = VSUB(tr2, tr3);
            ch[i + (3 * k + 2) * ido] = VADD(ti2, ti3);
            ch[ic + (3 * k + 1) * ido] = VSUB(ti3, ti2);
        }
    }
} /* radf3 */

static void radb3_ps(int ido, int l1, const v4sf *RESTRICT cc, v4sf *RESTRICT ch,
                     const float *wa1, const float *wa2) {
    static const float taur = -0.5f;
    static const float taui = 0.866025403784439f;
    static const float taui_2 = 0.866025403784439f * 2;
    int i, k, ic;
    v4sf ci2, ci3, di2, di3, cr2, cr3, dr2, dr3, ti2, tr2;
    for (k = 0; k < l1; k++) {
        tr2 = cc[ido - 1 + (3 * k + 1) * ido];
        tr2 = VADD(tr2, tr2);
        cr2 = VMADD(LD_PS1(taur), tr2, cc[3 * k * ido]);
        ch[k * ido] = VADD(cc[3 * k * ido], tr2);
        ci3 = SVMUL(taui_2, cc[(3 * k + 2) * ido]);
        ch[(k + l1) * ido] = VSUB(cr2, ci3);
        ch[(k + 2 * l1) * ido] = VADD(cr2, ci3);
    }
    if (ido == 1)
        return;
    for (k = 0; k < l1; k++) {
        for (i = 2; i < ido; i += 2) {
            ic = ido - i;
            tr2 = VADD(cc[i - 1 + (3 * k + 2) * ido], cc[ic - 1 + (3 * k + 1) * ido]);
            cr2 = VMADD(LD_PS1(taur), tr2, cc[i - 1 + 3 * k * ido]);
            ch[i - 1 + k * ido] = VADD(cc[i - 1 + 3 * k * ido], tr2);
            ti2 = VSUB(cc[i + (3 * k + 2) * ido], cc[ic + (3 * k + 1) * ido]);
            ci2 = VMADD(LD_PS1(taur), ti2, cc[i + 3 * k * ido]);
            ch[i + k * ido] = VADD(cc[i + 3 * k * ido], ti2);
            cr3 = SVMUL(taui, VSUB(cc[i - 1 + (3 * k + 2) * ido], cc[ic - 1 + (3 * k + 1) * ido]));
            ci3 = SVMUL(taui, VADD(cc[i + (3 * k + 2) * ido], cc[ic + (3 * k + 1) * ido]));
            dr2 = VSUB(cr2, ci3);
            dr3 = VADD(cr2, ci3);
            di2 = VADD(ci2, cr3);
            di3 = VSUB(ci2, cr3);
            VCPLXMUL(dr2, di2, LD_PS1(wa1[i - 2]), LD_PS1(wa1[i - 1]));
            ch[i - 1 + (k + l1) * ido] = dr2;
            ch[i + (k + l1) * ido] = di2;
            VCPLXMUL(dr3, di3, LD_PS1(wa2[i - 2]), LD_PS1(wa2[i - 1]));
            ch[i - 1 + (k + 2 * l1) * ido] = dr3;
            ch[i + (k + 2 * l1) * ido] = di3;
        }
    }
} /* radb3 */

static NEVER_INLINE(void) radf4_ps(int ido, int l1, const v4sf *RESTRICT cc, v4sf *RESTRICT ch,
                                   const float *RESTRICT wa1, const float *RESTRICT wa2, const float *RESTRICT wa3) {
    static const float minus_hsqt2 = (float)-0.7071067811865475;
    int i, k, l1ido = l1 * ido;
    {
        const v4sf *RESTRICT cc_ = cc, *RESTRICT cc_end = cc + l1ido;
        v4sf *RESTRICT ch_ = ch;
        while (cc < cc_end) {
            // this loop represents between 25% and 40% of total radf4_ps cost !
            v4sf a0 = cc[0], a1 = cc[l1ido];
            v4sf a2 = cc[2 * l1ido], a3 = cc[3 * l1ido];
            v4sf tr1 = VADD(a1, a3);
            v4sf tr2 = VADD(a0, a2);
            ch[2 * ido - 1] = VSUB(a0, a2);
            ch[2 * ido] = VSUB(a3, a1);
            ch[0] = VADD(tr1, tr2);
            ch[4 * ido - 1] = VSUB(tr2, tr1);
            cc += ido;
            ch += 4 * ido;
        }
        cc = cc_;
        ch = ch_;
    }
    if (ido < 2)
        return;
    if (ido != 2) {
        for (k = 0; k < l1ido; k += ido) {
            const v4sf *RESTRICT pc = (v4sf *)(cc + 1 + k);
            for (i = 2; i < ido; i += 2, pc += 2) {
                int ic = ido - i;
                v4sf wr, wi, cr2, ci2, cr3, ci3, cr4, ci4;
                v4sf tr1, ti1, tr2, ti2, tr3, ti3, tr4, ti4;

                cr2 = pc[1 * l1ido + 0];
                ci2 = pc[1 * l1ido + 1];
                wr = LD_PS1(wa1[i - 2]);
                wi = LD_PS1(wa1[i - 1]);
                VCPLXMULCONJ(cr2, ci2, wr, wi);

                cr3 = pc[2 * l1ido + 0];
                ci3 = pc[2 * l1ido + 1];
                wr = LD_PS1(wa2[i - 2]);
                wi = LD_PS1(wa2[i - 1]);
                VCPLXMULCONJ(cr3, ci3, wr, wi);

                cr4 = pc[3 * l1ido];
                ci4 = pc[3 * l1ido + 1];
                wr = LD_PS1(wa3[i - 2]);
                wi = LD_PS1(wa3[i - 1]);
                VCPLXMULCONJ(cr4, ci4, wr, wi);

                /* at this point, on SSE, five of "cr2 cr3 cr4 ci2 ci3 ci4" should be loaded in registers */

                tr1 = VADD(cr2, cr4);
                tr4 = VSUB(cr4, cr2);
                tr2 = VADD(pc[0], cr3);
                tr3 = VSUB(pc[0], cr3);
                ch[i - 1 + 4 * k] = VADD(tr1, tr2);
                ch[ic - 1 + 4 * k + 3 * ido] = VSUB(tr2, tr1); // at this point tr1 and tr2 can be disposed
                ti1 = VADD(ci2, ci4);
                ti4 = VSUB(ci2, ci4);
                ch[i - 1 + 4 * k + 2 * ido] = VADD(ti4, tr3);
                ch[ic - 1 + 4 * k + 1 * ido] = VSUB(tr3, ti4); // dispose tr3, ti4
                ti2 = VADD(pc[1], ci3);
                ti3 = VSUB(pc[1], ci3);
                ch[i + 4 * k] = VADD(ti1, ti2);
                ch[ic + 4 * k + 3 * ido] = VSUB(ti1, ti2);
                ch[i + 4 * k + 2 * ido] = VADD(tr4, ti3);
                ch[ic + 4 * k + 1 * ido] = VSUB(tr4, ti3);
            }
        }
        if (ido % 2 == 1)
            return;
    }
    for (k = 0; k < l1ido; k += ido) {
        v4sf a = cc[ido - 1 + k + l1ido], b = cc[ido - 1 + k + 3 * l1ido];
        v4sf c = cc[ido - 1 + k], d = cc[ido - 1 + k + 2 * l1ido];
        v4sf ti1 = SVMUL(minus_hsqt2, VADD(a, b));
        v4sf tr1 = SVMUL(minus_hsqt2, VSUB(b, a));
        ch[ido - 1 + 4 * k] = VADD(tr1, c);
        ch[ido - 1 + 4 * k + 2 * ido] = VSUB(c, tr1);
        ch[4 * k + 1 * ido] = VSUB(ti1, d);
        ch[4 * k + 3 * ido] = VADD(ti1, d);
    }
} /* radf4 */

static NEVER_INLINE(void) radb4_ps(int ido, int l1, const v4sf *RESTRICT cc, v4sf *RESTRICT ch,
                                   const float *RESTRICT wa1, const float *RESTRICT wa2, const float *RESTRICT wa3) {
    static const float minus_sqrt2 = (float)-1.414213562373095;
    static const float two = 2.f;
    int i, k, l1ido = l1 * ido;
    v4sf ci2, ci3, ci4, cr2, cr3, cr4, ti1, ti2, ti3, ti4, tr1, tr2, tr3, tr4;
    {
        const v4sf *RESTRICT cc_ = cc, *RESTRICT ch_end = ch + l1ido;
        v4sf *ch_ = ch;
        while (ch < ch_end) {
            v4sf a = cc[0], b = cc[4 * ido - 1];
            v4sf c = cc[2 * ido], d = cc[2 * ido - 1];
            tr3 = SVMUL(two, d);
            tr2 = VADD(a, b);
            tr1 = VSUB(a, b);
            tr4 = SVMUL(two, c);
            ch[0 * l1ido] = VADD(tr2, tr3);
            ch[2 * l1ido] = VSUB(tr2, tr3);
            ch[1 * l1ido] = VSUB(tr1, tr4);
            ch[3 * l1ido] = VADD(tr1, tr4);

            cc += 4 * ido;
            ch += ido;
        }
        cc = cc_;
        ch = ch_;
    }
    if (ido < 2)
        return;
    if (ido != 2) {
        for (k = 0; k < l1ido; k += ido) {
            const v4sf *RESTRICT pc = (v4sf *)(cc - 1 + 4 * k);
            v4sf *RESTRICT ph = (v4sf *)(ch + k + 1);
            for (i = 2; i < ido; i += 2) {

                tr1 = VSUB(pc[i], pc[4 * ido - i]);
                tr2 = VADD(pc[i], pc[4 * ido - i]);
                ti4 = VSUB(pc[2 * ido + i], pc[2 * ido - i]);
                tr3 = VADD(pc[2 * ido + i], pc[2 * ido - i]);
                ph[0] = VADD(tr2, tr3);
                cr3 = VSUB(tr2, tr3);

                ti3 = VSUB(pc[2 * ido + i + 1], pc[2 * ido - i + 1]);
                tr4 = VADD(pc[2 * ido + i + 1], pc[2 * ido - i + 1]);
                cr2 = VSUB(tr1, tr4);
                cr4 = VADD(tr1, tr4);

                ti1 = VADD(pc[i + 1], pc[4 * ido - i + 1]);
                ti2 = VSUB(pc[i + 1], pc[4 * ido - i + 1]);

                ph[1] = VADD(ti2, ti3);
                ph += l1ido;
                ci3 = VSUB(ti2, ti3);
                ci2 = VADD(ti1, ti4);
                ci4 = VSUB(ti1, ti4);
                VCPLXMUL(cr2, ci2, LD_PS1(wa1[i - 2]), LD_PS1(wa1[i - 1]));
                ph[0] = cr2;
                ph[1] = ci2;
                ph += l1ido;
                VCPLXMUL(cr3, ci3, LD_PS1(wa2[i - 2]), LD_PS1(wa2[i - 1]));
                ph[0] = cr3;
                ph[1] = ci3;
                ph += l1ido;
                VCPLXMUL(cr4, ci4, LD_PS1(wa3[i - 2]), LD_PS1(wa3[i - 1]));
                ph[0] = cr4;
                ph[1] = ci4;
                ph = ph - 3 * l1ido + 2;
            }
        }
        if (ido % 2 == 1)
            return;
    }
    for (k = 0; k < l1ido; k += ido) {
        int i0 = 4 * k + ido;
        v4sf c = cc[i0 - 1], d = cc[i0 + 2 * ido - 1];
        v4sf a = cc[i0 + 0], b = cc[i0 + 2 * ido + 0];
        tr1 = VSUB(c, d);
        tr2 = VADD(c, d);
        ti1 = VADD(b, a);
        ti2 = VSUB(b, a);
        ch[ido - 1 + k + 0 * l1ido] = VADD(tr2, tr2);
        ch[ido - 1 + k + 1 * l1ido] = SVMUL(minus_sqrt2, VSUB(ti1, tr1));
        ch[ido - 1 + k + 2 * l1ido] = VADD(ti2, ti2);
        ch[ido - 1 + k + 3 * l1ido] = SVMUL(minus_sqrt2, VADD(ti1, tr1));
    }
} /* radb4 */

static void radf5_ps(int ido, int l1, const v4sf *RESTRICT cc, v4sf *RESTRICT ch,
                     const float *wa1, const float *wa2, const float *wa3, const float *wa4) {
    static const float tr11 = .309016994374947f;
    static const float ti11 = .951056516295154f;
    static const float tr12 = -.809016994374947f;
    static const float ti12 = .587785252292473f;

    /* System generated locals */
    int cc_offset, ch_offset;

    /* Local variables */
    int i, k, ic;
    v4sf ci2, di2, ci4, ci5, di3, di4, di5, ci3, cr2, cr3, dr2, dr3, dr4, dr5,
        cr5, cr4, ti2, ti3, ti5, ti4, tr2, tr3, tr4, tr5;
    int idp2;

#define cc_ref(a_1, a_2, a_3) cc[((a_3)*l1 + (a_2)) * ido + a_1]
#define ch_ref(a_1, a_2, a_3) ch[((a_3)*5 + (a_2)) * ido + a_1]

    /* Parameter adjustments */
    ch_offset = 1 + ido * 6;
    ch -= ch_offset;
    cc_offset = 1 + ido * (1 + l1);
    cc -= cc_offset;

    /* Function Body */
    for (k = 1; k <= l1; ++k) {
        cr2 = VADD(cc_ref(1, k, 5), cc_ref(1, k, 2));
        ci5 = VSUB(cc_ref(1, k, 5), cc_ref(1, k, 2));
        cr3 = VADD(cc_ref(1, k, 4), cc_ref(1, k, 3));
        ci4 = VSUB(cc_ref(1, k, 4), cc_ref(1, k, 3));
        ch_ref(1, 1, k) = VADD(cc_ref(1, k, 1), VADD(cr2, cr3));
        ch_ref(ido, 2, k) = VADD(cc_ref(1, k, 1), VADD(SVMUL(tr11, cr2), SVMUL(tr12, cr3)));
        ch_ref(1, 3, k) = VADD(SVMUL(ti11, ci5), SVMUL(ti12, ci4));
        ch_ref(ido, 4, k) = VADD(cc_ref(1, k, 1), VADD(SVMUL(tr12, cr2), SVMUL(tr11, cr3)));
        ch_ref(1, 5, k) = VSUB(SVMUL(ti12, ci5), SVMUL(ti11, ci4));
        //printf("pffft: radf5, k=%d ch_ref=%f, ci4=%f\n", k, ch_ref(1, 5, k), ci4);
    }
    if (ido == 1) {
        return;
    }
    idp2 = ido + 2;
    for (k = 1; k <= l1; ++k) {
        for (i = 3; i <= ido; i += 2) {
            ic = idp2 - i;
            dr2 = LD_PS1(wa1[i - 3]);
            di2 = LD_PS1(wa1[i - 2]);
            dr3 = LD_PS1(wa2[i - 3]);
            di3 = LD_PS1(wa2[i - 2]);
            dr4 = LD_PS1(wa3[i - 3]);
            di4 = LD_PS1(wa3[i - 2]);
            dr5 = LD_PS1(wa4[i - 3]);
            di5 = LD_PS1(wa4[i - 2]);
            VCPLXMULCONJ(dr2, di2, cc_ref(i - 1, k, 2), cc_ref(i, k, 2));
            VCPLXMULCONJ(dr3, di3, cc_ref(i - 1, k, 3), cc_ref(i, k, 3));
            VCPLXMULCONJ(dr4, di4, cc_ref(i - 1, k, 4), cc_ref(i, k, 4));
            VCPLXMULCONJ(dr5, di5, cc_ref(i - 1, k, 5), cc_ref(i, k, 5));
            cr2 = VADD(dr2, dr5);
            ci5 = VSUB(dr5, dr2);
            cr5 = VSUB(di2, di5);
            ci2 = VADD(di2, di5);
            cr3 = VADD(dr3, dr4);
            ci4 = VSUB(dr4, dr3);
            cr4 = VSUB(di3, di4);
            ci3 = VADD(di3, di4);
            ch_ref(i - 1, 1, k) = VADD(cc_ref(i - 1, k, 1), VADD(cr2, cr3));
            ch_ref(i, 1, k) = VSUB(cc_ref(i, k, 1), VADD(ci2, ci3)); //
            tr2 = VADD(cc_ref(i - 1, k, 1), VADD(SVMUL(tr11, cr2), SVMUL(tr12, cr3)));
            ti2 = VSUB(cc_ref(i, k, 1), VADD(SVMUL(tr11, ci2), SVMUL(tr12, ci3))); //
            tr3 = VADD(cc_ref(i - 1, k, 1), VADD(SVMUL(tr12, cr2), SVMUL(tr11, cr3)));
            ti3 = VSUB(cc_ref(i, k, 1), VADD(SVMUL(tr12, ci2), SVMUL(tr11, ci3))); //
            tr5 = VADD(SVMUL(ti11, cr5), SVMUL(ti12, cr4));
            ti5 = VADD(SVMUL(ti11, ci5), SVMUL(ti12, ci4));
            tr4 = VSUB(SVMUL(ti12, cr5), SVMUL(ti11, cr4));
            ti4 = VSUB(SVMUL(ti12, ci5), SVMUL(ti11, ci4));
            ch_ref(i - 1, 3, k) = VSUB(tr2, tr5);
            ch_ref(ic - 1, 2, k) = VADD(tr2, tr5);
            ch_ref(i, 3, k) = VADD(ti2, ti5);
            ch_ref(ic, 2, k) = VSUB(ti5, ti2);
            ch_ref(i - 1, 5, k) = VSUB(tr3, tr4);
            ch_ref(ic - 1, 4, k) = VADD(tr3, tr4);
            ch_ref(i, 5, k) = VADD(ti3, ti4);
            ch_ref(ic, 4, k) = VSUB(ti4, ti3);
        }
    }
#undef cc_ref
#undef ch_ref
} /* radf5 */

static void radb5_ps(int ido, int l1, const v4sf *RESTRICT cc, v4sf *RESTRICT ch,
                     const float *wa1, const float *wa2, const float *wa3, const float *wa4) {
    static const float tr11 = .309016994374947f;
    static const float ti11 = .951056516295154f;
    static const float tr12 = -.809016994374947f;
    static const float ti12 = .587785252292473f;

    int cc_offset, ch_offset;

    /* Local variables */
    int i, k, ic;
    v4sf ci2, ci3, ci4, ci5, di3, di4, di5, di2, cr2, cr3, cr5, cr4, ti2, ti3,
        ti4, ti5, dr3, dr4, dr5, dr2, tr2, tr3, tr4, tr5;
    int idp2;

#define cc_ref(a_1, a_2, a_3) cc[((a_3)*5 + (a_2)) * ido + a_1]
#define ch_ref(a_1, a_2, a_3) ch[((a_3)*l1 + (a_2)) * ido + a_1]

    /* Parameter adjustments */
    ch_offset = 1 + ido * (1 + l1);
    ch -= ch_offset;
    cc_offset = 1 + ido * 6;
    cc -= cc_offset;

    /* Function Body */
    for (k = 1; k <= l1; ++k) {
        ti5 = VADD(cc_ref(1, 3, k), cc_ref(1, 3, k));
        ti4 = VADD(cc_ref(1, 5, k), cc_ref(1, 5, k));
        tr2 = VADD(cc_ref(ido, 2, k), cc_ref(ido, 2, k));
        tr3 = VADD(cc_ref(ido, 4, k), cc_ref(ido, 4, k));
        ch_ref(1, k, 1) = VADD(cc_ref(1, 1, k), VADD(tr2, tr3));
        cr2 = VADD(cc_ref(1, 1, k), VADD(SVMUL(tr11, tr2), SVMUL(tr12, tr3)));
        cr3 = VADD(cc_ref(1, 1, k), VADD(SVMUL(tr12, tr2), SVMUL(tr11, tr3)));
        ci5 = VADD(SVMUL(ti11, ti5), SVMUL(ti12, ti4));
        ci4 = VSUB(SVMUL(ti12, ti5), SVMUL(ti11, ti4));
        ch_ref(1, k, 2) = VSUB(cr2, ci5);
        ch_ref(1, k, 3) = VSUB(cr3, ci4);
        ch_ref(1, k, 4) = VADD(cr3, ci4);
        ch_ref(1, k, 5) = VADD(cr2, ci5);
    }
    if (ido == 1) {
        return;
    }
    idp2 = ido + 2;
    for (k = 1; k <= l1; ++k) {
        for (i = 3; i <= ido; i += 2) {
            ic = idp2 - i;
            ti5 = VADD(cc_ref(i, 3, k), cc_ref(ic, 2, k));
            ti2 = VSUB(cc_ref(i, 3, k), cc_ref(ic, 2, k));
            ti4 = VADD(cc_ref(i, 5, k), cc_ref(ic, 4, k));
            ti3 = VSUB(cc_ref(i, 5, k), cc_ref(ic, 4, k));
            tr5 = VSUB(cc_ref(i - 1, 3, k), cc_ref(ic - 1, 2, k));
            tr2 = VADD(cc_ref(i - 1, 3, k), cc_ref(ic - 1, 2, k));
            tr4 = VSUB(cc_ref(i - 1, 5, k), cc_ref(ic - 1, 4, k));
            tr3 = VADD(cc_ref(i - 1, 5, k), cc_ref(ic - 1, 4, k));
            ch_ref(i - 1, k, 1) = VADD(cc_ref(i - 1, 1, k), VADD(tr2, tr3));
            ch_ref(i, k, 1) = VADD(cc_ref(i, 1, k), VADD(ti2, ti3));
            cr2 = VADD(cc_ref(i - 1, 1, k), VADD(SVMUL(tr11, tr2), SVMUL(tr12, tr3)));
            ci2 = VADD(cc_ref(i, 1, k), VADD(SVMUL(tr11, ti2), SVMUL(tr12, ti3)));
            cr3 = VADD(cc_ref(i - 1, 1, k), VADD(SVMUL(tr12, tr2), SVMUL(tr11, tr3)));
            ci3 = VADD(cc_ref(i, 1, k), VADD(SVMUL(tr12, ti2), SVMUL(tr11, ti3)));
            cr5 = VADD(SVMUL(ti11, tr5), SVMUL(ti12, tr4));
            ci5 = VADD(SVMUL(ti11, ti5), SVMUL(ti12, ti4));
            cr4 = VSUB(SVMUL(ti12, tr5), SVMUL(ti11, tr4));
            ci4 = VSUB(SVMUL(ti12, ti5), SVMUL(ti11, ti4));
            dr3 = VSUB(cr3, ci4);
            dr4 = VADD(cr3, ci4);
            di3 = VADD(ci3, cr4);
            di4 = VSUB(ci3, cr4);
            dr5 = VADD(cr2, ci5);
            dr2 = VSUB(cr2, ci5);
            di5 = VSUB(ci2, cr5);
            di2 = VADD(ci2, cr5);
            VCPLXMUL(dr2, di2, LD_PS1(wa1[i - 3]), LD_PS1(wa1[i - 2]));
            VCPLXMUL(dr3, di3, LD_PS1(wa2[i - 3]), LD_PS1(wa2[i - 2]));
            VCPLXMUL(dr4, di4, LD_PS1(wa3[i - 3]), LD_PS1(wa3[i - 2]));
            VCPLXMUL(dr5, di5, LD_PS1(wa4[i - 3]), LD_PS1(wa4[i - 2]));

            ch_ref(i - 1, k, 2) = dr2;
            ch_ref(i, k, 2) = di2;
            ch_ref(i - 1, k, 3) = dr3;
            ch_ref(i, k, 3) = di3;
            ch_ref(i - 1, k, 4) = dr4;
            ch_ref(i, k, 4) = di4;
            ch_ref(i - 1, k, 5) = dr5;
            ch_ref(i, k, 5) = di5;
        }
    }
#undef cc_ref
#undef ch_ref
} /* radb5 */

static NEVER_INLINE(v4sf *) rfftf1_ps(int n, const v4sf *input_readonly, v4sf *work1, v4sf *work2,
                                      const float *wa, const int *ifac) {
    v4sf *in = (v4sf *)input_readonly;
    v4sf *out = (in == work2 ? work1 : work2);
    int nf = ifac[1], k1;
    int l2 = n;
    int iw = n - 1;
    SWAN_ASSERT(in != out && work1 != work2)
    for (k1 = 1; k1 <= nf; ++k1) {
        int kh = nf - k1;
        int ip = ifac[kh + 2];
        int l1 = l2 / ip;
        int ido = n / l2;
        iw -= (ip - 1) * ido;
        switch (ip) {
        case 5: {
            int ix2 = iw + ido;
            int ix3 = ix2 + ido;
            int ix4 = ix3 + ido;
            radf5_ps(ido, l1, in, out, &wa[iw], &wa[ix2], &wa[ix3], &wa[ix4]);
        } break;
        case 4: {
            int ix2 = iw + ido;
            int ix3 = ix2 + ido;
            radf4_ps(ido, l1, in, out, &wa[iw], &wa[ix2], &wa[ix3]);
        } break;
        case 3: {
            int ix2 = iw + ido;
            radf3_ps(ido, l1, in, out, &wa[iw], &wa[ix2]);
        } break;
        case 2:
            radf2_ps(ido, l1, in, out, &wa[iw]);
            break;
        default:
            SWAN_ASSERT(0)
            break;
        }
        l2 = l1;
        if (out == work2) {
            out = work1;
            in = work2;
        } else {
            out = work2;
            in = work1;
        }
    }
    return in; /* this is in fact the output .. */
} /* rfftf1 */

static NEVER_INLINE(v4sf *) rfftb1_ps(int n, const v4sf *input_readonly, v4sf *work1, v4sf *work2,
                                      const float *wa, const int *ifac) {
    v4sf *in = (v4sf *)input_readonly;
    v4sf *out = (in == work2 ? work1 : work2);
    int nf = ifac[1], k1;
    int l1 = 1;
    int iw = 0;
    SWAN_ASSERT(in != out)
    for (k1 = 1; k1 <= nf; k1++) {
        int ip = ifac[k1 + 1];
        int l2 = ip * l1;
        int ido = n / l2;
        switch (ip) {
        case 5: {
            int ix2 = iw + ido;
            int ix3 = ix2 + ido;
            int ix4 = ix3 + ido;
            radb5_ps(ido, l1, in, out, &wa[iw], &wa[ix2], &wa[ix3], &wa[ix4]);
        } break;
        case 4: {
            int ix2 = iw + ido;
            int ix3 = ix2 + ido;
            radb4_ps(ido, l1, in, out, &wa[iw], &wa[ix2], &wa[ix3]);
        } break;
        case 3: {
            int ix2 = iw + ido;
            radb3_ps(ido, l1, in, out, &wa[iw], &wa[ix2]);
        } break;
        case 2:
            radb2_ps(ido, l1, in, out, &wa[iw]);
            break;
        default:
            SWAN_ASSERT(0)
            break;
        }
        l1 = l2;
        iw += (ip - 1) * ido;

        if (out == work2) {
            out = work1;
            in = work2;
        } else {
            out = work2;
            in = work1;
        }
    }
    return in; /* this is in fact the output .. */
}

v4sf *cfftf1_ps(int n, const v4sf *input_readonly, v4sf *work1, v4sf *work2, const float *wa, const int *ifac, int isign) {
    v4sf *in = (v4sf *)input_readonly;
    v4sf *out = (in == work2 ? work1 : work2);
    int nf = ifac[1], k1;
    int l1 = 1;
    int iw = 0;
    SWAN_ASSERT(in != out && work1 != work2)
    for (k1 = 2; k1 <= nf + 1; k1++) {
        int ip = ifac[k1];
        int l2 = ip * l1;
        int ido = n / l2;
        int idot = ido + ido;
        switch (ip) {
        case 5: {
            int ix2 = iw + idot;
            int ix3 = ix2 + idot;
            int ix4 = ix3 + idot;
            passf5_ps(idot, l1, in, out, &wa[iw], &wa[ix2], &wa[ix3], &wa[ix4], isign);
        } break;
        case 4: {
            int ix2 = iw + idot;
            int ix3 = ix2 + idot;
            passf4_ps(idot, l1, in, out, &wa[iw], &wa[ix2], &wa[ix3], isign);
        } break;
        case 2: {
            passf2_ps(idot, l1, in, out, &wa[iw], isign);
        } break;
        case 3: {
            int ix2 = iw + idot;
            passf3_ps(idot, l1, in, out, &wa[iw], &wa[ix2], isign);
        } break;
        default:
            SWAN_ASSERT(0)
        }
        l1 = l2;
        iw += (ip - 1) * idot;
        if (out == work2) {
            out = work1;
            in = work2;
        } else {
            out = work2;
            in = work1;
        }
    }

    return in; /* this is in fact the output .. */
}

// standard routine using scalar floats, without SIMD stuff.

void pffft_zreorder_scalar(PFFFT_Setup *setup, const float *in, float *out, pffft_direction_t direction) {
    int k, N = setup->N;
    if (setup->transform == PFFFT_COMPLEX) {
        for (k = 0; k < 2 * N; ++k)
            out[k] = in[k];
        return;
    } else if (direction == PFFFT_FORWARD) {
        float x_N = in[N - 1];
        for (k = N - 1; k > 1; --k)
            out[k] = in[k - 1];
        out[0] = in[0];
        out[1] = x_N;
    } else {
        float x_N = in[1];
        for (k = 1; k < N - 1; ++k)
            out[k] = in[k + 1];
        out[0] = in[0];
        out[N - 1] = x_N;
    }
}

void pffft_transform_internal(PFFFT_Setup *setup, const float *input, float *output, float *scratch,
                              pffft_direction_t direction, int ordered) {
    int Ncvec = setup->Ncvec;
    int nf_odd = (setup->ifac[1] & 1);

    // temporary buffer is allocated on the stack if the scratch pointer is NULL
    float *buff[2];
    int ib;
    buff[0] = output;
    buff[1] = scratch;

    if (setup->transform == PFFFT_COMPLEX)
        ordered = 0; // it is always ordered.
    ib = (nf_odd ^ ordered ? 1 : 0);

    if (direction == PFFFT_FORWARD) {
        if (setup->transform == PFFFT_REAL) {
            ib = (rfftf1_ps(Ncvec * 2, input, buff[ib], buff[!ib],
                            setup->twiddle, &setup->ifac[0]) == buff[0]
                      ? 0
                      : 1);
        } else {
            ib = (cfftf1_ps(Ncvec, input, buff[ib], buff[!ib],
                            setup->twiddle, &setup->ifac[0], -1) == buff[0]
                      ? 0
                      : 1);
        }
        if (ordered) {
            pffft_zreorder_scalar(setup, buff[ib], buff[!ib], PFFFT_FORWARD);
            ib = !ib;
        }
    } else {
        if (input == buff[ib]) {
            ib = !ib; // may happen when finput == foutput
        }
        if (ordered) {
            pffft_zreorder_scalar(setup, input, buff[!ib], PFFFT_BACKWARD);
            input = buff[!ib];
        }
        if (setup->transform == PFFFT_REAL) {
            ib = (rfftb1_ps(Ncvec * 2, input, buff[ib], buff[!ib],
                            setup->twiddle, &setup->ifac[0]) == buff[0]
                      ? 0
                      : 1);
        } else {
            ib = (cfftf1_ps(Ncvec, input, buff[ib], buff[!ib],
                            setup->twiddle, &setup->ifac[0], +1) == buff[0]
                      ? 0
                      : 1);
        }
    }
    if (buff[ib] != output) {
        int k;
        // extra copy required -- this situation should happens only when finput == foutput
        SWAN_ASSERT(input == output)
        for (k = 0; k < Ncvec; ++k) {
            float a = buff[ib][2 * k], b = buff[ib][2 * k + 1];
            output[2 * k] = a;
            output[2 * k + 1] = b;
        }
        ib = !ib;
    }
    SWAN_ASSERT(buff[ib] == output)
}

void pffft_transform_scalar(PFFFT_Setup *setup, const float *input, float *output, float *work, pffft_direction_t direction) {
    pffft_transform_internal(setup, input, output, (v4sf *)work, direction, 0);
}

void pffft_transform_scalar_ordered(PFFFT_Setup *setup, const float *input, float *output, float *work, pffft_direction_t direction) {
    pffft_transform_internal(setup, input, output, (v4sf *)work, direction, 1);
}

void pffft_scalar(config_t *config,
                  input_t *input,
                  output_t *output) {

    pffft_config_t *pffft_config = (pffft_config_t *)config;
    pffft_input_t *pffft_input = (pffft_input_t *)input;
    pffft_output_t *pffft_output = (pffft_output_t *)output;

    PFFFT_Setup *setup = pffft_config->scalar_setup;
    pffft_direction_t direction = pffft_config->direction;
    float *work = pffft_input->work;

    uint32_t block_count = pffft_config->block_count;

    for (int block = 0; block < block_count; block++) {
        const float *input_buff = pffft_input->input_buff[block];

        float *output_buff = pffft_output->output_buff[block];

        pffft_transform_scalar_ordered(setup, input_buff, output_buff, work, direction);
    }
}

#undef v4sf
#undef SIMD_SZ
#undef VZERO
#undef VMUL
#undef VADD
#undef VMADD
#undef VSUB
#undef LD_PS1
#undef VALIGNED
#undef VCPLXMUL
#undef VCPLXMULCONJ
#undef SVMUL