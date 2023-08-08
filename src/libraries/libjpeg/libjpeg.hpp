#ifndef A027C2A0_39D2_45C4_A2AA_098B8EB820A1
#define A027C2A0_39D2_45C4_A2AA_098B8EB820A1

#define JDIMENSION unsigned int
#define JSAMPLE unsigned char
#define JSAMPROW unsigned char *
#define JSAMPARRAY unsigned char **
#define JSAMPIMAGE unsigned char ***

#define JCOEF short
#define JCOEFARRAY short *
#define UJCOEF unsigned short
#define UJCOEFARRAY unsigned short *
#define JLONG long

#define RGB_PIXELSIZE 4
#define RGB_PIXELSIZE_LOG 2
#define RGB_RED 0
#define RGB_GREEN 1
#define RGB_BLUE 2
#define RGB_ALPHA 3

#define MAXJSAMPLE 255
#define CENTERJSAMPLE 128

#define SCALEBITS 16 /* speediest right-shift on some machines */
#define CBCR_OFFSET ((JLONG)CENTERJSAMPLE << SCALEBITS)
#define ONE_HALF ((JLONG)1 << (SCALEBITS - 1))
#define FIX(x) ((JLONG)((x) * (1L << SCALEBITS) + 0.5))

#define R_Y_OFF 0                      /* offset to R => Y section */
#define G_Y_OFF (1 * (MAXJSAMPLE + 1)) /* offset to G => Y section */
#define B_Y_OFF (2 * (MAXJSAMPLE + 1)) /* etc. */
#define R_CB_OFF (3 * (MAXJSAMPLE + 1))
#define G_CB_OFF (4 * (MAXJSAMPLE + 1))
#define B_CB_OFF (5 * (MAXJSAMPLE + 1))
#define R_CR_OFF B_CB_OFF /* B=>Cb, R=>Cr are the same */
#define G_CR_OFF (6 * (MAXJSAMPLE + 1))
#define B_CR_OFF (7 * (MAXJSAMPLE + 1))
#define TABLE_SIZE (8 * (MAXJSAMPLE + 1))
#define RIGHT_SHIFT(x, shft) ((x) >> (shft))

#define FALLTHROUGH __attribute__((fallthrough));

#include "swan.hpp"

#endif /* A027C2A0_39D2_45C4_A2AA_098B8EB820A1 */
