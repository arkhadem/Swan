#ifndef D88A2E19_D3DF_45CA_AF95_AF1E0887F01E
#define D88A2E19_D3DF_45CA_AF95_AF1E0887F01E

#include "swan.hpp"
#include <limits.h>

#define opus_int int           /* used for counters etc; at least 16 bits */
#define opus_uint unsigned int /* used for counters etc; at least 16 bits */

typedef float opus_val16;
typedef float opus_val32;
typedef float opus_val64;

typedef int8_t opus_int8;
typedef uint8_t opus_uint8;
typedef int16_t opus_int16;
typedef uint16_t opus_uint16;
typedef int32_t opus_int32;
typedef uint32_t opus_uint32;
typedef int64_t opus_int64;
typedef uint64_t opus_uint64;

#define MAC16_16(c, a, b) ((c) + (opus_val32)(a) * (opus_val32)(b))
#define silk_LIMIT(a, limit1, limit2) ((limit1) > (limit2) ? ((a) > (limit1) ? (limit1) : ((a) < (limit2) ? (limit2) : (a))) \
                                                           : ((a) > (limit2) ? (limit2) : ((a) < (limit1) ? (limit1) : (a))))
#define silk_SMMUL(a32, b32) (opus_int32) silk_RSHIFT64(silk_SMULL((a32), (b32)), 32)
#define silk_abs(a) (((a) > 0) ? (a) : -(a)) /* Be careful, silk_abs returns wrong when input equals to silk_intXX_MIN */

#define SILK_MAX_ORDER_LPC 24 /* max order of the LPC analysis in schur() and k2a() */
#define MAX_SHAPE_LPC_ORDER 24
#define QC 10
#define QS 13

#if INT_MAX >= 2147483647
#define EC_CLZ0 ((int)sizeof(unsigned) * CHAR_BIT)
#define EC_CLZ(_x) (__builtin_clz(_x))
#elif LONG_MAX >= 2147483647L
#define EC_CLZ0 ((int)sizeof(unsigned long) * CHAR_BIT)
#define EC_CLZ(_x) (__builtin_clzl(_x))
#endif

#if defined(EC_CLZ)
/*Note that __builtin_clz is not defined when _x==0, according to the gcc
   documentation (and that of the BSR instruction that implements it on x86).
  The majority of the time we can never pass it zero.
  When we need to, it can be special cased.*/
#define EC_ILOG(_x) (EC_CLZ0 - EC_CLZ(_x))
#else
int ec_ilog(opus_uint32 _v);
#define EC_ILOG(_x) (ec_ilog(_x))
#endif

#define silk_int64_MAX ((opus_int64)0x7FFFFFFFFFFFFFFFLL) /*  2^63 - 1 */
#define silk_int64_MIN ((opus_int64)0x8000000000000000LL) /* -2^63 */
#define silk_int32_MAX 0x7FFFFFFF                         /*  2^31 - 1 =  2147483647 */
#define silk_int32_MIN ((opus_int32)0x80000000)           /* -2^31     = -2147483648 */
#define silk_int16_MAX 0x7FFF                             /*  2^15 - 1 =  32767 */
#define silk_int16_MIN ((opus_int16)0x8000)               /* -2^15     = -32768 */
#define silk_int8_MAX 0x7F                                /*  2^7 - 1  =  127 */
#define silk_int8_MIN ((opus_int8)0x80)                   /* -2^7      = -128 */
#define silk_uint8_MAX 0xFF                               /*  2^8 - 1 = 255 */

#define silk_TRUE 1
#define silk_FALSE 0

#define MAX_PREDICTION_POWER_GAIN 1e4f

/*                                                                                           */
/*                                                                                           */
/*                          external/libopus/silk/SigProc_FIX.h                              */

/* (a32 * b32) output have to be 32bit int */
#define silk_MUL(a32, b32) ((a32) * (b32))

/* (a32 * b32) output have to be 32bit uint */
#define silk_MUL_uint(a32, b32) silk_MUL(a32, b32)

