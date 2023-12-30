#include "state.h"

state_t state;

void state_load(const char *source)
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

void state_destroy()
{
  free(state.map);
  free(state.texts);
  state.map = NULL;
  state.texts = NULL;
}
