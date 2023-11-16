#ifndef TEXT_H
#define TEXT_H

#include "type.h"
#include "texture.h"

#define CHAR_HEIGHT 6
#define CHAR_WIDTH 4

typedef struct
{
  char message[128];
  i32 length;
} text_t;

i32 text_render(text_t text, texture_t alphabet_txt, const i32 x, const i32 y, u16 *pixels, const i32 pixels_width, const i32 pixels_max);

#endif
