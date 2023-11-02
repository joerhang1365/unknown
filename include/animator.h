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
} animator_t;

void animator_create(animator_t *animator, const texture_t texture_map, const i32 width, const i32 height, const u32 number_of_frames);
void animator_set_index(animator_t *animator, const u32 frame);
void animator_update(animator_t *animator, const u32 framerate);
i32 animator_add(animator_t animator, const i32 x, const i32 y, u16 *pixels, const i32 pixels_width, const i32 pixels_max);

#endif
