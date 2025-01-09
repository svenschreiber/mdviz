#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <stdbool.h>

#define u8 uint8_t
#define u32 uint32_t
#define s32 int32_t
#define b32 s32
#define f32 float
#define f64 double

#define ALLOC(type, count) ((type *)malloc(sizeof(type) * count))
#define ALLOC_ZERO(type, count) ((type *)calloc(count, sizeof(type)))

#endif // MAIN_H
