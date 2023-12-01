#include "text.h"

void font_create(font *font, const u16 color, const char *source)
{
  FILE *in = fopen(source, "r");
  if(in == NULL)
  {
    printf("failed to open font data file\n");
  }

  fscanf(in, "width=%u\n", &font->width);
  fscanf(in, "height=%u\n", &font->height);

  font->data = malloc(sizeof(i32) * 27 * font->height);
  if(font->data == NULL)
  {
    printf("failed to allocate memory to font data\n");
  }

  for(i32 i = 0; i < 27 * font->height; i++)
  {
    // gets line and extract hex value
    char line[64];
    fgets(line, sizeof(line), in);
    sscanf(line, "%x", &font->data[i]);
  }

  fclose(in);

  font->color = color;
}

void font_destroy(font *font)
{
  free(font->data);
}

// this is a fucking mess
i32 text_render(const text_t text, const font font, const i32 x, const i32 y, u16 *pixels, const i32 pixels_width, const i32 pixels_max)
{
  i32 overflow = 0;
  i32 row = 0;
  i32 offset = 0;
  for(i32 i = 0; i < text.length; i++)
  {
    i32 value;
    if(text.message[i] == '\n')
    {
      row++;
      offset += i;
      value = 0;
    }
    else if(text.message[i] == ' ')
    {
      value = 0;
    }
    else 
    {
      value = text.message[i] - 'a' + 1;
    }

    for(i32 j = 0; j < font.height; j++)
    {
      u8 data = font.data[value * font.height + j];
      for(i32 k = 0; k < 4; k++)
      {
        const i32 pixel_index = (j + y + row * font.height) * pixels_width + (k + x) + 4 * (i - offset);
        u32 pixel;

        if(data > 0x7) // this only works for width 4
        {
          pixel = font.color;
        }
        else
        {
          pixel = pixels[pixel_index];
        }
        
        // left shift and only store last 4 bits
        data = (data << 1) & 0xF;
        pixels[pixel_index] = pixel;
      }
    }
  }
  return overflow;
}
