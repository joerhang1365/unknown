#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "type.h"

#define TEXTURE_MAX 9

typedef struct
{
  u16 *pixels;
  i32 width;
  i32 height;
  i32 bytes_per_pixel;
} texture_t;

#define DATA_OFFSET_OFFSET 0x0009
#define WIDTH_OFFSET 0x0012
#define HEIGHT_OFFSET 0x0016
#define BITS_PER_PIXEL_OFFSET 0x001C
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define NO_COMPERSION 0
#define MAX_NUMBER_OF_COLORS 0
#define ALL_COLORS_REQUIRED 0

enum TEXTURES { TILE_TXT, BLANK_TXT, GRASS_TXT, FLOWER_TXT, TEXT_TXT, NEXT_TXT, P_TXT, ROCK_TXT, WATER_TXT };

i32 texture_create(const char *source, texture_t *texture);
i32 texture_add(texture_t texture, const i32 x, const i32 y, u16 *pixels, const i32 pixels_width, const i32 pixels_max);
void texture_destroy(texture_t texture);

#endif
