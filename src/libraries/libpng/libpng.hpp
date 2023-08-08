/* png.h - header file for PNG reference library
 *
 * libpng version 1.6.37 - April 14, 2019
 *
 * Copyright (c) 2018-2019 Cosmin Truta
 * Copyright (c) 1998-2002,2004,2006-2018 Glenn Randers-Pehrson
 * Copyright (c) 1996-1997 Andreas Dilger
 * Copyright (c) 1995-1996 Guy Eric Schalnat, Group 42, Inc.
 *
 * This code is released under the libpng license. (See LICENSE, below.)
 *
 * Authors and maintainers:
 *   libpng versions 0.71, May 1995, through 0.88, January 1996: Guy Schalnat
 *   libpng versions 0.89, June 1996, through 0.96, May 1997: Andreas Dilger
 *   libpng versions 0.97, January 1998, through 1.6.35, July 2018:
 *     Glenn Randers-Pehrson
 *   libpng versions 1.6.36, December 2018, through 1.6.37, April 2019:
 *     Cosmin Truta
 *   See also "Contributing Authors", below.
 */

/*
 * COPYRIGHT NOTICE, DISCLAIMER, and LICENSE
 * =========================================
 *
 * PNG Reference Library License version 2
 * ---------------------------------------
 *
 *  * Copyright (c) 1995-2019 The PNG Reference Library Authors.
 *  * Copyright (c) 2018-2019 Cosmin Truta.
 *  * Copyright (c) 2000-2002, 2004, 2006-2018 Glenn Randers-Pehrson.
 *  * Copyright (c) 1996-1997 Andreas Dilger.
 *  * Copyright (c) 1995-1996 Guy Eric Schalnat, Group 42, Inc.
 *
 * The software is supplied "as is", without warranty of any kind,
 * express or implied, including, without limitation, the warranties
 * of merchantability, fitness for a particular purpose, title, and
 * non-infringement.  In no event shall the Copyright owners, or
 * anyone distributing the software, be liable for any damages or
 * other liability, whether in contract, tort or otherwise, arising
 * from, out of, or in connection with the software, or the use or
 * other dealings in the software, even if advised of the possibility
 * of such damage.
 *
 * Permission is hereby granted to use, copy, modify, and distribute
 * this software, or portions hereof, for any purpose, without fee,
 * subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you
 *     must not claim that you wrote the original software.  If you
 *     use this software in a product, an acknowledgment in the product
 *     documentation would be appreciated, but is not required.
 *
 *  2. Altered source versions must be plainly marked as such, and must
 *     not be misrepresented as being the original software.
 *
 *  3. This Copyright notice may not be removed or altered from any
 *     source or altered source distribution.
 *
 *
 * PNG Reference Library License version 1 (for libpng 0.5 through 1.6.35)
 * -----------------------------------------------------------------------
 *
 * libpng versions 1.0.7, July 1, 2000, through 1.6.35, July 15, 2018 are
 * Copyright (c) 2000-2002, 2004, 2006-2018 Glenn Randers-Pehrson, are
 * derived from libpng-1.0.6, and are distributed according to the same
 * disclaimer and license as libpng-1.0.6 with the following individuals
 * added to the list of Contributing Authors:
 *
 *     Simon-Pierre Cadieux
 *     Eric S. Raymond
 *     Mans Rullgard
 *     Cosmin Truta
 *     Gilles Vollant
 *     James Yu
 *     Mandar Sahastrabuddhe
 *     Google Inc.
 *     Vadim Barkov
 *
 * and with the following additions to the disclaimer:
 *
 *     There is no warranty against interference with your enjoyment of
 *     the library or against infringement.  There is no warranty that our
 *     efforts or the library will fulfill any of your particular purposes
 *     or needs.  This library is provided with all faults, and the entire
 *     risk of satisfactory quality, performance, accuracy, and effort is
 *     with the user.
 *
 * Some files in the "contrib" directory and some configure-generated
 * files that are distributed with libpng have other copyright owners, and
 * are released under other open source licenses.
 *
 * libpng versions 0.97, January 1998, through 1.0.6, March 20, 2000, are
 * Copyright (c) 1998-2000 Glenn Randers-Pehrson, are derived from
 * libpng-0.96, and are distributed according to the same disclaimer and
 * license as libpng-0.96, with the following individuals added to the
 * list of Contributing Authors:
 *
 *     Tom Lane
 *     Glenn Randers-Pehrson
 *     Willem van Schaik
 *
 * libpng versions 0.89, June 1996, through 0.96, May 1997, are
 * Copyright (c) 1996-1997 Andreas Dilger, are derived from libpng-0.88,
 * and are distributed according to the same disclaimer and license as
 * libpng-0.88, with the following individuals added to the list of
 * Contributing Authors:
 *
 *     John Bowler
 *     Kevin Bracey
 *     Sam Bushell
 *     Magnus Holmgren
 *     Greg Roelofs
 *     Tom Tanner
 *
 * Some files in the "scripts" directory have other copyright owners,
 * but are released under this license.
 *
 * libpng versions 0.5, May 1995, through 0.88, January 1996, are
 * Copyright (c) 1995-1996 Guy Eric Schalnat, Group 42, Inc.
 *
 * For the purposes of this copyright and license, "Contributing Authors"
 * is defined as the following set of individuals:
 *
 *     Andreas Dilger
 *     Dave Martindale
 *     Guy Eric Schalnat
 *     Paul Schmidt
 *     Tim Wegner
 *
 * The PNG Reference Library is supplied "AS IS".  The Contributing
 * Authors and Group 42, Inc. disclaim all warranties, expressed or
 * implied, including, without limitation, the warranties of
 * merchantability and of fitness for any purpose.  The Contributing
 * Authors and Group 42, Inc. assume no liability for direct, indirect,
 * incidental, special, exemplary, or consequential damages, which may
 * result from the use of the PNG Reference Library, even if advised of
 * the possibility of such damage.
 *
 * Permission is hereby granted to use, copy, modify, and distribute this
 * source code, or portions hereof, for any purpose, without fee, subject
 * to the following restrictions:
 *
 *  1. The origin of this source code must not be misrepresented.
 *
 *  2. Altered versions must be plainly marked as such and must not
 *     be misrepresented as being the original source.
 *
 *  3. This Copyright notice may not be removed or altered from any
 *     source or altered source distribution.
 *
 * The Contributing Authors and Group 42, Inc. specifically permit,
 * without fee, and encourage the use of this source code as a component
 * to supporting the PNG file format in commercial products.  If you use
 * this source code in a product, acknowledgment is not required but would
 * be appreciated.
 *
 * END OF COPYRIGHT NOTICE, DISCLAIMER, and LICENSE.
 *
 * TRADEMARK
 * =========
 *
 * The name "libpng" has not been registered by the Copyright owners
 * as a trademark in any jurisdiction.  However, because libpng has
 * been distributed and maintained world-wide, continually since 1995,
 * the Copyright owners claim "common-law trademark protection" in any
 * jurisdiction where common-law trademark is recognized.
 */

