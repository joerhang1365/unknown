#include "state.h"

state_t state;

void maps_create()
{
  for (u32 i = 0; i < MAP_SIZE; i++)
  {
    size_t bytes = snprintf(NULL, 0, "maps/%u.map", i) + 1;
    if (state.map_src[i] == NULL)
      state.map_src[i] = malloc(bytes);
    ASSERT(state.map_src[i] == NULL, "error allocating memory to map\n");
    snprintf(state.map_src[i], bytes, "maps/%u.map", i); 
  }
}

void maps_destroy()
{
  for (u32 i = 0; i < MAP_SIZE; i++)
  {
    free(state.map_src[i]);
    state.map_src[i] = NULL;
  }
}

void state_load() 
{
  FILE *in = fopen(state.map_src[state.map_index], "r");
  ASSERT(in == NULL, "cannot open file in tilemap\n");

  fscanf(in, "columns=%u\n", &state.columns);
  fscanf(in, "rows=%u\n", &state.rows);
  fscanf(in, "tile_size=%u\n", &state.tile_size);
  fscanf(in, "background_color=%x\n", (u32 *)&state.background_color);
  fscanf(in, "light=%u\n", &state.light);
  fscanf(in, "weather=%u\n", &state.weather);
  fscanf(in, "girl_show=%hhu\n", &state.girl_show);

  /* map */
  if (state.map != NULL) free(state.map);
  state.map = malloc(sizeof(char) * state.columns * state.rows);
  ASSERT(state.map == NULL, "error allocating memory to map\n");

  char buffer[128];
  u32 index = 0;
  u32 jndex = 0;
  while (index < state.columns * state.rows) 
  {
    fgets(buffer, sizeof(buffer), in);
    jndex = 1;
    while (jndex < state.columns + 1) 
    {
      state.map[index] = buffer[jndex];
      jndex++;
      index++;
    }
  }
  state.map[index - 1] = '\0';

  /* text */
  if (state.texts != NULL) free(state.texts);
  fscanf(in, "text_size=%u\n", &state.text_size);
  state.texts = malloc(sizeof(text_t) * state.text_size);
  ASSERT(state.texts == NULL, "failed to allocate memory to texts\n");

  for (u32 i = 0; i < state.text_size; i++) 
  {
    char buffer;
    u32 length = 0;
    while ((buffer = fgetc(in)) != ';') 
    {
      state.texts[i].message[length] = buffer;
      length++;
    }
    state.texts[i].length = length;
    buffer = fgetc(in);
  }

  fclose(in);

  state.button = 0;
  state.key = NONE;
  state.text_show = 0;
  state.text_index = 0;

  state.camera = vec2_create(0, 0);
}

void state_destroy() 
{
  SDL_DestroyRenderer(state.renderer);
  SDL_DestroyWindow(state.window);
  SDL_DestroyTexture(state.texture);
  state.renderer = NULL;
  state.window = NULL;
  state.texture = NULL;
  free(state.map);
  free(state.texts);
  state.map = NULL;
  state.texts = NULL;
  state.key = 0;
  state.quit = 0;
  state.debug = 0;
  state.rows = 0;
  state.columns = 0;
  state.girl_show = 0;
  state.text_size = 0;
  state.background_color = 0;
  state.text_index = 0;
  state.text_show = 0;
  state.key = 0;
  state.button = 0;
  state.map_index = 0;

  maps_destroy();

  for (u32 i = 0; i < SCREEN_MAX; i++) 
  {
    state.pixels[i] = 0;
  }
}
