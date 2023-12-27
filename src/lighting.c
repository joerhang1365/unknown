#include "lighting.h"

i32 spotlight_add(const i32 x, const i32 y, const u16 color, const u32 radius, const char *map, u16 *pixels, const u32 pixels_width, const u32 pixels_max)
{
  i32 overflow = 0;
  const u32 pixels_height = pixels_max / pixels_width;
  for(f32 i = 0; i < LIGHT_ACCURACY; i++)
  {
    for(u32 k = 0; k < radius; k++)
    {
      const f32 theta = 2.0f * PI * i / LIGHT_ACCURACY;
      const i32 cylindrical_x = k * cos(theta) + x;
      const i32 cylindrical_y = k * sin(theta) + y;
      const u32 pixels_index = cylindrical_y * pixels_width + cylindrical_x;
      overflow = pixels_index > pixels_max;

      if(pixels_index >= 0 && 
        overflow == 0 && 
        cylindrical_x >= 0 && 
        cylindrical_x < pixels_width &&
        cylindrical_y >= 0 &&
        cylindrical_y < pixels_height)
      {
        // over blend formula
        f32 alpha_a = (color & 0x000F) * (f32)(radius - k) / 16;
        f32 red_a = color & 0xF000;
        f32 green_a = color & 0x0F00;
        f32 blue_a = color & 0x00F0;
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
  return overflow;
}
