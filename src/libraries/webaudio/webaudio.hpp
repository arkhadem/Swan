#ifndef A8CD2168_806D_4F89_AC24_3E8876AED906
#define A8CD2168_806D_4F89_AC24_3E8876AED906

#include "swan.hpp"

static inline float ClampTo(float value, float min, float max) {
    if (value >= max)
        return max;
    if (value <= min)
        return min;
    return value;
}

#endif /* A8CD2168_806D_4F89_AC24_3E8876AED906 */
