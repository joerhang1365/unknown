#include "level.h"

char level_get_type(level level, int column, int row)
{
  return level.map[row * level.columns + column];
}

bool level_is_type(level level, float x, float y, char c)
{
  int column = floor(x / level.tile_size / SCREEN_SCALE);
  int row = floor(y / level.tile_size / SCREEN_SCALE);
  char tile = level_get_type(level, column, row);
  return (tile == c);
}

bool level_is_collision(level level, float left, float right, float top, float bottom)
{
  return
    level_is_type(level, left, top, '#') ||
    level_is_type(level, right, top, '#') ||
    level_is_type(level, right, bottom, '#') ||
    level_is_type(level, left, bottom, '#')||
    level_is_type(level, left, top, 'W') ||
    level_is_type(level, right, top, 'W') ||
    level_is_type(level, right, bottom, 'W') ||
    level_is_type(level, left, bottom, 'W')||
    level_is_type(level, left, top, 'r') ||
    level_is_type(level, right, top, 'r') ||
    level_is_type(level, right, bottom, 'r') ||
    level_is_type(level, left, bottom, 'r')||
    level_is_type(level, left, top, 'w') ||
    level_is_type(level, right, top, 'w') ||
    level_is_type(level, right, bottom, 'w') ||
    level_is_type(level, left, bottom, 'w');
}

bool level_is_next(level level, float x, float y)
{
  return 
    (level_is_type(level, x, y, '>')) ||
    (level_is_type(level, x, y, '^'));
}

vector2 level_get_position(level level, char c)
{
  vector2 position = {0, 0};
  for(int i = 0; i < level.columns; i++)
  {
    for(int j = 0; j < level.rows; j++)
    {
      char tile = level_get_type(level, i, j);
      if(tile == c)
      {
        position.i = i * level.tile_size * SCREEN_SCALE;
        position.j = j * level.tile_size * SCREEN_SCALE;
      }
    }
  }
  return position;
}

void level_change_map(level *level, char *map)
{
  for(int i = 0; i < level->rows * level->columns; i++)
  {
    level->map[i] = map[i];
  }
}

void level_next_text(level *level)
{
  if(level->text_index < level->text_size - 1)
  {
    level->text_index++;
  }
}

