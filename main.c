#include <SDL2/SDL_pixels.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "constants.h"
#include "type.h"
#include "vector.h"
#include "texture.h"

struct 
{
  // SDL2 stuff
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;

  // levels
  i32 columns;
  i32 rows;
  i32 tile_size;
  char *map;

  u32 pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
  enum KEYS key;

  bool quit;
} state;

i32 state_load(const char *source)
{
  FILE *in = fopen(source, "r");
  if(in == NULL)
  {
    printf("cannot open file in tilemap\n");
    return 1;
  }

  fscanf(in, "columns=%d\n", &state.columns);
  fscanf(in, "rows=%d\n", &state.rows);
  fscanf(in, "tile_size=%d\n", &state.tile_size);
 
  // map
  state.map = (char*) malloc(sizeof(char) * state.columns * state.rows);
  if(state.map == NULL)
  {
    printf("error allocating memory to map\n");
    return 1;
  }
  char buffer[64];
  i32 index = 0;
  i32 jndex = 0;
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

  fclose(in);

  return 0;
}

void state_destroy()
{ 
  SDL_DestroyRenderer(state.renderer);
  SDL_DestroyWindow(state.window);
  SDL_DestroyTexture(state.texture);
  state.renderer = NULL;
  state.window = NULL;
  state.texture = NULL;
}

char get_type(const i32 column, const i32 row)
{
  return state.map[row * state.columns + column];
}

veci2 get_position(const char c)
{
  veci2 position;
  for(i32 i = 0; i < state.columns; i++)
  {
    for(i32 j = 0; j < state.rows; j++)
    {
      char tile = get_type(i, j);
      if(tile == c)
      {
        position.x = i;
        position.y = j;
      }
    }
  }
  return position;
}

bool is_type(const f32 x, const f32 y, const char c)
{
  i32 column = floor(x / state.tile_size);
  i32 row = floor(y / state.tile_size);
  char tile = get_type(column, row);
  return (tile == c);
}

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 16
#define PLAYER_SPEED 2

struct
{
  i32 width;
  i32 height;
  i32 offset;
  vec2 pos;
  vec2 dir;

} player;


void player_load(f32 x, f32 y)
{
  player.width = PLAYER_WIDTH;
  player.height = PLAYER_HEIGHT;
  player.offset = 0;
  player.pos.x = x;
  player.pos.y = y;
  player.dir.x = -1.0f;
  player.dir.y = 0.0f;
}

void player_tilemap_movement(u32 key) 
{
  player.dir.x = 0.0f;
  player.dir.y = 0.0f;

  if(key == LEFT) 
  {
    player.dir.x = -1.0f;
  }
  else if(key == RIGHT)
  {
    player.dir.x = 1.0f;
  }
  else if(key == UP)
  {
    player.dir.y = -1.0f;
  }
  else if(key == DOWN)
  {
    player.dir.y = 1.0f;
  }

  player.pos.x += player.dir.x * PLAYER_SPEED;
  player.pos.y += player.dir.y * PLAYER_SPEED;
}


i32 main(i32 argc, char *argv[])
{
  if(SDL_Init(SDL_INIT_VIDEO > 0))
  {
    printf("failed to initialize video\n");
    return 1;
  }
  
  state.window = 
    SDL_CreateWindow(
        "unknown", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        SCREEN_WIDTH * SCALE, 
        SCREEN_HEIGHT * SCALE, 
        SDL_WINDOW_SHOWN);
  if(state.window == NULL) 
  {
    printf("failed to create window\n");
    return 1;
  }

  state.renderer =
    SDL_CreateRenderer(
        state.window, 
        -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(state.renderer == NULL) 
  {
    printf("failed to create renderer\n");
    return 1;
  }

  state.texture = 
    SDL_CreateTexture(
        state.renderer,
        SDL_PIXELFORMAT_RGBA4444,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH,
        SCREEN_HEIGHT);
  
  // player
  player_load(10, 10);

  texture_t player_tex;
  texture_create("textures/player.txt", &player_tex);
 
  f32 time = 0;
  f32 frame_time = 0;
  f32 delta_time = 0;
  i32 previous_time = 0;
  i32 current_time = 0;
  while(!state.quit)
  {
    SDL_Event event;
    while(SDL_PollEvent(&event) != 0)
    {
      switch(event.type)
      {
        case SDL_QUIT: state.quit = true; break;
        case SDL_KEYDOWN:
          switch(event.key.keysym.sym)
          {
            case SDLK_LEFT: state.key = LEFT; break;
            case SDLK_RIGHT: state.key = RIGHT; break;
            case SDLK_UP: state.key = UP; break;
            case SDLK_DOWN: state.key = DOWN; break;
            case SDLK_x: state.key = X; break;
            default: break;
          }
          break;
        case SDL_KEYUP: state.key = NONE; break;
        default: break;
      }
    }

    previous_time = current_time;
    current_time = SDL_GetTicks();
    delta_time = (current_time - previous_time) / 1000.0f;
    time += delta_time;
    frame_time += delta_time;
    if (frame_time >= (f32)1 / FRAMERATE) 
    {
      frame_time = 0;
      // update 

    }

    // clear screen
    for(i32 i = 0; i < SCREEN_WIDTH * SCREEN_WIDTH; i++)
    {
      state.pixels[i] = 0x0000000;
    }

    // render
    texture_add(player_tex, 100, 10, state.pixels, SCREEN_WIDTH, SCREEN_HEIGHT);

    const i32 pitch = 4;
    SDL_UpdateTexture(
        state.texture, 
        NULL, 
        state.pixels, 
        SCREEN_WIDTH * pitch);
    SDL_Rect rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(
        state.renderer,
        state.texture,
        &rect,
        NULL);

    // pixel grid lines
    if(SCALE > 1)
    {
      SDL_SetRenderDrawColor(state.renderer, 0.0f, 0.0f, 0.0f, 0.0f);
      for (i32 i = 0; i < SCREEN_HEIGHT * SCALE; i += SCALE) 
      {
        SDL_RenderDrawLine(state.renderer, 0, i, SCREEN_WIDTH * SCALE, i);
        SDL_RenderDrawLine(state.renderer, i, 0, i, SCREEN_HEIGHT * SCALE);
      }
    }

    // draw to screen
    SDL_RenderPresent(state.renderer);
  }

  state_destroy();
  SDL_Quit();

  return 0;
}
