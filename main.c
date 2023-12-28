#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "alpha.h"
#include "constants.h"
#include "type.h"
#include "vector.h"
#include "texture.h"
#include "animator.h"
#include "lighting.h"
#include "text.h"
#include "corruption.h"

#define MAP_COUNT 6
#define SCREEN_MAX SCREEN_WIDTH * SCREEN_HEIGHT

struct 
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
  char *map_src[MAP_COUNT];
  u32 map_index;
  byte girl_show;
  veci2 camera;

  /* text */
  u32 text_size;
  text_t *texts;
  u32 text_index;
  byte text_show;
  font_t font;

  /* the corruptions */
  u32 corrupt_num;
  byte corrupt_time;
  corruption_t *corruptions;

} state;

char get_type(const u32 column, const u32 row)
{
  return state.map[row * state.columns + column];
}

veci2 get_position(const char c, const u32 instance_num)
{
  u32 instance = 0;
  for(u32 i = 0; i < state.columns; i++)
  {
    for(u32 j = 0; j < state.rows; j++)
    {
      char tile = get_type(i, j);
      if(tile == c)
      {
        if(instance == instance_num)
        {
          return veci2_create(i, j);
        }
        instance++;
      }
    }
  }
  return veci2_create(0, 0);
}

byte is_type(const f32 x, const f32 y, const char c)
{
  char tile = get_type(x / state.tile_size, y / state.tile_size);
  return (tile == c);
}

/* this for girl positions */
void veci2_leftshift(veci2 *array, veci2 new, const u32 size)
{
  for(i32 i = 0; i < size - 1; i++)
  {
    array[i] = array[i + 1];
  }

  array[size - 1] = new;
}

i32 state_load(const char *source)
{
  FILE *in = fopen(source, "r");
  if(in == NULL)
  {
    printf("cannot open file in tilemap\n");
    return 1;
  }

  fscanf(in, "columns=%u\n", &state.columns);
  fscanf(in, "rows=%u\n", &state.rows);
  fscanf(in, "tile_size=%u\n", &state.tile_size);
  fscanf(in, "girl_show=%hhu\n", &state.girl_show);
  fscanf(in, "corrupt_num=%u\n", &state.corrupt_num);
 
  /* map */
  state.map = malloc(sizeof(char) * state.columns * state.rows);
  if(state.map == NULL)
  {
    printf("error allocating memory to map\n");
    return 1;
  }
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
  state.text_index = 0;
  state.text_show = 0;
  fscanf(in, "text_size=%u\n", &state.text_size);
  state.texts = malloc(sizeof(text_t) * state.text_size);
  if(state.texts == NULL)
  {
    printf("failed to allocate memory to texts\n");
    return 1;
  }

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

  /* corruption */
  state.corruptions = malloc(sizeof(corruption_t) * state.corrupt_num);
  if(state.corruptions == NULL)
  {
    printf("failed to allocate memory to corruptions\n");
    return 1;
  }

  index = 0;
  while(index < state.corrupt_num)
  {
    state.corruptions[index].pos = get_position('C', index);
    index++;
  }

  /* camera */
  state.camera.x = 0;
  state.camera.y = 0;

  return 0;
}

#define PLAYER_WIDTH 8
#define PLAYER_HEIGHT 8
#define PLAYER_SPEED 2

struct
{
  u32 width;
  u32 height;
  veci2 pos;
  veci2 prev_pos[8];
  veci2 dir;
} player;


void player_load()
{
  player.width = PLAYER_WIDTH;
  player.height = PLAYER_HEIGHT;
  veci2 pos = get_position('p', 0);
  player.pos.x = pos.x * state.tile_size;
  player.pos.y = pos.y * state.tile_size;
  
  /* set all prev_pos to current pos */
  for(u32 i = 0; i < 8; i++)
  {
    player.prev_pos[i] = player.pos;
  }

  player.dir.x = 0;
  player.dir.y = 0;
}

byte player_is_touch(char c)
{
  return 
    is_type(player.pos.x, player.pos.y, c) ||
    is_type(player.pos.x + player.width - 1, player.pos.y, c) ||
    is_type(player.pos.x, player.pos.y + player.height - 1, c) ||
    is_type(player.pos.x + player.width - 1, player.pos.y + player.height - 1, c);
}

