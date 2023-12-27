#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "alpha.h"

#define TEXTURE_MAX 13

typedef struct
{
  u16 *pixels;
  u32 width;
  u32 height;
  u32 bytes_per_pixel;
} texture_t;

enum TEXTURES { TILE_TXT, BLANK_TXT, GRASS_TXT, FLOWER_TXT, WOOD_TXT, T_TXT, NEXT_TXT, PLAYER_TXT, P_TXT, ROCK_TXT, WATER_TXT, GIRL_TXT, CORRUPTION_TXT };

i32 texture_create(const char *source, texture_t *texture);
i32 texture_add(texture_t texture, const i32 x, const i32 y, u16 *pixels, const u32 pixels_width, const u32 pixels_height);
void texture_destroy(texture_t texture);

#endif
