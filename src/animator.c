#include "animator.h"

animator_t animations[ANIMATION_MAX];

void animator_create(const u32 width, const u32 height, 
                     const u32 number_of_frames, const u32 type, 
                     const u32 text_type) 
{
  animations[type].texture_map = textures[text_type];
  animations[type].width = width;
  animations[type].height = height;
  animations[type].number_of_frames = number_of_frames;
  animations[type].frame = 0.0f;
  animations[type].index = 0;
}

void animator_set_index(const u32 frame, const u32 type) 
{
  animations[type].index = frame;
}

void animator_update(const f32 framerate, const u32 type) 
{
  animations[type].index = animations[type].frame / framerate;
  animations[type].frame += DELTA_TIME;

  if (animations[type].index > animations[type].number_of_frames - 1) 
  {
    animations[type].frame = 0.0f;
    animations[type].index = 0;
  }
}

void animator_render(const i32 x, const i32 y, const u32 type) 
{
  for (u32 i = 0; i < animations[type].height; i++) 
  {
    for (u32 j = 0; j < animations[type].width; j++) 
    {
      const i32 pixels_index = (i + y) * SCREEN_WIDTH + j + x;
      const u32 animator_index = i * animations[type].texture_map.width + j +
                                 animations[type].width * animations[type].index;
      const u16 animator_pixel = animations[type].texture_map.pixels[animator_index];

      if(!is_valid_pixel(pixels_index, x + j, y + i)) continue;

        ALPHA_OVER(state.pixels[pixels_index], 
                  animator_pixel,
                  state.pixels[pixels_index]);
    }
  }
}
