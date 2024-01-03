#include "animator.h"
#include "globals.h"

animator_t animations[ANIMATION_MAX];

void animator_create(const u32 width, const u32 height, 
                     const u32 number_of_frames, const u32 type, 
                     const u32 text_type) 
{
  animator_t *animator = &animations[type];
  animator->texture_map = textures[text_type];
  animator->width = width;
  animator->height = height;
  animator->number_of_frames = number_of_frames;
  animator->frame = 0.0f;
  animator->index = 0;
}

void animator_set_index(const u32 frame, const u32 type) 
{
  animations[type].index = frame;
}

void animator_update(const f32 framerate, const u32 type) 
{
  animator_t *animator = &animations[type];
  animator->index = animator->frame / framerate;
  animator->frame += DELTA_TIME;

  if (animator->index > animator->number_of_frames - 1) 
  {
    animator->frame = 0.0f;
    animator->index = 0;
  }
}

i32 animator_render(const i32 x, const i32 y, const u32 type) 
{
  animator_t *animator = &animations[type];
  byte overflow = 0;

  for (u32 i = 0; i < animator->height; i++) 
  {
    for (u32 j = 0; j < animator->width; j++) 
    {
      const i32 pixels_index = (i + y) * SCREEN_WIDTH + j + x;
      const u32 animator_index = i * animator->texture_map.width + j +
                                 animator->width * animator->index;
      const u16 animator_pixel = animator->texture_map.pixels[animator_index];

      if(is_valid_pixel(SCREEN_MAX, SCREEN_WIDTH, SCREEN_HEIGHT,
                        pixels_index, x + j, y + i))
      {
        ALPHA_BLEND_OVER(state.pixels[pixels_index], animator_pixel,
                         state.pixels[pixels_index]);
      }
      else overflow  = 1;
    }
  }

  return overflow;
}
