#ifndef STATE_H
#define STATE_H

#include "globals.h"
#include "text.h"
#include "camera.h"

#define MAP_SIZE 9

typedef struct 
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;

  u16 pixels[SCREEN_MAX];
  enum KEYS key;

  byte debug;
  byte quit;
  byte button;

  /* map */
  char *map_src[MAP_SIZE];
  u32 map_index;
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
  font_t font;
  byte text_show;
  u32 text_index;

  camera_t camera;

} state_t;

extern state_t state;

enum WEATHER { CLEAR, RAIN, WIND }; 
enum LIGHT { LIGHT, DARK };

void maps_create();
void maps_destroy();
void state_load();
void state_destroy();

static inline char get_type(const u32 column, const u32 row)
{
  if (column > state.columns || 
      column < 0 ||
      row > state.rows ||
      row < 0)
  {
    printf("out of index for map\n");
    return '\0';
  }

  return state.map[row * state.columns + column];
}

static inline byte is_type(const f32 x, const f32 y, const char c)
{
  char tile = get_type(floor(x / state.tile_size), floor(y / state.tile_size));
  return (tile == c);
}

static inline byte is_tile(const f32 x, const f32 y, const u32 width, const u32 height, const char c)
{
  return is_type(x, y, c) ||
         is_type(x + width - 1, y, c) ||
         is_type(x, y + height - 1, c) ||
         is_type(x + width - 1, y + height - 1, c);
}

static inline vec2 find_position(const char c, const u32 instance_num)
{
  u32 instance = 0;
  vec2 temp = vec2_create(0, 0);
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

      temp = vec2_create(i, j);
      vec2_scale(&temp, state.tile_size);
      break;
    }
  }

  return temp;
}

#endif
