/*
  Copyright (C) 1995-2022 Jean-loup Gailly and Mark Adler

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Jean-loup Gailly        Mark Adler
  jloup@gzip.org          madler@alumni.caltech.edu


  The data format used by the zlib library is described by RFCs (Request for
  Comments) 1950 to 1952 in the files http://tools.ietf.org/html/rfc1950
  (zlib format), rfc1951 (deflate format) and rfc1952 (gzip format).
*/

#include "scalar.hpp"
#include "adler32.hpp"

/* The following function is the modified version of adler32_z,
 * provided in the webaudio library. Please refer to adler32.c
 * for the unmodified version in the source library. */
void adler32_scalar(config_t *config,
                    input_t *input,
                    output_t *output) {
    adler32_config_t *adler32_config = (adler32_config_t *)config;
    adler32_input_t *adler32_input = (adler32_input_t *)input;
    adler32_output_t *adler32_output = (adler32_output_t *)output;

    uLong adler = adler32_config->adler;
    const Bytef *buf = adler32_input->buf;
    z_size_t len = adler32_config->len;

    unsigned long sum2;
    unsigned n;

    /* split Adler-32 into component sums */
    sum2 = (adler >> 16) & 0xffff;
    adler &= 0xffff;

    /* do length NMAX blocks -- requires just one modulo operation */
    while (len >= NMAX) {
        len -= NMAX;
        n = NMAX / 16; /* NMAX is divisible by 16 */
        do {
            DO16(buf); /* 16 sums unrolled */
            buf += 16;
        } while (--n);
        MOD(adler);
        MOD(sum2);
    }

    /* do remaining bytes (less than NMAX, still just one modulo) */
    if (len) { /* avoid modulos if none remaining */
        while (len >= 16) {
            len -= 16;
            DO16(buf);
            buf += 16;
        }
        while (len--) {
            adler += *buf++;
            sum2 += adler;
        }
        MOD(adler);
        MOD(sum2);
    }

    /* return recombined sums */
    adler32_output->return_value[0] = adler | (sum2 << 16);
}