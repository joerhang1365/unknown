#include "text.h"

// this is a fucking mess
i32 text_render(text_t text, texture_t alphabet_txt, const i32 x, const i32 y, u16 *pixels, const i32 pixels_width, const i32 pixels_max)
{
  i32 overflow = 0;
  i32 row = 0;
  i32 offset = 0;
  for(i32 i = 0; i < text.length; i++)
  {
    i32 value = 0;
    if(text.message[i] != ' ' && text.message[i] != '\\')
    { 
      // does cool thing to find letter texture
      value = text.message[i] - 'a' + 1;
    }
    if(text.message[i] == '\\')
    {
      row++;
      offset = i;
    }
    for(i32 j = 0; j < alphabet_txt.height; j++)
    {
      for(i32 k = 0; k < 4; k++)
      {
        const i32 pixel_index = (j + y + row * 6) * pixels_width + (k + x) + 4 * (i - offset);
        overflow = pixel_index > pixels_max;
        if(pixel_index >= 0 && pixel_index < pixels_max && (k + x) >= 0 && (k + x) < pixels_width)
        {
          pixels[pixel_index] = alphabet_txt.pixels[j * alphabet_txt.width + k + value * 4];
        }
      }
    }
  }
  return overflow;
}
