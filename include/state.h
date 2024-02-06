#ifndef STATE_H
#define STATE_H

#include "globals.h"
#include "text.h"

typedef struct 
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
  u16 background_color;
  byte girl_show;
  u32 light;
  u32 weather;

  /* text */
  u32 text_size;
  text_t *texts;
} state_t;

extern state_t state;

enum WEATHER { CLEAR, RAIN }; 
enum LIGHT { LIGHT, DARK };

void state_load(const char *source);
void state_destroy();

static inline char get_type(const u32 column, const u32 row)
{
  return state.map[row * state.columns + column];
}

static inline byte is_type(const f32 x, const f32 y, const char c)
{
  char tile = get_type(floor(x / state.tile_size), floor(y / state.tile_size));
  return (tile == c);
}

static inline veci2 find_position(const char c, const u32 instance_num)
{
  u32 instance = 0;
  veci2 temp = veci2_create(0, 0);
  for (u32 i = 0; i < state.columns; i++)
  {
    for (u32 j = 0; j < state.rows; j++)
    {
      char tile = get_type(i, j);

      if (tile != c) continue;
      if (instance != instance_num)
      {
        instance++;
        continue;
      }

      temp = veci2_create(i, j);
      VECI2_SCALE(temp, state.tile_size);
      break;
    }
  }

  return temp;
}

#endif
