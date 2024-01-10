#ifndef COMMON_TYPES_H_
#define COMMON_TYPES_H_

#define __STDC_LIMIT_MACROS

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <float.h>
#include <assert.h>
#include <string.h>

#if (CHAR_BIT == 16)
    typedef unsigned char       uint8_t;
    typedef char                int8_t;
#endif
#if (CHAR_BIT == 8)
    typedef unsigned char       uint8_t;
#endif

typedef float  float32_t;
typedef double float64_t;

#endif /* COMMON_TYPES_H_ */
