#include "tilemap.h"
#include "animator.h"
#include "constants.h"
#include "text.h"
#include "texture.h"
#include "vector.h"
#include <SDL2/SDL_render.h>

char tilemap_get_tile(tilemap tilemap, int column, int row)
{
  return tilemap.map[row * tilemap.columns + column];
}

bool tilemap_is_tile(tilemap tilemap, float x, float y, char c)
{
  int column = floor(x / tilemap.tile_size / SCREEN_SCALE);
  int row = floor(y / tilemap.tile_size / SCREEN_SCALE);
  char tile = tilemap_get_tile(tilemap, column, row);
  return (tile == c);
}

bool tilemap_is_collision(tilemap tilemap, float left, float right, float top, float bottom)
{
  return 
    tilemap_is_tile(tilemap, left, top, '#') ||
    tilemap_is_tile(tilemap, right, top, '#') ||
    tilemap_is_tile(tilemap, right, bottom, '#') ||
    tilemap_is_tile(tilemap, left, bottom, '#') ||
    tilemap_is_tile(tilemap, left, top, 'W') ||
    tilemap_is_tile(tilemap, right, top, 'W') ||
    tilemap_is_tile(tilemap, right, bottom, 'W') ||
    tilemap_is_tile(tilemap, left, bottom, 'W') ||
    tilemap_is_tile(tilemap, left, top, 'r') ||
    tilemap_is_tile(tilemap, right, top, 'r') ||
    tilemap_is_tile(tilemap, right, bottom, 'r') ||
    tilemap_is_tile(tilemap, left, bottom, 'r') ||
    tilemap_is_tile(tilemap, left, top, 'w') ||
    tilemap_is_tile(tilemap, right, top, 'w') ||
    tilemap_is_tile(tilemap, right, bottom, 'w') ||
    tilemap_is_tile(tilemap, left, bottom, 'w');
}

bool tilemap_next(tilemap tilemap, float x, float y)
{
  return 
    (tilemap_is_tile(tilemap, x, y, '>')) ||
    (tilemap_is_tile(tilemap, x, y, '^'));
}

vector2 tilemap_get_position(tilemap tilemap, char c)
{
  vector2 position = {0, 0};
  for(int i = 0; i < tilemap.columns; i++)
  {
    for(int j = 0; j < tilemap.rows; j++)
    {
      char tile = tilemap_get_tile(tilemap, i, j);
      if(tile == c)
      {
        position.i = i * tilemap.tile_size * SCREEN_SCALE;
        position.j = j * tilemap.tile_size * SCREEN_SCALE;
      }
    }
  }
  return position;
}

void tilemap_change_map(tilemap *tilemap, char *map)
{
  for(int i = 0; i < tilemap->rows * tilemap->columns; i++)
  {
    tilemap->map[i] = map[i];
  }
}

void tilemap_next_text(tilemap *tilemap)
{
  if(tilemap->text_index < tilemap->text_size - 1)
  {
    tilemap->text_index++;
  }
}

