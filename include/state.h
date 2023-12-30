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
  byte girl_show;

  /* text */
  u32 text_size;
  text_t *texts;

} state_t;

extern state_t state;

void state_load(const char *source);
char get_type(const u32 column, const u32 row);
byte is_type(const f32 x, const f32 y, const char c);
veci2 find_position(const char c, const u32 instance_num);

#endif
