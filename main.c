#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "constants.h"
#include "type.h"
#include "vector.h"
#include "texture.h"
#include "animator.h"
#include "text.h"

struct 
{
  // SDL2 shit
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;

  // map
  i32 columns;
  i32 rows;
  i32 tile_size;
  char *map;
  i32 map_index;

  // text
  i32 text_size;
  text_t *texts;
  i32 text_index;
  bool text_show;

  u16 pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
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
  char buffer[128];
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

  // text
  state.text_index = 0;
  state.text_show = false;
  fscanf(in, "text_size=%d\n", &state.text_size);
  state.texts = malloc(sizeof(text_t) * state.text_size);
  if(state.texts == NULL)
  {
    printf("failed to allocate memory to texts\n");
    return 1;
  }

  for(i32 i = 0; i < state.text_size; i++)
  {
    char buffer;
    i32 length = 0;
    while((buffer = fgetc(in)) != ';')
    {
      state.texts[i].message[length] = buffer;
      length++;
    }
    state.texts[i].length = length;
    buffer = fgetc(in);
  }

  fclose(in);

  return 0;
}

void state_destroy()
{ 
  SDL_DestroyRenderer(state.renderer);
  SDL_DestroyWindow(state.window);
  SDL_DestroyTexture(state.texture);
  free(state.map);
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

  texture_t texture;
} player;


void player_load()
{
  player.width = PLAYER_WIDTH;
  player.height = PLAYER_HEIGHT;
  player.offset = 0;
  veci2 pos = get_position('p');
  player.pos.x = pos.x * state.tile_size;
  player.pos.y = pos.y * state.tile_size;
  player.dir.x = -1.0f;
  player.dir.y = 0.0f;
}

void player_movement() 
{
  player.dir.x = 0.0f;
  player.dir.y = 0.0f;

  if(state.key == LEFT) 
  {
    player.dir.x = -1.0f;
  }
  else if(state.key == RIGHT)
  {
    player.dir.x = 1.0f;
  }
  else if(state.key == UP)
  {
    player.dir.y = -1.0f;
  }
  else if(state.key == DOWN)
  {
    player.dir.y = 1.0f;
  }

  player.pos.x += player.dir.x * PLAYER_SPEED;
  player.pos.y += player.dir.y * PLAYER_SPEED;
}

void player_render()
{
  texture_add(
      player.texture, 
      player.pos.x, 
      player.pos.y, 
      state.pixels, 
      SCREEN_WIDTH, 
      SCREEN_WIDTH * SCREEN_HEIGHT);
}

animator_t animations[ANIMATION_MAX];
texture_t textures[TEXTURE_MAX];

