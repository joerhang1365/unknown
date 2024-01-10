#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "texture.h"

#define ANIMATION_MAX 6

typedef struct
{
  texture_t texture_map;
  u32 width, height;
  u32 index;
  u32 number_of_frames;
  f32 frame;
} animator_t;

enum ANIMATIONS { PLAYER_ANIM, GRASS_ANIM, FLOWER_ANIM, WATER_ANIM, 
                  BUTTON_ANIM };

extern animator_t animations[ANIMATION_MAX]; 

void animator_create(const u32 width, const u32 height, 
                     const u32 number_of_frames, const u32 type,
                     const u32 text_type);
void animator_set_index(const u32 frame, const u32 type);
void animator_update(const f32 framerate, const u32 type);
i32 animator_render(const i32 x, const i32 y, const u32 type);

#endif