/* a32 + (b32 * c32) output have to be 32bit int */
#define silk_MLA(a32, b32, c32) silk_ADD32((a32), ((b32) * (c32)))

/* a32 + (b32 * c32) output have to be 32bit uint */
#define silk_MLA_uint(a32, b32, c32) silk_MLA(a32, b32, c32)

/* ((a32 >> 16)  * (b32 >> 16)) output have to be 32bit int */
#define silk_SMULTT(a32, b32) (((a32) >> 16) * ((b32) >> 16))

/* a32 + ((a32 >> 16)  * (b32 >> 16)) output have to be 32bit int */
#define silk_SMLATT(a32, b32, c32) silk_ADD32((a32), ((b32) >> 16) * ((c32) >> 16))

#define silk_SMLALBB(a64, b16, c16) silk_ADD64((a64), (opus_int64)((opus_int32)(b16) * (opus_int32)(c16)))

/* (a32 * b32) */
#define silk_SMULL(a32, b32) ((opus_int64)(a32) * (opus_int64)(b32))

#define silk_SMLABB_ovflw(a32, b32, c32) (silk_ADD32_ovflw((a32), ((opus_int32)((opus_int16)(b32))) * (opus_int32)((opus_int16)(c32))))

#define silk_DIV32_16(a32, b16) ((opus_int32)((a32) / (b16)))
#define silk_DIV32(a32, b32) ((opus_int32)((a32) / (b32)))

/* These macros enables checking for overflow in silk_API_Debug.h*/
#define silk_ADD16(a, b) ((a) + (b))
#define silk_ADD32(a, b) ((a) + (b))
#define silk_ADD64(a, b) ((a) + (b))

#define silk_SUB16(a, b) ((a) - (b))
#define silk_SUB32(a, b) ((a) - (b))
#define silk_SUB64(a, b) ((a) - (b))

#define silk_SAT8(a) ((a) > silk_int8_MAX ? silk_int8_MAX : ((a) < silk_int8_MIN ? silk_int8_MIN : (a)))
#define silk_SAT16(a) ((a) > silk_int16_MAX ? silk_int16_MAX : ((a) < silk_int16_MIN ? silk_int16_MIN : (a)))
#define silk_SAT32(a) ((a) > silk_int32_MAX ? silk_int32_MAX : ((a) < silk_int32_MIN ? silk_int32_MIN : (a)))

#define silk_CHECK_FIT8(a) (a)
#define silk_CHECK_FIT16(a) (a)
#define silk_CHECK_FIT32(a) (a)

#define silk_ADD_SAT16(a, b) (opus_int16) silk_SAT16(silk_ADD32((opus_int32)(a), (b)))
#define silk_ADD_SAT64(a, b) ((((a) + (b)) & 0x8000000000000000LL) == 0 ? ((((a) & (b)) & 0x8000000000000000LL) != 0 ? silk_int64_MIN : (a) + (b)) : ((((a) | (b)) & 0x8000000000000000LL) == 0 ? silk_int64_MAX : (a) + (b)))

#define silk_SUB_SAT64(a, b) ((((a) - (b)) & 0x8000000000000000LL) == 0 ? (((a) & ((b) ^ 0x8000000000000000LL) & 0x8000000000000000LL) ? silk_int64_MIN : (a) - (b)) : ((((a) ^ 0x8000000000000000LL) & (b)&0x8000000000000000LL) ? silk_int64_MAX : (a) - (b)))

/* Saturation for positive input values */
#define silk_POS_SAT32(a) ((a) > silk_int32_MAX ? silk_int32_MAX : (a))

/* Add with saturation for positive input values */
#define silk_ADD_POS_SAT8(a, b) ((((a) + (b)) & 0x80) ? silk_int8_MAX : ((a) + (b)))
#define silk_ADD_POS_SAT16(a, b) ((((a) + (b)) & 0x8000) ? silk_int16_MAX : ((a) + (b)))