/*
 * A "png_get_copyright" function is available, for convenient use in "about"
 * boxes and the like:
 *
 *    printf("%s", png_get_copyright(NULL));
 *
 * Also, the PNG logo (in PNG format, of course) is supplied in the
 * files "pngbar.png" and "pngbar.jpg (88x31) and "pngnow.png" (98x31).
 */

/*
 * The contributing authors would like to thank all those who helped
 * with testing, bug fixes, and patience.  This wouldn't have been
 * possible without all of you.
 *
 * Thanks to Frank J. T. Wojcik for helping with the documentation.
 */

/* Note about libpng version numbers:
 *
 *    Due to various miscommunications, unforeseen code incompatibilities
 *    and occasional factors outside the authors' control, version numbering
 *    on the library has not always been consistent and straightforward.
 *    The following table summarizes matters since version 0.89c, which was
 *    the first widely used release:
 *
 *    source                 png.h  png.h  shared-lib
 *    version                string   int  version
 *    -------                ------ -----  ----------
 *    0.89c "1.0 beta 3"     0.89      89  1.0.89
 *    0.90  "1.0 beta 4"     0.90      90  0.90  [should have been 2.0.90]
 *    0.95  "1.0 beta 5"     0.95      95  0.95  [should have been 2.0.95]
 *    0.96  "1.0 beta 6"     0.96      96  0.96  [should have been 2.0.96]
 *    0.97b "1.00.97 beta 7" 1.00.97   97  1.0.1 [should have been 2.0.97]
 *    0.97c                  0.97      97  2.0.97
 *    0.98                   0.98      98  2.0.98
 *    0.99                   0.99      98  2.0.99
 *    0.99a-m                0.99      99  2.0.99
 *    1.00                   1.00     100  2.1.0 [100 should be 10000]
 *    1.0.0      (from here on, the   100  2.1.0 [100 should be 10000]
 *    1.0.1       png.h string is   10001  2.1.0
 *    1.0.1a-e    identical to the  10002  from here on, the shared library
 *    1.0.2       source version)   10002  is 2.V where V is the source code
 *    1.0.2a-b                      10003  version, except as noted.
 *    1.0.3                         10003
 *    1.0.3a-d                      10004
 *    1.0.4                         10004
 *    1.0.4a-f                      10005
 *    1.0.5 (+ 2 patches)           10005
 *    1.0.5a-d                      10006
 *    1.0.5e-r                      10100 (not source compatible)
 *    1.0.5s-v                      10006 (not binary compatible)
 *    1.0.6 (+ 3 patches)           10006 (still binary incompatible)
 *    1.0.6d-f                      10007 (still binary incompatible)
 *    1.0.6g                        10007
 *    1.0.6h                        10007  10.6h (testing xy.z so-numbering)
 *    1.0.6i                        10007  10.6i
 *    1.0.6j                        10007  2.1.0.6j (incompatible with 1.0.0)
 *    1.0.7beta11-14        DLLNUM  10007  2.1.0.7beta11-14 (binary compatible)
 *    1.0.7beta15-18           1    10007  2.1.0.7beta15-18 (binary compatible)
 *    1.0.7rc1-2               1    10007  2.1.0.7rc1-2 (binary compatible)
 *    1.0.7                    1    10007  (still compatible)
 *    ...
 *    1.0.69                  10    10069  10.so.0.69[.0]
 *    ...
 *    1.2.59                  13    10259  12.so.0.59[.0]
 *    ...
 *    1.4.20                  14    10420  14.so.0.20[.0]
 *    ...
 *    1.5.30                  15    10530  15.so.15.30[.0]
 *    ...
 *    1.6.37                  16    10637  16.so.16.37[.0]
 *
 *    Henceforth the source version will match the shared-library major and
 *    minor numbers; the shared-library major version number will be used for
 *    changes in backward compatibility, as it is intended.
 *    The PNG_LIBPNG_VER macro, which is not used within libpng but is
 *    available for applications, is an unsigned integer of the form XYYZZ
 *    corresponding to the source version X.Y.Z (leading zeros in Y and Z).
 *    Beta versions were given the previous public release number plus a
 *    letter, until version 1.0.6j; from then on they were given the upcoming
 *    public release number plus "betaNN" or "rcNN".
 *
 *    Binary incompatibility exists only when applications make direct access
 *    to the info_ptr or png_ptr members through png.h, and the compiled
 *    application is loaded with a different version of the library.
 *
 *    DLLNUM will change each time there are forward or backward changes
 *    in binary compatibility (e.g., when a new feature is added).
 *
 * See libpng.txt or libpng.3 for more information.  The PNG specification
 * is available as a W3C Recommendation and as an ISO/IEC Standard; see
 * <https://www.w3.org/TR/2003/REC-PNG-20031110/>
 */

#ifndef A027C2A0_39D2_45C4_A2AA_098B8EB820A1
#define A027C2A0_39D2_45C4_A2AA_098B8EB820A1

#define png_byte unsigned char
#define png_bytep unsigned char *
#define png_bytepp unsigned char **
#define png_uint_32 unsigned int
#define png_int_32 int

#define png_aligncastconst(type, value) \
    static_cast<type>(static_cast<const void *>(value))

#define png_aligncast(type, value) \
    static_cast<type>(static_cast<void *>(value))

#define png_ptrc(type, pointer) png_aligncastconst(const type *, pointer)

#define png_ptr(type, pointer) png_aligncast(type *, pointer)

#define png_ldr(type, pointer) \
    (temp_pointer = png_ptr(type, pointer), *temp_pointer)

#include "swan.hpp"

#endif /* A027C2A0_39D2_45C4_A2AA_098B8EB820A1 */
