#ifndef AB273D1B_1B52_4C9B_9783_9D7BF54DC8D1
#define AB273D1B_1B52_4C9B_9783_9D7BF54DC8D1
unsigned char ClampTo8(int a);
/** Returns alpha byte from color value.
*/
#define SkColorGetA(color) (((color) >> 24) & 0xFF)

/** Returns red component of color, from zero to 255.
*/
#define SkColorGetR(color) (((color) >> 16) & 0xFF)

/** Returns green component of color, from zero to 255.
*/
#define SkColorGetG(color) (((color) >> 8) & 0xFF)

/** Returns blue component of color, from zero to 255.
*/
#define SkColorGetB(color) (((color) >> 0) & 0xFF)

////////////////////////////////////////////////////////////////////////////////////////////
// Convert a 16bit pixel to a 32bit pixel

#define SK_R16_BITS 5
#define SK_G16_BITS 6
#define SK_B16_BITS 5

#define SK_R16_SHIFT 11
#define SK_G16_SHIFT 5
#define SK_B16_SHIFT 0

#define SK_R16_MASK 31
#define SK_G16_MASK 63
#define SK_B16_MASK 31

#define SkGetPackedR16(color) (((unsigned)(color) >> 11) & 31)
#define SkGetPackedG16(color) (((unsigned)(color) >> 5) & 63)
#define SkGetPackedB16(color) (((unsigned)(color) >> 0) & 31)

#define SK_A32_SHIFT 24
#define SK_R32_SHIFT 16
#define SK_G32_SHIFT 8
#define SK_B32_SHIFT 0

#define SkGetPackedA32(packed) ((uint32_t)((packed) << 0) >> 24)
#define SkGetPackedR32(packed) ((uint32_t)((packed) << 8) >> 24)
#define SkGetPackedG32(packed) ((uint32_t)((packed) << 16) >> 24)
#define SkGetPackedB32(packed) ((uint32_t)((packed) << 24) >> 24)

#define NEON_A 3
#define NEON_R 2
#define NEON_G 1
#define NEON_B 0

#include "swan.hpp"

#endif /* AB273D1B_1B52_4C9B_9783_9D7BF54DC8D1 */
