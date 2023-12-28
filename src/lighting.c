#include "lighting.h"
#include "texture.h"

i32 flash_light(const i32 x, const i32 y, u32 radius, const char *map, u16 *pixels, const u32 pixels_width, const u32 pixels_height)
{
  i32 overflow = 0;
  const u32 pixels_max = pixels_width * pixels_height;
  u16 *temp_pixels = malloc(sizeof(u16) * pixels_max);

  // copy all the pixels
  for(u32 i = 0; i < pixels_max; i++)
  {
    temp_pixels[i] = pixels[i];
  }

  // convert all pixels to black
  for(u32 i = 0; i < pixels_max; i++)
  {
    pixels[i] = 0x0000;
  }

  for(f32 theta = 0; theta < 2 * PI; theta += (f32) PI / LIGHT_ACCURACY)
  {

    for(u32 r = 0; r < radius; r++)
    {
      const i32 cylindrical_x = r * cosf(theta) + x;
      const i32 cylindrical_y = r * sinf(theta) + y;
      const i32 pixels_index = cylindrical_y * pixels_width + cylindrical_x;
      overflow = pixels_index > pixels_max;

      // make light not pass through solid objects
      u32 column = cylindrical_x / 8;
      u32 row = cylindrical_y / 8;
      char type = ' ';
      // protect segfault
      if(column <= 16 && row <= 16)
      {
        type = map[row * 16 + column];
      }
      if(type == 'R')
      {
        break;
      }

      // is in screen boundary
      if(pixels_index >= 0 && 
        overflow == 0 && 
        cylindrical_x >= 0 && 
        cylindrical_x < pixels_width &&
        cylindrical_y >= 0 &&
        cylindrical_y < pixels_height)
      {
        // add only pixels in light to screen
        pixels[pixels_index] = temp_pixels[pixels_index];
      }
    }
  }

  free(temp_pixels);

  return overflow;
}