#define silk_LSHIFT8(a, shift) ((opus_int8)((opus_uint8)(a) << (shift)))    /* shift >= 0, shift < 8  */
#define silk_LSHIFT16(a, shift) ((opus_int16)((opus_uint16)(a) << (shift))) /* shift >= 0, shift < 16 */
#define silk_LSHIFT32(a, shift) ((opus_int32)((opus_uint32)(a) << (shift))) /* shift >= 0, shift < 32 */
#define silk_LSHIFT64(a, shift) ((opus_int64)((opus_uint64)(a) << (shift))) /* shift >= 0, shift < 64 */
#define silk_LSHIFT(a, shift) silk_LSHIFT32(a, shift)                       /* shift >= 0, shift < 32 */

#define silk_RSHIFT16(a, shift) ((a) >> (shift))      /* shift >= 0, shift < 16 */
#define silk_RSHIFT32(a, shift) ((a) >> (shift))      /* shift >= 0, shift < 32 */
#define silk_RSHIFT64(a, shift) ((a) >> (shift))      /* shift >= 0, shift < 64 */
#define silk_RSHIFT(a, shift) silk_RSHIFT32(a, shift) /* shift >= 0, shift < 32 */

/* saturates before shifting */
#define silk_LSHIFT_SAT32(a, shift) (silk_LSHIFT32(silk_LIMIT((a), silk_RSHIFT32(silk_int32_MIN, (shift)), \
                                                              silk_RSHIFT32(silk_int32_MAX, (shift))),     \
                                                   (shift)))

#define silk_LSHIFT_ovflw(a, shift) ((opus_int32)((opus_uint32)(a) << (shift))) /* shift >= 0, allowed to overflow */
#define silk_LSHIFT_uint(a, shift) ((a) << (shift))                             /* shift >= 0 */
#define silk_RSHIFT_uint(a, shift) ((a) >> (shift))                             /* shift >= 0 */

#define silk_ADD_LSHIFT(a, b, shift) ((a) + silk_LSHIFT((b), (shift)))              /* shift >= 0 */
#define silk_ADD_LSHIFT32(a, b, shift) silk_ADD32((a), silk_LSHIFT32((b), (shift))) /* shift >= 0 */
#define silk_ADD_LSHIFT_uint(a, b, shift) ((a) + silk_LSHIFT_uint((b), (shift)))    /* shift >= 0 */
#define silk_ADD_RSHIFT32(a, b, shift) silk_ADD32((a), silk_RSHIFT32((b), (shift))) /* shift >= 0 */
#define silk_ADD_RSHIFT_uint(a, b, shift) ((a) + silk_RSHIFT_uint((b), (shift)))    /* shift >= 0 */

/* Requires that shift > 0 */
#define silk_RSHIFT_ROUND(a, shift) ((shift) == 1 ? ((a) >> 1) + ((a)&1) : (((a) >> ((shift)-1)) + 1) >> 1)
#define silk_RSHIFT_ROUND64(a, shift) ((shift) == 1 ? ((a) >> 1) + ((a)&1) : (((a) >> ((shift)-1)) + 1) >> 1)

/* Number of rightshift required to fit the multiplication */
#define silk_NSHIFT_MUL_32_32(a, b) (-(31 - (32 - silk_CLZ32(silk_abs(a)) + (32 - silk_CLZ32(silk_abs(b))))))
#define silk_NSHIFT_MUL_16_16(a, b) (-(15 - (16 - silk_CLZ16(silk_abs(a)) + (16 - silk_CLZ16(silk_abs(b))))))

#define silk_min(a, b) (((a) < (b)) ? (a) : (b))
#define silk_max(a, b) (((a) > (b)) ? (a) : (b))

/* Macro to convert floating-point constants to fixed-point */
#define SILK_FIX_CONST(C, Q) ((opus_int32)((C) * ((opus_int64)1 << (Q)) + 0.5))

/*                          external/libopus/silk/SigProc_FIX.h                              */
/*                                                                                           */
/*                                                                                           */

/*                                                                                           */
/*                                                                                           */
/*                             external/libopus/silk/macros.h                                */

