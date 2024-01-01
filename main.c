#include "particle.h"
#include "state.h"
#include "texture.h"
#include "animator.h"
#include "text.h"
#include "player.h"
#include "globals.h"
#include "lighting.h"
#include "corrupt.h"

#define MAP_COUNT 7
#define CAMERA_SPEED 64 * DELTA_TIME

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

  ASSERT(SDL_Init(SDL_INIT_VIDEO) > 0, "failed to initialize video\n");
  
  /* window */
  state.window = 
    SDL_CreateWindow(
        "unknown", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        SCREEN_WIDTH * SCALE, 
        SCREEN_HEIGHT * SCALE, 
        SDL_WINDOW_SHOWN);

  ASSERT(state.window == NULL, "failed to create window\n");

  /* renderer */
  state.renderer =
    SDL_CreateRenderer(
        state.window, 
        -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  ASSERT(state.renderer == NULL, "failed to create renderer\n");

  /* texture */
  state.texture = 
    SDL_CreateTexture(
        state.renderer,
        SDL_PIXELFORMAT_RGBA4444, 
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH, 
        SCREEN_HEIGHT);
 
  /* map */
  u32 map_index = 0;
  char *map_src[MAP_COUNT];
  map_src[0] = "maps/test.map";
  map_src[1] = "maps/start.map";
  map_src[2] = "maps/flower.map";
  map_src[3] = "maps/big.map";
  map_src[4] = "maps/meeting.map";
  map_src[5] = "maps/corruption.map";
  map_src[6] = "maps/restart.map";

  /* camera */
  VECi2(camera, 0, 0);

  state_load(map_src[map_index]);
  player_load();

  /* particles */
  particle_sim_t player_float_sim;
  particle_sim_create(&player_float_sim, 16);

  /* corruption */
  corruption_t corruption;
  corrupt_load(&corruption);

  /* font */
  font_t font;
  font_create(&font, 0xFFFF, "font_data");

  /* text */
  byte text_show = 0;
  u32 text_index = 0;

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

/*
  ######## ##    ## ########  
  ##       ###   ## ##     ## 
  ##       ####  ## ##     ## 
  ######   ## ## ## ##     ## 
  ##       ##  #### ##     ## 
  ##       ##   ### ##     ## 
  ######## ##    ## ########  
*/

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
    
    PREVIOUS_TIME = CURRENT_TIME;
    CURRENT_TIME = SDL_GetTicks();
    DELTA_TIME = (CURRENT_TIME - PREVIOUS_TIME) / 1000.0f;
    TIME += DELTA_TIME;
    FRAME_TIME += DELTA_TIME;

    if(FRAME_TIME >= (f32) 1 / FRAMERATE) 
    {
      FRAME_TIME = 0;
      
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
          scanf("%u", &map_index);
          state_load(map_src[map_index]);
          player_load(); 
          corrupt_load(&corruption);
          VECi2(camera, 0, 0);
        }
      }

      /* player movement */ 
      player.dir.x = 0;
      player.dir.y = 0;

      if(state.key != NONE)
      {
        switch(state.key)
        {
          case LEFT: player.dir.x = -1; break;
          case RIGHT: player.dir.x = 1; break;
          case UP: player.dir.y = -1; break;
          case DOWN: player.dir.y = 1; break;
          default: player.dir.x = 0; player.dir.y = 0;
        }
        if(state.girl_show == 1)
        {     
          LEFT_SHIFT(player.prev_pos, player.pos, PLAYER_PREVIOUS);
        }
      }

      player.pos.x += round(player.dir.x * PLAYER_SPEED);
      player.pos.y += round(player.dir.y * PLAYER_SPEED);

      /* player collision */
      if(player_touch('R') == 1 ||
         player_touch('w') == 1 ||
         player_touch('G') == 1 ||
         player.pos.x < 0 ||
         player.pos.x > (state.columns - 1) * state.tile_size ||
         player.pos.y < 1 ||
         player.pos.y > (state.rows - 1) * state.tile_size)
      {
        player.pos.x -= round(player.dir.x * PLAYER_SPEED);
        player.pos.y -= round(player.dir.y * PLAYER_SPEED);
      }

      /* player animation */
      if(state.key != NONE)
      {
        switch(state.key)
        {
          case LEFT: animator_set_index(&animations[PLAYER_ANIM], 2); break;
          case RIGHT: animator_set_index(&animations[PLAYER_ANIM], 1); break;
          case UP: animator_set_index(&animations[PLAYER_ANIM], 3); break;
          case DOWN: animator_set_index(&animations[PLAYER_ANIM], 4); break;
          default: animator_set_index(&animations[PLAYER_ANIM], 0);
        }
      }
      else
      {
        animator_set_index(&animations[PLAYER_ANIM], 0);
      }

      /* text */
      if(player_touch('T'))
      {
        text_show = 1;
        if(state.key == X && 
           text_index < state.text_size - 1)
        {
          text_index++;
          state.key = NONE;
        }
      }
      else
      {
        text_show = 0;
        text_index = 0;
      }

      /* next map */
      if(player_touch('>'))
      {
        map_index++;
        state_load(map_src[map_index]);
        player_load();
        corrupt_load(&corruption);
        VECi2(camera, 0, 0);
      }

      /* restart */
      if(player_touch('C'))
      {
        map_index = 0;
        state_load(map_src[MAP_COUNT - 1]);
        player_load();
        corrupt_load(&corruption);
        VECi2(camera, 0, 0);
      }

      /* animations */
      animator_update(&animations[GRASS_ANIM], 0.5f);
      animator_update(&animations[FLOWER_ANIM], 2.0f);
      animator_update(&animations[WATER_ANIM], 0.25f);

      /* player camera */
      if(player.pos.x > camera.x + 12 * state.tile_size &&
         camera.x < (state.columns - 16) * state.tile_size)
      {
        camera.x += round(CAMERA_SPEED);
      }
      else if(player.pos.x < camera.x + 4 * state.tile_size &&
              camera.x > 0)
      {
        camera.x -= round(CAMERA_SPEED);
      }
      else if(player.pos.y > camera.y + 12 * state.tile_size &&
         camera.y < (state.columns - 16) * state.tile_size)
      {
        camera.y += round(CAMERA_SPEED);
      }
      else if(player.pos.y < camera.y + 4  * state.tile_size &&
              camera.y > 0)
      {
        camera.y -= round(CAMERA_SPEED);
      }

      /* corruption */
      veci2 target;
      VECi2(target, player.pos.x, player.pos.y);
      corrupt_update(&corruption, target);

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

    SCREEN_CLEAR(state.pixels, SCREEN_MAX);

    /* map */
    for(u32 i = 0; i < SCREEN_MAX; i++)
    {
      state.pixels[i] = state.background_color;
    }

    for(u32 i = camera.y / state.tile_size; i < state.columns; i++)
    {
      for(u32 j = camera.x / state.tile_size; j < state.rows; j++)
      {  
        const i32 x = j * state.tile_size - camera.x;
        const i32 y = i * state.tile_size - camera.y;
        switch(get_type(j, i))
        {
          case 't': texture_add(textures[TILE_TXT], x, y); break;
          case 'g': animator_add(&animations[GRASS_ANIM], x, y); break;
          case 'f': animator_add(&animations[FLOWER_ANIM], x, y); break;
          case 'W': texture_add(textures[WOOD_TXT], x, y); break;
          case 'T': texture_add(textures[T_TXT], x, y); break;
          case '>': texture_add(textures[NEXT_TXT], x, y); break;
          case 'p': texture_add(textures[P_TXT], x, y); break;
          case 'R': texture_add(textures[ROCK_TXT], x, y); break;
          case 'w': animator_add(&animations[WATER_ANIM], x, y); break;
          case 'G': texture_add(textures[GIRL_TXT], x, y); break;
          case 'C': texture_add(textures[CORRUPTION_TXT], x, y); break;
          default: texture_add(textures[BLANK_TXT], x, y);
        }
      }
    }

    /* player */
    animator_add(
      &animations[PLAYER_ANIM], 
      player.pos.x - camera.x, 
      player.pos.y - camera.y);

    particle_float(
        &player_float_sim, 
        player.pos.x,
        player.pos.y,
        0x00FF,
        0.1f);

    /* girl */
    if(state.girl_show)
    {
      texture_add(
         textures[GIRL_TXT], 
         player.prev_pos[0].x - camera.x,
         player.prev_pos[0].y - camera.y);
    }

    /* lighting */
    flash_light(
        player.pos.x + (f32)PLAYER_WIDTH_2,
        player.pos.y + (f32)PLAYER_HEIGHT_2, 
        64);

    /* corruption glow */
    for(u32 i = 0; i < corruption.count; i++)
    {
      glow(
          corruption.corrupts[i].x,
          corruption.corrupts[i].y, 
          textures[CORRUPTION_TXT]);
    }

    /* text */
    if(text_show)
    {
      text_render(
          state.texts[text_index], 
          font, 
          32, 
          98);
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
    SDL_SetRenderDrawColor(
        state.renderer, 
        0.0f, 
        0.0f, 
        0.0f, 
        0.0f);

    for (u32 i = 0; i < SCREEN_HEIGHT; i++) 
    {
      for(u32 j = 0; j < SCALE - 1; j++)
      {
        SDL_RenderDrawLine(
          state.renderer, 0, 
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

  /* texture */
  texture_destroy(&textures[PLAYER_TXT]);
  texture_destroy(&textures[TILE_TXT]);
  texture_destroy(&textures[BLANK_TXT]);
  texture_destroy(&textures[GRASS_TXT]);
  texture_destroy(&textures[FLOWER_TXT]);
  texture_destroy(&textures[NEXT_TXT]);
  texture_destroy(&textures[P_TXT]);
  texture_destroy(&textures[ROCK_TXT]);
  texture_destroy(&textures[WATER_TXT]);
  texture_destroy(&textures[GIRL_TXT]);
  texture_destroy(&textures[CORRUPTION_TXT]);
  
  font_destroy(&font);
  state_destroy();
  corrupt_destroy(&corruption);
  particle_sim_destroy(&player_float_sim);
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
