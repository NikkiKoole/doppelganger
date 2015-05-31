#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>     // int8_t et al.
#include <stdlib.h>     // size_t

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

typedef size_t memory_index;

#define internal static

#define kilobytes(value) ((value)*1024LL)
#define megabytes(value) (kilobytes(value)*1024LL)
#define gigabytes(value) (megabytes(value)*1024LL)
#define terabytes(value) (gigabytes(value)*1024LL)

#define assert(expression) if(!(expression)) {printf("\nASSERT FAIL: '%s' in function %s on line:%d (%s)\n\n",#expression, __FUNCTION__, __LINE__, __FILE__);exit(0);}

#endif