void level_load(level *level, char *source, SDL_Renderer *renderer)
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
  int mode = 0;
  char buffer[256];

  fscanf(in, "rows=%d\n", &rows);
  fscanf(in, "columns=%d\n", &columns);
  fscanf(in, "tile_size=%d\n", &tile_size);
  fscanf(in, "mode=%d\n", &mode);

  // messages
  fgets(buffer, sizeof(buffer), in);
  while(buffer[0] != '%' && index < LEVEL_TEXTS)
  {
    for(int i = 0; i < LEVEL_MESSAGE; i++)
    {
      level->message[index * LEVEL_MESSAGE + i] = buffer[i];
    }
    index++;
    level->text_size++;
    fgets(buffer, sizeof(buffer), in);
  }

  // map
  index = 0;
  while(index < LEVEL_MAP)
  {
    fgets(buffer, sizeof(buffer), in);
    for(int i = 1; i < columns + 1; i++)
    {
      level->map[index] = buffer[i];
      index++;
    }
  }
  level->map[index - 1] = '\0';

  fclose(in);

  level->rows = rows;
  level->columns = columns;
  level->tile_size = tile_size;
  level->mode = mode;

  // textures
  for(int i = 0; i < LEVEL_MAP; i++)
  {
    // flower
    if(level->textures[FLOWER] == NULL && level->map[i] == 'f')
    {
      texture_create(&level->textures[FLOWER], "images/flower.png", renderer);
      animator_create(&level->animations[FLOWER], level->textures[FLOWER], 16, 16, 2);
    }
    // grass
    if(level->textures[GRASS] == NULL && level->map[i] == 'l')
    {
      texture_create(&level->textures[GRASS], "images/grass.png", renderer);
      animator_create(&level->animations[GRASS], level->textures[GRASS], 16, 16, 4);
    }
    // water
    if(level->textures[WATER] == NULL && level->map[i] == 'w')
    {
      texture_create(&level->textures[WATER], "images/water.png", renderer);
      animator_create(&level->animations[WATER], level->textures[WATER], 16, 16, 4);
    }
    // button
    if(level->textures[BUTTON] == NULL && level->map[i] == 'b')
    {
      texture_create(&level->textures[BUTTON], "images/button.png", renderer);
      animator_create(&level->animations[BUTTON], level->textures[BUTTON], 16, 16, 2);
    }
    // rock
    if(level->textures[ROCK] == NULL && level->map[i] == 'r')
    {
      texture_create(&level->textures[ROCK], "images/rock.png", renderer);
    }
    // gravel
    if(level->textures[GRAVEL] == NULL && level->map[i] == 'g')
    {
      texture_create(&level->textures[GRAVEL], "images/gravel.png", renderer);
    }
    // wall
    if(level->textures[WALL] == NULL && level->map[i] == 'W')
    {
      texture_create(&level->textures[WALL], "images/wall.png", renderer);
    }
    // floor
    if(level->textures[FLOOR] == NULL && level->map[i] == 'F')
    {
      texture_create(&level->textures[FLOOR], "images/floor.png", renderer);
    }
    // up arrow
    if(level->textures[UP_ARROW] == NULL && level->map[i] == '^')
    {
      texture_create(&level->textures[UP_ARROW], "images/up_arrow.png", renderer);
    }
    // right arrow
    if(level->textures[RIGHT_ARROW] == NULL && level->map[i] == '>')
    {
      texture_create(&level->textures[RIGHT_ARROW], "images/right_arrow.png", renderer);
    }
    // spawn
    if(level->textures[SPAWN] == NULL && level->map[i] == 'p')
    {
      texture_create(&level->textures[SPAWN], "images/p.png", renderer);
    }
    // text
    if(level->textures[TEXT] == NULL && level->map[i] == 't')
    {
      texture_create(&level->textures[TEXT], "images/t.png", renderer);
    }
    // larry
    if(level->textures[LARRY] == NULL && level->map[i] == 'L')
    {
      texture_create(&level->textures[LARRY], "images/larry.png", renderer);
    }
  }

  // textbox
  for(int i = 0; i < level->text_size; i++)
  {
    char message[LEVEL_MESSAGE];
    for(int j = 0; j < LEVEL_MESSAGE; j++)
    {
      message[j] = level->message[i * LEVEL_MESSAGE + j];
    }
    message[LEVEL_MESSAGE - 1] = '\0';
    text_create(&level->text[i], message);
    if(level->textures[TEXT_BOX] == NULL)
    {
      texture_create(&level->textures[TEXT_BOX], "images/textbox.png", renderer);
    }
  }
}

void level_update_animation(level *level)
{
  // flower
  if(level->textures[FLOWER] != NULL)
  {
    animator_update_clip(&level->animations[FLOWER], 60);
  }
  // grass
  if(level->textures[GRASS] != NULL)
  {
    animator_update_clip(&level->animations[GRASS], 120);
  }
  // water
  if(level->textures[WATER] != NULL)
  {
    animator_update_clip(&level->animations[WATER], 30);
  }
  // button
  if(level->textures[BUTTON_TRIGGER] != NULL)
  {
    animator_set_frame(&level->animations[BUTTON], 1);
  }
}

void level_unload(level *level)
{
  // destroy textures
  for(int i = 0; i < LEVEL_TEXTURES; i++)
  {
    if(level->textures[i] != NULL)
    {
      texture_destroy(level->textures[i]);
    }
  }

  // destroy animations
  for(int i = 0; i < LEVEL_ANIMATIONS; i++)
  {
    animator_destroy(&level->animations[i]);
  }
}
