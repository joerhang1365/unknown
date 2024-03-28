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
#define SCREEN_WIDTH_2 SCREEN_WIDTH / 2
#define SCREEN_HEIGHT_2 SCREEN_HEIGHT / 2
#define SCREEN_MAX SCREEN_WIDTH * SCREEN_HEIGHT
#define SCALE 7
#define FRAMERATE 24

#define PI 3.1415926535
#define PI2 PI * 2
#define PI_2 PI / 2

typedef int16_t  i16;
typedef uint16_t u16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint64_t u64;
typedef uint32_t u32;
typedef float    f32;
typedef double   f64;
typedef unsigned char byte;

typedef struct { f32 x, y; } vec2;
typedef struct { i32 x, y; } veci2;

static inline vec2 vec2_create(const f32 x, const f32 y)
{
  vec2 temp;
  temp.x = x;
  temp.y = y;
  return temp;
}

static inline veci2 veci2_create(const i32 x, const i32 y)
{
  veci2 temp;
  temp.x = x;
  temp.y = y;
  return temp;
}

static inline veci2 vec2_to_veci2(const vec2 vec2)
{
  return veci2_create(vec2.x, vec2.y);
}

static inline void vec2_scale(vec2 *vec2, const f32 scale)
{
  vec2->x *= scale;
  vec2->y *= scale;
}

static inline void veci2_scale(veci2 *veci2, const i32 scale)
{
  veci2->x *= scale;
  veci2->y *= scale;
}

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
  ((u16)((_color & 0xF000) * (f32)(_color & 0x000F) / 15.0f) & 0xF000) + \
  ((u16)((_color & 0x0F00) * (f32)(_color & 0x000F) / 15.0f) & 0x0F00) + \
  ((u16)((_color & 0x00F0) * (f32)(_color & 0x000F) / 15.0f) & 0x00F0) + \
  (u16)(_color & 0x000F);

/* 
 * TODO
 * actually fix alpha out
 */

#define ALPHA_OVER(_color, _a, _b) _color = \
  (((_a & 0xF000) + (_b & 0xF000) * (15 - (_a & 0x000F))) & 0xF000) + \
  (((_a & 0x0F00) + (_b & 0x0F00) * (15 - (_a & 0x000F))) & 0x0F00) + \
  (((_a & 0x00F0) + (_b & 0x00F0) * (15 - (_a & 0x000F))) & 0x00F0) + \
  (((_a & 0x000F) > (_b & 0x000F) ? (_a & 0x000F) : (_b & 0x000F)) & 0x000F);

#define ALPHA_SET(_arr, _size, _a) \
  for(u32 i = 0; i < _size; i++) \
  { \
    _arr[i] &= 0xFFF0; \
    _arr[i] += _a; \
  }

static inline byte is_valid_pixel(const u32 index, const i32 x, const i32 y)
{
  return index >= 0 && index < SCREEN_MAX &&
         x >= 0 && x < SCREEN_WIDTH &&
         y >= 0 && y < SCREEN_HEIGHT;
}

enum KEYS { NONE, LEFT, RIGHT, UP, DOWN, X, ONE, TWO, THREE, F1};

extern f64 TIME;
extern f64 FRAME_TIME;
extern f64 DELTA_TIME;
extern u32 PREVIOUS_TIME;
extern u32 CURRENT_TIME;

#endif
