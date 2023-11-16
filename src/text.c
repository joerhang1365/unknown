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
    if(text.message[i] != ' ' && text.message[i] != '\n')
    { 
      // does cool thing to find letter texture
      value = text.message[i] - 'a' + 1;
    }
    else if(text.message[i] == '\n')
    {
      row++;
      offset = i;
    }
    for(i32 j = 0; j < alphabet_txt.height; j++)
    {
      for(i32 k = 0; k < CHAR_WIDTH; k++)
      {
        const i32 pixel_index = (j + y + row * CHAR_HEIGHT) * pixels_width + (k + x) + CHAR_WIDTH * (i - offset);
        const i32 alphabet_txt_index = j * alphabet_txt.width + k + value * CHAR_WIDTH;
        overflow = pixel_index > pixels_max;

        if(pixel_index >= 0 && 
           pixel_index < pixels_max && 
           (k + x) >= 0 && 
           (k + x) < pixels_width && 
           alphabet_txt.pixels[alphabet_txt_index] != 0x0000)
        {
          pixels[pixel_index] = alphabet_txt.pixels[alphabet_txt_index];
        }
      }
    }
  }
  return overflow;
}
