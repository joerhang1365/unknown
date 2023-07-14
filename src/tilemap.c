#include "tilemap.h"

void tilemap_render_map(level level, SDL_Renderer *renderer)
{
  for(int i = 0; i < level.columns; i++)
  {
    for(int j = 0; j < level.rows; j++)
    {
      char tile = level_get_type(level, i, j);
      int x = i * level.tile_size * SCREEN_SCALE;
      int y = j * level.tile_size * SCREEN_SCALE;
      int width = level.tile_size * SCREEN_SCALE;
      int height = level.tile_size * SCREEN_SCALE;
      if(tile != ' ')
      {
        if(tile == 'f')
        {
          animator_render(level.animations[FLOWER], renderer, x, y, width, height);
        }
        else if(tile == 'l')
        {
          animator_render(level.animations[GRASS], renderer, x, y, width, height);
        }
        else if(tile == 'w')
        {
          animator_render(level.animations[WATER], renderer, x, y, width, height);
        }
        else if(tile == 'b')
        {
          animator_render(level.animations[BUTTON], renderer, x, y, width, height);
        }
        else if(tile == 'r')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, level.textures[ROCK], NULL, &rect);
        }
        else if(tile == 'l')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, level.textures[GRASS], NULL, &rect);
        }
        else if(tile == 'g')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, level.textures[GRAVEL], NULL, &rect);
        }
        else if(tile == 'W')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, level.textures[WALL], NULL, &rect);
        }
        else if(tile == 'F')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, level.textures[FLOOR], NULL, &rect);
        }
        else if(tile == '^')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, level.textures[UP_ARROW], NULL, &rect);
        }
        else if(tile == '>')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, level.textures[RIGHT_ARROW], NULL, &rect);
        }
        else if(tile == 'p')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, level.textures[SPAWN], NULL, &rect);
        }
        else if(tile == 't')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, level.textures[TEXT], NULL, &rect);
        }
        else if(tile == 'L')
        {
          SDL_Rect rect = {x, y, 32 * SCREEN_SCALE, 32 * SCREEN_SCALE};
          SDL_RenderCopy(renderer, level.textures[LARRY], NULL, &rect);
        }
      }
    }
  }
}

void tilemap_render_text(level level, SDL_Renderer *renderer, TTF_Font *font, SDL_Color color)
{
  if(level.triggers[TEXT_TRIGGER])
  {
    SDL_Rect rect = {32 * SCREEN_SCALE, 156 * SCREEN_SCALE, 190 * SCREEN_SCALE, 64 * SCREEN_SCALE};
    SDL_RenderCopy(renderer, level.textures[TEXT_BOX], NULL, &rect);
    text_render(level.text[level.text_index], renderer, font, color, 36 * SCREEN_SCALE, 156 * SCREEN_SCALE, LEVEL_TEXT_WIDTH, LEVEL_TEXT_HEIGHT, LEVEL_COLUMN);
  }
}
