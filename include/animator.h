#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "type.h"
#include "texture.h"

#define ANIMATION_MAX 5

typedef struct
{
  texture_t texture_map;
  u32 width, height;
  u32 index;
  u32 number_of_frames;
  u32 frame;
  byte animate;
} animator_t;

enum ANIMATIONS { PLAYER_ANIM, GRASS_ANIM, FLOWER_ANIM, WATER_ANIM, CORRUPTION_ANIM };

void animator_create(animator_t *animator, const texture_t texture_map, const u32 width, const u32 height, const u32 number_of_frames);
void animator_set_index(animator_t *animator, const u32 frame);
void animator_update(animator_t *animator, const u32 framerate);
i32 animator_add(animator_t *animator, const i32 x, const i32 y, u16 *pixels, const u32 pixels_width, const u32 pixels_height);

#endif
