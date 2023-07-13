#include "text.h"

int font_initialize(TTF_Font **font, char *source, int size) 
{
  int result = 0;
  if (TTF_Init() == -1) 
  {
    printf("failed to initiate ttf\n");
    result = -1;
  } 
  else 
  {
    *font = TTF_OpenFont(source, size);
    if (*font == NULL) 
    {
      printf("error creating font\n");
      result = -1;
    }
  }
  return result;
}

void text_create(text *text, char *message) 
{
  int index = 0;
  int length = 0;
  while (message[index] != '\0') 
  {
    text->message[index] = message[index];
    length++;
    index++;
  }
  text->length = length;
}

void text_render(text text, SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, float x, float y, float width, float height, int max_characters_per_column) {
  char line[max_characters_per_column];
  int index = 0;
  int row = 0;

  while (index < text.length) 
  {
    for (int column = 0; column < max_characters_per_column; column++) 
    {
      line[column] = text.message[index];
      index++;

      // make rest of row spaces at end of message
      if (index == text.length) 
      {
        column++;
        while (column < max_characters_per_column) 
        {
          line[column] = ' ';
          column++;
        }
      }
    }

    SDL_Surface *text_surface = TTF_RenderText_Solid(font, line, color);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_FreeSurface(text_surface);
    SDL_Rect rect = {x, row * TEXT_ROW_SPACE + y, width, height};
    SDL_RenderCopy(renderer, text_texture, NULL, &rect);
    texture_destroy(text_texture);
    row++;
  }
}
