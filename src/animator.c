#include "animator.h"

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
  if(animator->animate == 0) return;

  animator->index = animator->frame / framerate;
  ++animator->frame;

  if(animator->index > animator->number_of_frames - 1) 
  {
    animator->frame = 0;
    animator->index = 0;
  }

  animator->animate = 0;
}

i32 animator_add(animator_t *animator, const i32 x, const i32 y, u16 *pixels, const u32 pixels_width, const u32 pixels_max)
{
  byte overflow = 0;
  // const u32 pixels_height = pixels_max / pixels_height;

  for(u32 i = 0; i < animator->height; i++)
  {
    for(u32 j = 0; j < animator->width; j++)
    {
      const i32 pixels_index = (i + y) * pixels_width + j + x;
      const u32 animator_index = i * animator->texture_map.width + j + animator->width * animator->index;
      const u16 animator_pixel = animator->texture_map.pixels[animator_index];
      overflow = pixels_index > pixels_max;

      if(pixels_index >= 0 && 
        overflow == 0 && 
        (j + x) >= 0 && 
        (j + x) < pixels_width &&
        (i + y) >= 0)
      {
        // over blend formula
        f32 alpha_a = animator_pixel & 0x000F;
        f32 red_a = animator_pixel & 0xF000;
        f32 green_a = animator_pixel & 0x0F00;
        f32 blue_a = animator_pixel & 0x00F0;
        f32 alpha_b = pixels[pixels_index] & 0x000F;
        f32 red_b = pixels[pixels_index] & 0xF000;
        f32 green_b = pixels[pixels_index] & 0x0F00;
        f32 blue_b = pixels[pixels_index] & 0x00F0;
        
        u16 alpha_out = (u16)(alpha_a + alpha_b * (1 - alpha_a / 16)) & 0x000F;
        u16 red_out = (u16)((red_a * alpha_a + red_b * alpha_b * (1 - alpha_a / 16)) / 16) & 0xF000;
        u16 green_out = (u16)((green_a * alpha_a + green_b * alpha_b * (1 - alpha_a / 16)) / 16) & 0x0F00;
        u16 blue_out = (u16)((blue_a * alpha_a + blue_b * alpha_b * (1 - alpha_a / 16)) / 16) & 0x00F0;

        pixels[pixels_index] = red_out + green_out + blue_out + alpha_out;

      }
    }
  }

  animator->animate = 1;
  return overflow;
}