/* (a32 * (opus_int32)((opus_int16)(b32))) >> 16 output have to be 32bit int */
#define silk_SMULWB(a32, b32) ((((a32) >> 16) * (opus_int32)((opus_int16)(b32))) + ((((a32)&0x0000FFFF) * (opus_int32)((opus_int16)(b32))) >> 16))

/* a32 + (b32 * (opus_int32)((opus_int16)(c32))) >> 16 output have to be 32bit int */
#define silk_SMLAWB(a32, b32, c32) ((a32) + ((((b32) >> 16) * (opus_int32)((opus_int16)(c32))) + ((((b32)&0x0000FFFF) * (opus_int32)((opus_int16)(c32))) >> 16)))

/* (a32 * (b32 >> 16)) >> 16 */
#define silk_SMULWT(a32, b32) (((a32) >> 16) * ((b32) >> 16) + ((((a32)&0x0000FFFF) * ((b32) >> 16)) >> 16))

/* a32 + (b32 * (c32 >> 16)) >> 16 */
#define silk_SMLAWT(a32, b32, c32) ((a32) + (((b32) >> 16) * ((c32) >> 16)) + ((((b32)&0x0000FFFF) * ((c32) >> 16)) >> 16))

/* (opus_int32)((opus_int16)(a3))) * (opus_int32)((opus_int16)(b32)) output have to be 32bit int */
#define silk_SMULBB(a32, b32) ((opus_int32)((opus_int16)(a32)) * (opus_int32)((opus_int16)(b32)))

/* a32 + (opus_int32)((opus_int16)(b32)) * (opus_int32)((opus_int16)(c32)) output have to be 32bit int */
#define silk_SMLABB(a32, b32, c32) ((a32) + ((opus_int32)((opus_int16)(b32))) * (opus_int32)((opus_int16)(c32)))

/* (opus_int32)((opus_int16)(a32)) * (b32 >> 16) */
#define silk_SMULBT(a32, b32) ((opus_int32)((opus_int16)(a32)) * ((b32) >> 16))

/* a32 + (opus_int32)((opus_int16)(b32)) * (c32 >> 16) */
#define silk_SMLABT(a32, b32, c32) ((a32) + ((opus_int32)((opus_int16)(b32))) * ((c32) >> 16))

/* a64 + (b32 * c32) */
#define silk_SMLAL(a64, b32, c32) (silk_ADD64((a64), ((opus_int64)(b32) * (opus_int64)(c32))))

/* (a32 * b32) >> 16 */
#define silk_SMLAWW(a32, b32, c32) silk_MLA(silk_SMLAWB((a32), (b32), (c32)), (b32), silk_RSHIFT_ROUND((c32), 16))

#define silk_CLZ32(in32) \
    ((in32) ? 32 - EC_ILOG(in32) : 32)

/*                                                                                           */
/*                                                                                           */
/*                             external/libopus/silk/macros.h                                */

/*                                                                                           */
/*                                                                                           */
/*                            external/libopus/silk/Inlines.h                                */

/* count leading zeros of opus_int64 */
#define silk_CLZ64(in) \
    (((opus_int32)silk_RSHIFT64(in, 32)) == 0 ? (32 + silk_CLZ32((opus_int32)(in))) : silk_CLZ32((opus_int32)silk_RSHIFT64(in, 32)))

/*                            external/libopus/silk/Inlines.h                                */
/*                                                                                           */
/*                                                                                           */

/*                                                                                           */
/*                                                                                           */
/*                           external/libopus/silk/MacroDebug.h                              */

#define silk_SUB_SAT32(a32, b32) \
    ((((opus_uint32)(a32) - (opus_uint32)(b32)) & 0x80000000) == 0 ? (((a32) & ((b32) ^ 0x80000000) & 0x80000000) ? silk_int32_MIN : (a32) - (b32)) : ((((a32) ^ 0x80000000) & (b32)&0x80000000) ? silk_int32_MAX : (a32) - (b32)))

/*                           external/libopus/silk/MacroDebug.h                              */
/*                                                                                           */
/*                                                                                           */

#endif /* D88A2E19_D3DF_45CA_AF95_AF1E0887F01E */
