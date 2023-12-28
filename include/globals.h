#ifndef GLOBALS_H
#define GLOBALS_H

#include "stdio.h"
#include "stdint.h"

#define ASSERT(_e, ...) if (_e) { fprintf(stderr, __VA_ARGS__); exit(1); }

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
#define SCREEN_MAX SCREEN_WIDTH * SCREEN_HEIGHT
#define SCALE 6
#define FRAMERATE 12
#define PI 3.1415926535

typedef int16_t  i16;
typedef uint16_t u16;
typedef int32_t  i32;
typedef uint32_t u32;
typedef float    f32;
typedef double   f64;
typedef unsigned char byte;

typedef struct { f32 x, y; } vec2;
typedef struct { i32 x, y; } veci2;

#define VEC2(v, _x, _y) (v).x = (f32)(_x); (v).y = (f32)(_y);
#define VECi2(v, _x, _y) (v).x = (i32)(_x); (v).y = (i32)(_y);
#define VEC2_MULTI(v, _s) (v).x = (v).x * (f32)_s; (v).y = (v).y * (f32)_s;
#define VECi2_MULTI(v, _s) (v).x = (v).x * (i32)_s; (v).y = (v).y * (i32)_s;

#define LEFT_SHIFT(arr, new, size) \
  for(u32 i = 0; i < size - 1; i++) \
  { \
    arr[i] = arr[i + 1]; \
  } \
  arr[size - 1] = new; \

#define SCREEN_CLEAR(pixels, size) \
  for(u32 i = 0; i < size; i++) \
  { \
    pixels[i] = 0x0000; \
  }

enum KEYS { NONE, LEFT, RIGHT, UP, DOWN, X, ONE, F1};

#define MAP_COUNT 6

#endif
