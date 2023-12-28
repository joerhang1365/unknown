#include "alpha.h"

u16 alpha_color(u16 color)
{
  return 
    ((u16)((color & 0xF000) * (color & 0x000F) / 16.0f) & 0xF000) + 
    ((u16)((color & 0x0F00) * (color & 0x000F) / 16.0f) & 0x0F00) +
    ((u16)((color & 0x00F0) * (color & 0x000F) / 16.0f) & 0x00F0) +
    (u16)(color & 0x000F);
}

u16 alpha_blend_over(u16 color_a, u16 color_b)
{
  return 
    ((u16)(((color_a & 0xF000) * (color_a & 0x000F) + (color_b & 0xF000) * (color_b & 0x000F) * (1 - (f32)(color_a & 0x000F) / 16)) / 16) & 0xF000) +
    ((u16)(((color_a & 0x0F00) * (color_a & 0x000F) + (color_b & 0x0F00) * (color_b & 0x000F) * (1 - (f32)(color_a & 0x000F) / 16)) / 16) & 0x0F00) +
    ((u16)(((color_a & 0x00F0) * (color_a & 0x000F) + (color_b & 0x00F0) * (color_b & 0x000F) * (1 - (f32)(color_a & 0x000F) / 16)) / 16) & 0x00F0) + 
    ((u16)((color_a & 0x000F)+ (color_a & 0x000F) * (1 - (f32)(color_a & 0x000F) / 16)) & 0x000F);
}