void map_render()
{
  for(i32 i = 0; i < state.columns; i++)
  {
    for(i32 j = 0; j < state.rows; j++)
    {
      switch(get_type(j, i))
      {
        case 't':  texture_add(textures[TILE_TXT], j * state.tile_size, i * state.tile_size, state.pixels, SCREEN_WIDTH, SCREEN_WIDTH * SCREEN_HEIGHT); break;
        case 'g': animator_add(animations[GRASS_ANIM], j * state.tile_size, i * state.tile_size, state.pixels, SCREEN_WIDTH, SCREEN_WIDTH * SCREEN_HEIGHT); break;
        case 'f': animator_add(animations[FLOWER_ANIM], j * state.tile_size, i * state.tile_size, state.pixels, SCREEN_WIDTH, SCREEN_WIDTH * SCREEN_HEIGHT); break;
        case 'T': texture_add(textures[TEXT_TXT], j * state.tile_size, i * state.tile_size, state.pixels, SCREEN_WIDTH, SCREEN_WIDTH * SCREEN_HEIGHT); break;
        case '>': texture_add(textures[NEXT_TXT], j * state.tile_size, i * state.tile_size, state.pixels, SCREEN_WIDTH, SCREEN_WIDTH * SCREEN_HEIGHT); break;
        case 'p': texture_add(textures[P_TXT], j * state.tile_size, i * state.tile_size, state.pixels, SCREEN_WIDTH, SCREEN_WIDTH * SCREEN_HEIGHT); break;
        case 'R': texture_add(textures[ROCK_TXT], j * state.tile_size, i * state.tile_size, state.pixels, SCREEN_WIDTH, SCREEN_WIDTH * SCREEN_HEIGHT); break;
        default: texture_add(textures[BLANK_TXT], j * state.tile_size, i * state.tile_size, state.pixels, SCREEN_WIDTH, SCREEN_WIDTH * SCREEN_HEIGHT); break;
      }
    }
  }
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
 
  // initialize
  char *map_src[] =
  {
    "maps/test.map",
    "maps/start.map",
    "maps/flower.map"
  };
  state_load("maps/test.map");
  player_load();
  texture_t text_txt;
  texture_create("textures/alphabet.txt", &text_txt);

  // textures
  texture_create("textures/player.txt", &player.texture);
  texture_create("textures/tile.txt", &textures[TILE_TXT]);
  texture_create("textures/blank.txt", &textures[BLANK_TXT]);
  texture_create("textures/grass.txt", &textures[GRASS_TXT]);
  texture_create("textures/flower.txt", &textures[FLOWER_TXT]);
  texture_create("textures/text.txt", &textures[TEXT_TXT]);
  texture_create("textures/next.txt", &textures[NEXT_TXT]);
  texture_create("textures/p.txt", &textures[P_TXT]);
  texture_create("textures/rock.txt", &textures[ROCK_TXT]);

  // animations
  animator_create(&animations[GRASS_ANIM], textures[GRASS_TXT], 8, 8, 4);
  animator_create(&animations[FLOWER_ANIM], textures[FLOWER_TXT], 8, 8, 2);
 
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
      player_movement();

      animator_update(&animations[GRASS_ANIM], 16);
      animator_update(&animations[FLOWER_ANIM], 32);

      // text logic
      if(is_type(player.pos.x, player.pos.y, 'T'))
      {
        state.text_show = true;
        if(state.key == X && state.text_index < state.text_size - 1)
        {
          state.text_index++;
          state.key = NONE;
        }
      }
      else
      {
        state.text_show = false;
        state.text_index = 0;
      }

      // next logic
      if(is_type(player.pos.x, player.pos.y, '>'))
      {
        state.map_index++;
        state_load(map_src[state.map_index]);
        player_load();
      }
    }

    // clear screen
    for(i32 i = 0; i < SCREEN_WIDTH * SCREEN_WIDTH; i++)
    {
      state.pixels[i] = 0x0000;
    }

    // render 
    map_render();
    player_render();

    if(state.text_show == true)
    {
      text_render(state.texts[state.text_index], text_txt, 32, 98, state.pixels, SCREEN_WIDTH, SCREEN_WIDTH * SCREEN_HEIGHT);
    }
 
    const i32 pitch = 2;
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
    SDL_SetRenderDrawColor(state.renderer, 0.0f, 0.0f, 0.0f, 0.0f);
    for (i32 i = 0; i < SCREEN_HEIGHT; i++) 
    {
      for(i32 j = 0; j < SCALE / 2; j++)
      {
        SDL_RenderDrawLine(state.renderer, 0, i * SCALE + j, SCREEN_WIDTH * SCALE, i * SCALE + j);
      }
    }
 
    // draw to screen
    SDL_RenderPresent(state.renderer);
  }

  // destroy
  texture_destroy(player.texture);
  texture_destroy(textures[TILE_TXT]);
  texture_destroy(textures[BLANK_TXT]);
  texture_destroy(textures[GRASS_TXT]);
  texture_destroy(textures[FLOWER_TXT]);
  texture_destroy(textures[TEXT_TXT]);
  texture_destroy(textures[NEXT_TXT]);
  texture_destroy(textures[P_TXT]);
  texture_destroy(textures[ROCK_TXT]);
  state_destroy();
  SDL_Quit();

  return 0;
}
