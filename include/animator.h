#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "type.h"
#include "texture.h"

typedef struct
{
  texture_t texture_map;
  i32 width, height;
  u32 index;
  u32 number_of_frames;
  u32 frame;
} animator;

void animator_create(animator *animator, const texture_t texture_map, const i32 width, const i32 height, const u32 number_of_frames);
void animator_set_index(animator *animator, const u32 frame);
void animator_update(animator *animator, const u32 framerate);
i32 animator_add(animator animator, const i32 x, const i32 y, u32 *pixels, const i32 pixels_width, const u32 pixels_max);

#endif