animator_t animations[ANIMATION_MAX];
texture_t textures[TEXTURE_MAX];

i32 main(i32 argc, char *argv[])
{
  /*
   #### ##    ## #### ######## ####    ###    ##       #### ######## ######## 
    ##  ###   ##  ##     ##     ##    ## ##   ##        ##       ##  ##       
    ##  ####  ##  ##     ##     ##   ##   ##  ##        ##      ##   ##       
    ##  ## ## ##  ##     ##     ##  ##     ## ##        ##     ##    ######   
    ##  ##  ####  ##     ##     ##  ######### ##        ##    ##     ##       
    ##  ##   ###  ##     ##     ##  ##     ## ##        ##   ##      ##       
   #### ##    ## ####    ##    #### ##     ## ######## #### ######## ########
  */

  if(SDL_Init(SDL_INIT_VIDEO > 0))
  {
    printf("failed to initialize video\n");
    return 1;
  }
  
  /* window */
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

  /* renderer */
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

  /* texture */
  state.texture = 
    SDL_CreateTexture(
        state.renderer,
        SDL_PIXELFORMAT_RGBA4444,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH,
        SCREEN_HEIGHT);
 
  /* map */
  state.map_src[0] = "maps/test.map";
  state.map_src[1] = "maps/start.map";
  state.map_src[2] = "maps/flower.map";
  state.map_src[3] = "maps/big.map";
  state.map_src[4] = "maps/meeting.map";
  state.map_src[5] = "maps/corruption.map";

  state.map_index = 0;
  state_load(state.map_src[state.map_index]);
  player_load();

  /* font */
  font_create(&state.font, 0xFFFF, "font_data");

  /* textures */
  texture_create("textures/player.txt", &textures[PLAYER_TXT]);
  texture_create("textures/tile.txt", &textures[TILE_TXT]);
  texture_create("textures/blank.txt", &textures[BLANK_TXT]);
  texture_create("textures/grass.txt", &textures[GRASS_TXT]);
  texture_create("textures/flower.txt", &textures[FLOWER_TXT]);
  texture_create("textures/wood.txt", &textures[WOOD_TXT]);
  texture_create("textures/text.txt", &textures[T_TXT]);
  texture_create("textures/next.txt", &textures[NEXT_TXT]);
  texture_create("textures/p.txt", &textures[P_TXT]);
  texture_create("textures/rock.txt", &textures[ROCK_TXT]);
  texture_create("textures/water.txt", &textures[WATER_TXT]);
  texture_create("textures/girl.txt", &textures[GIRL_TXT]);
  texture_create("textures/corruption.txt", &textures[CORRUPTION_TXT]);

  /* animations */
  animator_create(&animations[PLAYER_ANIM], textures[PLAYER_TXT], 8, 8, 5);
  animator_create(&animations[GRASS_ANIM], textures[GRASS_TXT], 8, 8, 4);
  animator_create(&animations[FLOWER_ANIM], textures[FLOWER_TXT], 8, 8, 2);
  animator_create(&animations[WATER_ANIM], textures[WATER_TXT], 8, 8, 2);
  animator_create(&animations[CORRUPTION_ANIM], textures[CORRUPTION_ANIM], 8, 8, 1);

  /*
   ######## ##    ## ########  
   ##       ###   ## ##     ## 
   ##       ####  ## ##     ## 
   ######   ## ## ## ##     ## 
   ##       ##  #### ##     ## 
   ##       ##   ### ##     ## 
   ######## ##    ## ########  
   */

  f32 time = 0;
  f32 frame_time = 0;
  f32 delta_time = 0;
  u32 previous_time = 0;
  u32 current_time = 0;

  while(!state.quit)
  {
    SDL_Event event;
    while(SDL_PollEvent(&event) != 0)
    {
      /*
       ##    ## ######## ##    ##    #### ##    ## ########  ##     ## ######## 
       ##   ##  ##        ##  ##      ##  ###   ## ##     ## ##     ##    ##    
       ##  ##   ##         ####       ##  ####  ## ##     ## ##     ##    ##    
       #####    ######      ##        ##  ## ## ## ########  ##     ##    ##    
       ##  ##   ##          ##        ##  ##  #### ##        ##     ##    ##    
       ##   ##  ##          ##        ##  ##   ### ##        ##     ##    ##    
       ##    ## ########    ##       #### ##    ## ##         #######     ##  
       */
      switch(event.type)
      {
        case SDL_QUIT: state.quit = 1; break;
        case SDL_KEYDOWN:
          switch(event.key.keysym.sym)
          {
            case SDLK_LEFT: state.key = LEFT; break;
            case SDLK_RIGHT: state.key = RIGHT; break;
            case SDLK_UP: state.key = UP; break;
            case SDLK_DOWN: state.key = DOWN; break;
            case SDLK_x: state.key = X; break;
            case SDLK_1: state.key = ONE; break;
            case SDLK_F1: state.key = F1; break;
            default: break;
          }
        break;
        case SDL_KEYUP: state.key = NONE; break;
        default: break;
      }
      /*
       ######## ##    ## ########  
       ##       ###   ## ##     ## 
       ##       ####  ## ##     ## 
       ######   ## ## ## ##     ## 
       ##       ##  #### ##     ## 
       ##       ##   ### ##     ## 
       ######## ##    ## ########  
      */
    }
    
    previous_time = current_time;
    current_time = SDL_GetTicks();
    delta_time = (current_time - previous_time) / 1000.0f;
    time += delta_time;
    frame_time += delta_time;

    if(frame_time >= (f32) 1 / FRAMERATE) 
    {
      frame_time = 0;
      
      /*
       ##     ## ########  ########     ###    ######## ######## 
       ##     ## ##     ## ##     ##   ## ##      ##    ##       
       ##     ## ##     ## ##     ##  ##   ##     ##    ##       
       ##     ## ########  ##     ## ##     ##    ##    ######   
       ##     ## ##        ##     ## #########    ##    ##       
       ##     ## ##        ##     ## ##     ##    ##    ##       
        #######  ##        ########  ##     ##    ##    ######## 
       */

      /* debugging */
      if(state.key == F1 &&
         state.debug == 0)
      {
        state.debug = 1;
        printf("debug mode activated\n");
      }

      if(state.debug)
      {
        if(state.key == ONE)
        {
          printf("enter map index: ");
          scanf("%u", &state.map_index);
          state_load(state.map_src[state.map_index]);
          player_load();
        }
      }

      /* player movement & girl movement */
      player.dir.x = 0;
      player.dir.y = 0;

      if(state.key == LEFT ||
         state.key == RIGHT ||
         state.key == UP ||
         state.key == DOWN)
      {
        switch(state.key)
        {
          case LEFT: 
            player.dir.x = -1; 
            animator_set_index(&animations[PLAYER_ANIM], 2);
            break;
          case RIGHT:
            player.dir.x = 1;
            animator_set_index(&animations[PLAYER_ANIM], 1);
            break;
          case UP:
            player.dir.y = -1;
            animator_set_index(&animations[PLAYER_ANIM], 3);
            break;
          default:
            player.dir.y = 1;
            animator_set_index(&animations[PLAYER_ANIM], 4);
        }
        if(state.girl_show == 1)
        {
          veci2_leftshift(player.prev_pos, player.pos, 8);
        }
      }
      else
      {
        animator_set_index(&animations[PLAYER_ANIM], 0);
      }

      player.pos.x += player.dir.x * PLAYER_SPEED;
      player.pos.y += player.dir.y * PLAYER_SPEED;

      /* player collison */
      if(player_is_touch('R') == 1||
         player_is_touch('w') == 1||
         player.pos.x < 0 ||
         player.pos.x / state.tile_size > state.columns - 1 ||
         player.pos.y < 0 ||
         player.pos.y / state.tile_size > state.rows - 1)
      {
        player.pos.x -= player.dir.x * PLAYER_SPEED;
        player.pos.y -= player.dir.y * PLAYER_SPEED;
      }

      /* animations */
      animator_update(&animations[GRASS_ANIM], 24);
      animator_update(&animations[FLOWER_ANIM], 24);
      animator_update(&animations[WATER_ANIM], 12);
      animator_update(&animations[CORRUPTION_ANIM], 48);

      /* corruption */
      state.corrupt_time++;
      
      if(state.corrupt_num > 0 &&
          state.corrupt_time > 48)
      {
        state.corrupt_time = 0;
        u32 index = 0;
        while(index < state.corrupt_num)
        {
          veci2 target = 
            veci2_create(player.pos.x / state.tile_size, player.pos.y / state.tile_size);
          corruption_update(&state.corruptions[index], target, state.map, state.columns);
          index++;
        }
      }

      /* camera */
      if(player.pos.x > state.camera.x + 12 * state.tile_size && 
          state.camera.x / state.tile_size < state.columns - 16)
      {
        state.camera.x += PLAYER_SPEED;
      }
      else if(player.pos.x < state.camera.x + 4 * state.tile_size && 
          state.camera.x > 0)
      {
        state.camera.x -= PLAYER_SPEED;
      }
      else if(player.pos.y > state.camera.y + 12 * state.tile_size && 
          state.camera.y / state.tile_size < state.rows - 16)
      {
        state.camera.y += PLAYER_SPEED;
      }
      else if(player.pos.y < state.camera.y + 4 * state.tile_size && 
          state.camera.y > 0)
      {
        state.camera.y -= PLAYER_SPEED;
      }

      /* text */
      if(player_is_touch('T'))
      {
        state.text_show = 1;
        if(state.key == X && state.text_index < state.text_size - 1)
        {
          state.text_index++;
          state.key = NONE;
        }
      }
      else
      {
        state.text_show = 0;
        state.text_index = 0;
      }

      /* next map */
      if(player_is_touch('>'))
      {
        state.map_index++;
        state_load(state.map_src[state.map_index]);
        player_load();
      }

      /*
       ######## ##    ## ########  
       ##       ###   ## ##     ## 
       ##       ####  ## ##     ## 
       ######   ## ## ## ##     ## 
       ##       ##  #### ##     ## 
       ##       ##   ### ##     ## 
       ######## ##    ## ########  
      */
    } 

    /*
     ########  ######## ##    ## ########  ######## ########  
     ##     ## ##       ###   ## ##     ## ##       ##     ## 
     ##     ## ##       ####  ## ##     ## ##       ##     ## 
     ########  ######   ## ## ## ##     ## ######   ########  
     ##   ##   ##       ##  #### ##     ## ##       ##   ##   
     ##    ##  ##       ##   ### ##     ## ##       ##    ##  
     ##     ## ######## ##    ## ########  ######## ##     ## 
     */

    /* clear screen */
    for(u32 i = 0; i < SCREEN_MAX; i++)
    {
      state.pixels[i] = 0x0000;
    }

    /* map */
    for(u32 i = state.camera.y / state.tile_size; i < state.columns; i++)
    {
      for(u32 j = state.camera.x / state.tile_size; j < state.rows; j++)
      {  
        const i32 x = j * state.tile_size - state.camera.x;
        const i32 y = i * state.tile_size - state.camera.y;
        switch(get_type(j, i))
        {
          case 't': texture_add(textures[TILE_TXT], x, y, state.pixels, SCREEN_WIDTH, SCREEN_HEIGHT); break;
          case 'g': animator_add(&animations[GRASS_ANIM], x, y, state.pixels, SCREEN_WIDTH, SCREEN_HEIGHT); break;
          case 'f': animator_add(&animations[FLOWER_ANIM], x, y, state.pixels, SCREEN_WIDTH, SCREEN_HEIGHT); break;
          case 'W': texture_add(textures[WOOD_TXT], x, y, state.pixels, SCREEN_WIDTH, SCREEN_HEIGHT); break;
          case 'T': texture_add(textures[T_TXT], x, y, state.pixels, SCREEN_WIDTH, SCREEN_HEIGHT); break;
          case '>': texture_add(textures[NEXT_TXT], x, y, state.pixels, SCREEN_WIDTH, SCREEN_HEIGHT); break;
          case 'p': texture_add(textures[P_TXT], x, y, state.pixels, SCREEN_WIDTH, SCREEN_HEIGHT); break;
          case 'R': texture_add(textures[ROCK_TXT], x, y, state.pixels, SCREEN_WIDTH, SCREEN_HEIGHT); break;
          case 'w': animator_add(&animations[WATER_ANIM], x, y, state.pixels, SCREEN_WIDTH, SCREEN_HEIGHT); break;
          case 'G': texture_add(textures[GIRL_TXT], x, y, state.pixels, SCREEN_WIDTH, SCREEN_HEIGHT); break;
          case 'C': texture_add(textures[CORRUPTION_TXT], x, y, state.pixels, SCREEN_WIDTH, SCREEN_HEIGHT); break;
          default: texture_add(textures[BLANK_TXT], x, y, state.pixels, SCREEN_WIDTH, SCREEN_HEIGHT);
        }
      }
    }

    /* player */
    animator_add(
        &animations[PLAYER_ANIM], 
        player.pos.x - state.camera.x, 
        player.pos.y - state.camera.y, 
        state.pixels, 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT);

    /* girl */
    if(state.girl_show)
    {
      texture_add(
          textures[GIRL_TXT], 
          player.prev_pos[0].x - state.camera.x,
          player.prev_pos[0].y - state.camera.y,
          state.pixels,
          SCREEN_WIDTH,
          SCREEN_HEIGHT);
    }

    /* lighting */
    flash_light(
        player.pos.x - state.camera.x,
        player.pos.y - state.camera.y,
        64,
        state.map,
        state.pixels, 
        SCREEN_WIDTH,
        SCREEN_HEIGHT);

    /* text */
    if(state.text_show)
    {
      text_render(
          state.texts[state.text_index], 
          state.font, 
          32, 
          98, 
          state.pixels, 
          SCREEN_WIDTH, 
          SCREEN_HEIGHT);
    }
 
    const u32 pitch = 2;
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

    /* pixel grid lines */
    SDL_SetRenderDrawColor(state.renderer, 0.0f, 0.0f, 0.0f, 0.0f);
    for (u32 i = 0; i < SCREEN_HEIGHT; i++) 
    {
      for(u32 j = 0; j < 4; j++)
      {
        SDL_RenderDrawLine(
          state.renderer, 
          0, 
          i * SCALE + j, 
          SCREEN_WIDTH * SCALE, 
          i * SCALE + j);
      }
    }
 
    /* draw to screen */
    SDL_RenderPresent(state.renderer);

    /*
     ######## ##    ## ########  
     ##       ###   ## ##     ## 
     ##       ####  ## ##     ## 
     ######   ## ## ## ##     ## 
     ##       ##  #### ##     ## 
     ##       ##   ### ##     ## 
     ######## ##    ## ########  
    */
  }

  /*
   ########  ########  ######  ######## ########   #######  ##    ## 
   ##     ## ##       ##    ##    ##    ##     ## ##     ##  ##  ##  
   ##     ## ##       ##          ##    ##     ## ##     ##   ####   
   ##     ## ######    ######     ##    ########  ##     ##    ##    
   ##     ## ##             ##    ##    ##   ##   ##     ##    ##    
   ##     ## ##       ##    ##    ##    ##    ##  ##     ##    ##    
   ########  ########  ######     ##    ##     ##  #######     ##    
   */

  /* textures */
  texture_destroy(textures[PLAYER_TXT]);
  texture_destroy(textures[TILE_TXT]);
  texture_destroy(textures[BLANK_TXT]);
  texture_destroy(textures[GRASS_TXT]);
  texture_destroy(textures[FLOWER_TXT]);
  texture_destroy(textures[NEXT_TXT]);
  texture_destroy(textures[P_TXT]);
  texture_destroy(textures[ROCK_TXT]);
  texture_destroy(textures[WATER_TXT]);
  texture_destroy(textures[GIRL_TXT]);
  texture_destroy(textures[CORRUPTION_TXT]);

  /* font */
  font_destroy(&state.font);

  /* state */
  SDL_DestroyRenderer(state.renderer);
  SDL_DestroyWindow(state.window);
  SDL_DestroyTexture(state.texture);
  free(state.map);
  state.renderer = NULL;
  state.window = NULL;
  state.texture = NULL;

  SDL_Quit();

  /*
   ######## ##    ## ########  
   ##       ###   ## ##     ## 
   ##       ####  ## ##     ## 
   ######   ## ## ## ##     ## 
   ##       ##  #### ##     ## 
   ##       ##   ### ##     ## 
   ######## ##    ## ########  
   */

  return 0;
}
