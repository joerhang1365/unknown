#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "texture.h"

#define ANIMATION_MAX 5

typedef struct
{
  texture_t texture_map;
  u32 width, height;
  u32 index;
  u32 number_of_frames;
  f32 frame;
} animator_t;

enum ANIMATIONS { PLAYER_ANIM, GRASS_ANIM, FLOWER_ANIM, WATER_ANIM };

extern animator_t animations[ANIMATION_MAX]; 

void animator_create(animator_t *animator, const texture_t texture_map,
                     const u32 width, const u32 height, 
                     const u32 number_of_frames);
void animator_set_index(animator_t *animator, const u32 frame);
void animator_update(animator_t *animator, const f32 framerate);
texture_t animator_to_texture(const animator_t animator);
i32 animator_add(animator_t *animator, const i32 x, const i32 y);

#endif
