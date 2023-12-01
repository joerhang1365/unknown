#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>
#include <stdlib.h>
#include "type.h"

typedef struct
{
  u32 *data;
  u32 width;
  u32 height;
  u16 color;
} font;

void font_create(font *font, const u16 color, const char *source);
void font_destroy(font *font);

typedef struct
{
  char message[128];
  i32 length;
} text_t;

i32 text_render(const text_t text, const font font, const i32 x, const i32 y, u16 *pixels, const i32 pixels_width, const i32 pixels_max);

#endif
