#include "animator.h"
#include "globals.h"

animator_t animations[ANIMATION_MAX];

void animator_create(animator_t *animator, const texture_t texture_map, const u32 width, const u32 height, const u32 number_of_frames) 
{
  animator->texture_map = texture_map;
  animator->width = width;
  animator->height = height;
  animator->number_of_frames = number_of_frames;
  animator->frame = 0;
  animator->index = 0;
}

void animator_set_index(animator_t *animator, const u32 frame) 
{
  animator->index = frame;
}

void animator_update(animator_t *animator, const u32 framerate) 
{
  animator->index = animator->frame / framerate;
  ++animator->frame;

  if(animator->index > animator->number_of_frames - 1) 
  {
    animator->frame = 0;
    animator->index = 0;
  }
}

i32 animator_add(animator_t *animator, const i32 x, const i32 y)
{
  byte overflow = 0;

  for(u32 i = 0; i < animator->height; i++)
  {
    for(u32 j = 0; j < animator->width; j++)
    {
      const i32 pixels_index = (i + y) * SCREEN_WIDTH + j + x;
      const u32 animator_index = i * animator->texture_map.width + j + animator->width * animator->index;
      const u16 animator_pixel = animator->texture_map.pixels[animator_index];
      overflow = pixels_index > SCREEN_MAX;

      if(pixels_index >= 0 && 
        overflow == 0 && 
        (j + x) >= 0 && 
        (j + x) < SCREEN_WIDTH &&
        (i + y) >= 0 &&
        (i + y) < SCREEN_HEIGHT)
      {
        ALPHA_BLEND_OVER(state.pixels[pixels_index], animator_pixel, state.pixels[pixels_index]);
      }
    }
  }

  return overflow;
}

