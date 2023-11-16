#include "animator.h"

void animator_create(animator_t *animator, const texture_t texture_map, const i32 width, const i32 height, const u32 number_of_frames) 
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

i32 animator_add(animator_t animator, const i32 x, const i32 y, u16 *pixels, const i32 pixels_width, const i32 pixels_max)
{
  i32 overflow = 0;
  for(i32 i = 0; i < animator.height; i++)
  {
    for(i32 j = 0; j < animator.width; j++)
    {
      const i32 pixel_index = (i + y) * pixels_width + j + x;
      const i32 animator_txt_index = i * animator.texture_map.width + j + animator.width * animator.index;
        
      if(pixel_index >= 0 && 
         pixel_index < pixels_max && 
         (j + x) >= 0 && 
         (j + x) < pixels_width &&
          animator.texture_map.pixels[animator_txt_index] != 0x0000)
      {
        pixels[pixel_index] = animator.texture_map.pixels[animator_txt_index];
      }
    }
  }
  return overflow;
}

