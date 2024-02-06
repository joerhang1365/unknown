#ifndef TEXTURE_H
#define TEXTURE_H

#include "globals.h"
#include "state.h"

#define TEXTURE_MAX 14

typedef struct
{
  u16 *pixels;
  u32 width;
  u32 height;
  u32 bytes_per_pixel;
} texture_t;

enum TEXTURES { TILE_TXT, BLANK_TXT, GRASS_TXT, FLOWER_TXT, WOOD_TXT, T_TXT, 
                NEXT_TXT, PLAYER_TXT, P_TXT, ROCK_TXT, WATER_TXT, GIRL_TXT, 
                CORRUPTION_TXT, BUTTON_TXT };

extern texture_t textures[TEXTURE_MAX];

void texture_create(const char *source, const u32 type);
void texture_render(const i32 x, const i32 y, const u32 type);
void texture_destroy(const u32 type);

#endif
