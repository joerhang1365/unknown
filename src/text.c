#include "text.h"
#include "state.h"

void font_create(font_t *font, const u16 color, const char *source)
{
  FILE *in = fopen(source, "r");
  ASSERT(in == NULL, "failed to open font data file\n");

  fscanf(in, "width=%u\n", &font->width);
  fscanf(in, "height=%u\n", &font->height);

  //if(font->data != NULL) free(font->data);
  font->data = malloc(sizeof(i32) * 27 * font->height);
  ASSERT(font->data == NULL, "failed to allocate memory to font data\n");
  
  for(u32 i = 0; i < 27 * font->height; i++)
  {
    // gets line and extract hex value
    char line[64];
    fgets(line, sizeof(line), in);
    sscanf(line, "%x", &font->data[i]);
  }

  fclose(in);

  font->color = color;
}

void font_destroy(font_t *font)
{
  free(font->data);
  font->data = NULL;
  font->color = 0;
  font->width = 0;
  font->height = 0;
}

// this is a fucking mess
i32 text_render(const text_t text, const font_t font, const i32 x, const i32 y)
{
  byte overflow = 0;
  u32 row = 0;
  u32 offset = 0;
  for(u32 i = 0; i < text.length; i++)
  {
    i32 value;
    if(text.message[i] == '\n')
    {
      row++;
      offset = i + 1;
      value = 0;
    }
    else if(text.message[i] == ' ') value = 0;
    else value = text.message[i] - 'a' + 1;

    for(u32 j = 0; j < font.height; j++)
    {
      byte data = font.data[value * font.height + j];
      for(u32 k = 0; k < font.width; k++)
      {
        const i32 pixels_index = (j + y + row * font.height) * SCREEN_WIDTH + (k + x) + 4 * (i - offset);
        overflow = pixels_index > SCREEN_MAX;
        u32 pixel;

        if(data > 0x7) pixel = font.color; // this only works for width 4
        else pixel = state.pixels[pixels_index];
        
        // left shift and only store last 4 bits
        data = (data << 1) & 0xF;
        state.pixels[pixels_index] = pixel;
      }
    }
  }
  return overflow;
}
