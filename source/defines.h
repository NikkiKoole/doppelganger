#ifndef DEFINES_H
#define DEFINES_H

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

#define Kilobytes(value) ((value)*1024LL)
#define Megabytes(value) (Kilobytes(value)*1024LL)
#define Gigabytes(value) (Megabytes(value)*1024LL)
#define Terabytes(value) (Gigabytes(value)*1024LL)

#define Assert(expression) if(!(expression)) {printf("\nASSERT FAILED in function %s on line:%d (%s)\n\n",__FUNCTION__, __LINE__, __FILE__);exit(0);}

#endif
