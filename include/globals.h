#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>

#define ASSERT(_e, ...) if (_e) { fprintf(stderr, __VA_ARGS__); exit(1); }

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
#define SCREEN_MAX SCREEN_WIDTH * SCREEN_HEIGHT
#define SCALE 6
#define FRAMERATE 24

#define PI 3.1415926535
#define PI2 PI * 2

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
#define VECi2_TO_VEC(v2, vi2) (v2).x = (vi2).x; (v2).y = (vi2).y;
#define VEC2_MULTI(v, _s) (v).x = (v).x * (f32)_s; (v).y = (v).y * (f32)_s;
#define VECi2_MULTI(v, _s) (v).x = (v).x * (i32)_s; (v).y = (v).y * (i32)_s;

#define LEFT_SHIFT(_arr, _new, _size) \
  for(u32 i = 0; i < _size - 1; i++) \
  { \
    _arr[i] = _arr[i + 1]; \
  } \
  _arr[_size - 1] = _new; \

#define RIGHT_SHIFT(_arr, _size) \
  for(u32 i = _size; i > 0; i--) \
  { \
    _arr[i] = _arr[i - 1]; \
  }

#define SCREEN_CLEAR(pixels, size) \
  for(u32 i = 0; i < size; i++) \
  { \
    pixels[i] = 0x0000; \
  }

#define ALPHA_BLEND(_color) _color = \
  ((u16)((_color & 0xF000) * (f32)(_color & 0x000F) / 16.0f) & 0xF000) + \
  ((u16)((_color & 0x0F00) * (f32)(_color & 0x000F) / 16.0f) & 0x0F00) + \
  ((u16)((_color & 0x00F0) * (f32)(_color & 0x000F) / 16.0f) & 0x00F0) + \
  (u16)(_color & 0x000F);

#define ALPHA_BLEND_OVER(_color, _a, _b) _color = \
  ((u16)(((_a & 0xF000) * (_a & 0x000F) + (_b & 0xF000) * (_b & 0x000F) * (1 - (f32)(_a & 0x000F) / 16)) / 16) & 0xF000) + \
  ((u16)(((_a & 0x0F00) * (_a & 0x000F) + (_b & 0x0F00) * (_b & 0x000F) * (1 - (f32)(_a & 0x000F) / 16)) / 16) & 0x0F00) + \
  ((u16)(((_a & 0x00F0) * (_a & 0x000F) + (_b & 0x00F0) * (_b & 0x000F) * (1 - (f32)(_a & 0x000F) / 16)) / 16) & 0x00F0) +  \
  ((u16)((_a & 0x000F)+ (_a & 0x000F) * (1 - (f32)(_a & 0x000F) / 16.0f)) & 0x000F);

enum KEYS { NONE, LEFT, RIGHT, UP, DOWN, X, ONE, F1};

extern f32 TIME;
extern f32 FRAME_TIME;
extern f32 DELTA_TIME;
extern u32 PREVIOUS_TIME;
extern u32 CURRENT_TIME;

#endif
