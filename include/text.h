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
} font_t;

void font_create(font_t *font, const u16 color, const char *source);
void font_destroy(font_t *font);

typedef struct
{
  char message[128];
  i32 length;
} text_t;

i32 text_render(const text_t text, const font_t font, const i32 x, const i32 y, u16 *pixels, const u32 pixels_width, const u32 pixels_height);

#endif