void tilemap_load(tilemap *tilemap, char *source, SDL_Renderer *renderer)
{
  FILE *in = fopen(source, "r");
  if(in == NULL)
  {
    printf("cannot open file in tilemap\n");
  }

  // info
  int index = 0;
  int rows = 0;
  int columns = 0;
  int tile_size = 0;
  char buffer[256];

  fscanf(in, "rows=%d\n", &rows);
  fscanf(in, "columns=%d\n", &columns);
  fscanf(in, "tile_size=%d\n", &tile_size);

  // messages
  fgets(buffer, sizeof(buffer), in);
  while(buffer[0] != '%' && index < TILEMAP_TEXTS)
  {
    for(int i = 0; i < TILEMAP_MESSAGE; i++)
    {
      tilemap->message[index * TILEMAP_MESSAGE + i] = buffer[i];
    }
    index++;
    tilemap->text_size++;
    fgets(buffer, sizeof(buffer), in);
  }

  // map
  index = 0;
  while(index < TILEMAP_MAP)
  {
    fgets(buffer, sizeof(buffer), in);
    for(int i = 1; i < columns + 1; i++)
    {
      tilemap->map[index] = buffer[i];
      index++;
    }
  }
  tilemap->map[index - 1] = '\0';

  printf("%s", tilemap->map);

  fclose(in);

  tilemap->rows = rows;
  tilemap->columns = columns;
  tilemap->tile_size = tile_size;

  // textures
  for(int i = 0; i < TILEMAP_MAP; i++)
  {
    // flower
    if(tilemap->textures[FLOWER] == NULL && tilemap->map[i] == 'f')
    {
      texture_create(&tilemap->textures[FLOWER], "images/flower.png", renderer);
      animator_create(&tilemap->animations[FLOWER], tilemap->textures[FLOWER], 16, 16, 2);
    }
    // grass
    if(tilemap->textures[GRASS] == NULL && tilemap->map[i] == 'l')
    {
      texture_create(&tilemap->textures[GRASS], "images/grass.png", renderer);
      animator_create(&tilemap->animations[GRASS], tilemap->textures[GRASS], 16, 16, 4);
    }
    // water
    if(tilemap->textures[WATER] == NULL && tilemap->map[i] == 'w')
    {
      texture_create(&tilemap->textures[WATER], "images/water.png", renderer);
      animator_create(&tilemap->animations[WATER], tilemap->textures[WATER], 16, 16, 4);
    }
    // button
    if(tilemap->textures[BUTTON] == NULL && tilemap->map[i] == 'b')
    {
      texture_create(&tilemap->textures[BUTTON], "images/button.png", renderer);
      animator_create(&tilemap->animations[BUTTON], tilemap->textures[BUTTON], 16, 16, 2);
    }
    // rock
    if(tilemap->textures[ROCK] == NULL && tilemap->map[i] == 'r')
    {
      texture_create(&tilemap->textures[ROCK], "images/rock.png", renderer);
    }
    // gravel
    if(tilemap->textures[GRAVEL] == NULL && tilemap->map[i] == 'g')
    {
      texture_create(&tilemap->textures[GRAVEL], "images/gravel.png", renderer);
    }
    // wall
    if(tilemap->textures[WALL] == NULL && tilemap->map[i] == 'W')
    {
      texture_create(&tilemap->textures[WALL], "images/wall.png", renderer);
    }
    // floor
    if(tilemap->textures[FLOOR] == NULL && tilemap->map[i] == 'F')
    {
      texture_create(&tilemap->textures[FLOOR], "images/floor.png", renderer);
    }
    // up arrow
    if(tilemap->textures[UP_ARROW] == NULL && tilemap->map[i] == '^')
    {
      texture_create(&tilemap->textures[UP_ARROW], "images/up_arrow.png", renderer);
    }
    // right arrow
    if(tilemap->textures[RIGHT_ARROW] == NULL && tilemap->map[i] == '>')
    {
      texture_create(&tilemap->textures[RIGHT_ARROW], "images/right_arrow.png", renderer);
    }
    // spawn
    if(tilemap->textures[SPAWN] == NULL && tilemap->map[i] == 'p')
    {
      texture_create(&tilemap->textures[SPAWN], "images/p.png", renderer);
    }
    // text
    if(tilemap->textures[TEXT] == NULL && tilemap->map[i] == 't')
    {
      texture_create(&tilemap->textures[TEXT], "images/t.png", renderer);
    }
    // larry
    if(tilemap->textures[LARRY] == NULL && tilemap->map[i] == 'L')
    {
      texture_create(&tilemap->textures[LARRY], "images/larry.png", renderer);
    }
  }

  // textbox
  for(int i = 0; i < tilemap->text_size; i++)
  {
    char message[TILEMAP_MESSAGE];
    for(int j = 0; j < TILEMAP_MESSAGE; j++)
    {
      message[j] = tilemap->message[i * TILEMAP_MESSAGE + j];
    }
    message[TILEMAP_MESSAGE - 1] = '\0';
    text_create(&tilemap->text[i], message);
    if(tilemap->textures[TEXT_BOX] == NULL)
    {
      texture_create(&tilemap->textures[TEXT_BOX], "images/textbox.png", renderer);
    }
  }
}

