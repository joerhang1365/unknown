#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "constants.h"
#include "texture.h"

#define TEXT_MAX_CHARACTERS 512
#define TEXT_ROW_SPACE 48

int font_initialize(TTF_Font **font, char *source, int size);

typedef struct
{
  char message[TEXT_MAX_CHARACTERS];
  int length;
} text;

void text_create(text *text, char *message);
void text_render(text text, SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, float x, float y, float width, float height, int max_characters_per_column);

#endif
