#ifndef STATE_H
#define STATE_H

#include <SDL2/SDL.h>
#include "globals.h"
#include "text.h"

static struct 
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;

  u16 pixels[SCREEN_MAX];
  enum KEYS key;

  byte debug;
  byte quit;

  /* map */
  u32 columns;
  u32 rows;
  u32 tile_size;
  char *map;
  byte girl_show;

  /* text */
  u32 text_size;
  text_t *texts;

} state;

static inline char get_type(const u32 column, const u32 row)
{
  return state.map[row * state.columns + column];
}

static inline byte is_type(const f32 x, const f32 y, const char c)
{
  char tile = get_type(x / state.tile_size, y / state.tile_size);
  return (tile == c);
}

static inline veci2 find_position(const char c, const u32 instance_num)
{
  u32 instance = 0;
  veci2 temp;
  VECi2(temp, 0, 0);
  for(u32 i = 0; i < state.columns; i++)
  {
    for(u32 j = 0; j < state.rows; j++)
    {
      char tile = get_type(i, j);
      if(tile == c)
      {
        if(instance == instance_num)
        {
          VECi2(temp, i, j);
          break;
        }
        instance++;
      }
    }
  }
  return temp;
}

static inline void state_load(const char *source)
{
  FILE *in = fopen(source, "r");
  ASSERT(in == NULL, "cannot open file in tilemap\n");

  fscanf(in, "columns=%u\n", &state.columns);
  fscanf(in, "rows=%u\n", &state.rows);
  fscanf(in, "tile_size=%u\n", &state.tile_size);
  fscanf(in, "girl_show=%hhu\n", &state.girl_show);
 
  /* map */
  state.map = malloc(sizeof(char) * state.columns * state.rows);
  ASSERT(state.map == NULL, "error allocating memory to map\n");

  char buffer[128];
  u32 index = 0;
  u32 jndex = 0;
  while(index < state.columns * state.rows)
  {
    fgets(buffer, sizeof(buffer), in);
    jndex = 1;
    while(jndex < state.columns + 1)
    {
      state.map[index] = buffer[jndex];
      jndex++;
      index++;
    }
  }
  state.map[index - 1] = '\0';

  /* text */
  fscanf(in, "text_size=%u\n", &state.text_size);
  state.texts = malloc(sizeof(text_t) * state.text_size);
  ASSERT(state.texts == NULL, "failed to allocate memory to texts\n");

  for(u32 i = 0; i < state.text_size; i++)
  {
    char buffer;
    u32 length = 0;
    while((buffer = fgetc(in)) != ';')
    {
      state.texts[i].message[length] = buffer;
      length++;
    }
    state.texts[i].length = length;
    buffer = fgetc(in);
  }

  fclose(in);
}

#endif