void tilemap_update_animation(tilemap *tilemap)
{
  // flower
  if(tilemap->textures[FLOWER] != NULL)
  {
    animator_update_clip(&tilemap->animations[FLOWER], 60);
  }
  // grass
  if(tilemap->textures[GRASS] != NULL)
  {
    animator_update_clip(&tilemap->animations[GRASS], 120);
  }
  // water
  if(tilemap->textures[WATER] != NULL)
  {
    animator_update_clip(&tilemap->animations[WATER], 30);
  }
  // button
  if(tilemap->textures[BUTTON_TRIGGER] != NULL)
  {
    animator_set_frame(&tilemap->animations[BUTTON], 1);
  }
}

void tilemap_render_map(tilemap tilemap, SDL_Renderer *renderer)
{
  for(int i = 0; i < tilemap.columns; i++)
  {
    for(int j = 0; j < tilemap.rows; j++)
    {
      char tile = tilemap_get_tile(tilemap, i, j);
      int x = i * tilemap.tile_size * SCREEN_SCALE;
      int y = j * tilemap.tile_size * SCREEN_SCALE;
      int width = tilemap.tile_size * SCREEN_SCALE;
      int height = tilemap.tile_size * SCREEN_SCALE;
      if(tile != ' ')
      {
        if(tile == 'f')
        {
          animator_render(tilemap.animations[FLOWER], renderer, x, y, width, height);
        }
        else if(tile == 'l')
        {
          animator_render(tilemap.animations[GRASS], renderer, x, y, width, height);
        }
        else if(tile == 'w')
        {
          animator_render(tilemap.animations[WATER], renderer, x, y, width, height);
        }
        else if(tile == 'b')
        {
          animator_render(tilemap.animations[BUTTON], renderer, x, y, width, height);
        }
        else if(tile == 'r')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, tilemap.textures[ROCK], NULL, &rect);
        }
        else if(tile == 'l')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, tilemap.textures[GRASS], NULL, &rect);
        }
        else if(tile == 'g')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, tilemap.textures[GRAVEL], NULL, &rect);
        }
        else if(tile == 'W')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, tilemap.textures[WALL], NULL, &rect);
        }
        else if(tile == 'F')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, tilemap.textures[FLOOR], NULL, &rect);
        }
        else if(tile == '^')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, tilemap.textures[UP_ARROW], NULL, &rect);
        }
        else if(tile == '>')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, tilemap.textures[RIGHT_ARROW], NULL, &rect);
        }
        else if(tile == 'p')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, tilemap.textures[SPAWN], NULL, &rect);
        }
        else if(tile == 't')
        {
          SDL_Rect rect = {x, y, width, height};
          SDL_RenderCopy(renderer, tilemap.textures[TEXT], NULL, &rect);
        }
        else if(tile == 'L')
        {
          SDL_Rect rect = {x, y, 32 * SCREEN_SCALE, 32 * SCREEN_SCALE};
          SDL_RenderCopy(renderer, tilemap.textures[LARRY], NULL, &rect);
        }
      }
    }
  }
}

void tilemap_render_text(tilemap tilemap, SDL_Renderer *renderer, TTF_Font *font, SDL_Color color)
{
  if(tilemap.triggers[TEXT_TRIGGER])
  {
    SDL_Rect rect = {32 * SCREEN_SCALE, 156 * SCREEN_SCALE, 190 * SCREEN_SCALE, 64 * SCREEN_SCALE};
    SDL_RenderCopy(renderer, tilemap.textures[TEXT_BOX], NULL, &rect);
    text_render(tilemap.text[tilemap.text_index], renderer, font, color, 36 * SCREEN_SCALE, 156 * SCREEN_SCALE, TILEMAP_TEXT_WIDTH, TILEMAP_TEXT_HEIGHT, TILEMAP_COLUMN);
  }
}

void tilemap_unload(tilemap *tilemap)
{
  // destroy textures
  for(int i = 0; i < TILEMAP_TEXTURES; i++)
  {
    if(tilemap->textures[i] != NULL)
    {
      texture_destroy(tilemap->textures[i]);
    }
  }

  // destroy animations
  for(int i = 0; i < TILEMAP_ANIMATIONS; i++)
  {
    animator_destroy(&tilemap->animations[i]);
  }
}
