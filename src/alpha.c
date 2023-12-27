#include "alpha.h"

u16 alpha_color(u16 color)
{
  f32 alpha_scale = (color & 0x000F) / 16.0f;

  u16 red_out = (u16)((color & 0xF000) * alpha_scale) & 0xF000;
  u16 green_out = (u16)((color & 0x0F00) * alpha_scale) & 0x0F00;
  u16 blue_out = (u16)((color & 0x00F0) * alpha_scale) & 0x00F0;

  return red_out + green_out + blue_out + (color & 0x000F);
}

u16 alpha_blend_over(u16 color_a, u16 color_b)
{
  f32 alpha_a = color_a & 0x000F;
  f32 red_a = color_a & 0xF000;
  f32 green_a = color_a & 0x0F00;
  f32 blue_a = color_a & 0x00F0;
  f32 alpha_b = color_b & 0x000F;
  f32 red_b = color_b & 0xF000;
  f32 green_b = color_b & 0x0F00;
  f32 blue_b = color_b & 0x00F0;
      
  u16 alpha_out = (u16)(alpha_a + alpha_b * (1 - alpha_a / 16)) & 0x000F;
  u16 red_out = (u16)((red_a * alpha_a + red_b * alpha_b * (1 - alpha_a / 16)) / 16) & 0xF000;
  u16 green_out = (u16)((green_a * alpha_a + green_b * alpha_b * (1 - alpha_a / 16)) / 16) & 0x0F00;
  u16 blue_out = (u16)((blue_a * alpha_a + blue_b * alpha_b * (1 - alpha_a / 16)) / 16) & 0x00F0;

  return red_out + green_out + blue_out + alpha_out;
}
