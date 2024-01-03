#include "animator.h"
#include "corrupt.h"
#include "globals.h"
#include "lighting.h"
#include "particle.h"
#include "player.h"
#include "state.h"
#include "text.h"
#include "texture.h"

#define MAP_COUNT 7
#define CAMERA_SPEED 64 * DELTA_TIME
#define SCREEN_TILES SCREEN_WIDTH / state.tile_size

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
  state.window = SDL_CreateWindow("unknown", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * SCALE,
                                  SCREEN_HEIGHT * SCALE, SDL_WINDOW_SHOWN);

  ASSERT(state.window == NULL, "failed to create window\n");

  /* renderer */
  state.renderer = SDL_CreateRenderer(
      state.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  ASSERT(state.renderer == NULL, "failed to create renderer\n");

  /* texture */
  state.texture = SDL_CreateTexture(state.renderer, SDL_PIXELFORMAT_RGBA4444,
                                    SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
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
  camera = veci2_create(0, 0);

  state_load(map_src[map_index]);
  player_load();

  /* particles */
  particle_sim_t player_float_sim;
  particle_sim_t rain_sim;
  particle_sim_create(&player_float_sim, 16);
  particle_sim_create(&rain_sim, 128);

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
  texture_create("textures/player.txt", PLAYER_TXT);
  texture_create("textures/tile.txt", TILE_TXT);
  texture_create("textures/blank.txt", BLANK_TXT);
  texture_create("textures/grass.txt", GRASS_TXT);
  texture_create("textures/flower.txt", FLOWER_TXT);
  texture_create("textures/wood.txt", WOOD_TXT);
  texture_create("textures/text.txt", T_TXT);
  texture_create("textures/next.txt", NEXT_TXT);
  texture_create("textures/p.txt", P_TXT);
  texture_create("textures/rock.txt", ROCK_TXT);
  texture_create("textures/water.txt", WATER_TXT);
  texture_create("textures/girl.txt", GIRL_TXT);
  texture_create("textures/corruption.txt", CORRUPTION_TXT);

  /* animations */
  animator_create(8, 8, 5, PLAYER_ANIM, PLAYER_TXT);
  animator_create(8, 8, 4, GRASS_ANIM, GRASS_TXT);
  animator_create(8, 8, 2, FLOWER_ANIM, FLOWER_TXT);
  animator_create(8, 8, 2, WATER_ANIM, WATER_TXT);

  while (!state.quit) 
  {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) 
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
      switch (event.type) 
      {
      case SDL_QUIT: state.quit = 1; break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) 
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
    }

    PREVIOUS_TIME = CURRENT_TIME;
    CURRENT_TIME = SDL_GetTicks();
    DELTA_TIME = (CURRENT_TIME - PREVIOUS_TIME) / 1000.0f;
    TIME += DELTA_TIME;
    FRAME_TIME += DELTA_TIME;

    if (FRAME_TIME >= (f32)1 / FRAMERATE) 
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
      if (state.key == F1 && state.debug == 0) 
      {
        state.debug = 1;
        printf("debug mode activated\n");
      }

      if (state.debug) 
      {
        if (state.key == ONE) 
        {
          printf("enter map index: ");
          scanf("%u", &map_index);
          state_load(map_src[map_index]);
          player_load();
          corrupt_load(&corruption);
          camera = veci2_create(0, 0);
        }
      }

      /* player movement */
      player.dir.x = 0;
      player.dir.y = 0;

      if (state.key != NONE) 
      {
        switch (state.key) 
        {
          case LEFT: player.dir.x = -1; break;
          case RIGHT: player.dir.x = 1; break;
          case UP: player.dir.y = -1; break;
          case DOWN: player.dir.y = 1; break;
          default: player.dir.x = 0; player.dir.y = 0;
        }

        LEFT_SHIFT(player.prev_pos, player.pos, PLAYER_PREVIOUS);
      }

      player.pos.x += round(player.dir.x * PLAYER_SPEED);
      player.pos.y += round(player.dir.y * PLAYER_SPEED);

      /* player collision */
      if (player_touch('R') || player_touch('w') ||
          player_touch('G') || player.pos.x < 0 ||
          player.pos.x > (state.columns - 1) * state.tile_size ||
          player.pos.y < 1 ||
          player.pos.y > (state.rows - 1) * state.tile_size) 
      {
        player.pos.x -= round(player.dir.x * PLAYER_SPEED);
        player.pos.y -= round(player.dir.y * PLAYER_SPEED);
      }

      /* player animation */
      if (state.key != NONE) 
      {
        switch (state.key) 
        {
        case LEFT: animator_set_index(2, PLAYER_ANIM); break;
        case RIGHT: animator_set_index(1, PLAYER_ANIM); break;
        case UP: animator_set_index(3, PLAYER_ANIM); break;
        case DOWN: animator_set_index(4, PLAYER_ANIM); break;
        default: animator_set_index(0, PLAYER_ANIM);
        }
      } 
      else 
      {
        animator_set_index(0, PLAYER_ANIM);
      }

      /* text */
      if (player_touch('T')) 
      {
        text_show = 1;
        if (state.key == X && text_index < state.text_size - 1) 
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
      if (player_touch('>')) 
      {
        map_index++;
        state_load(map_src[map_index]);
        player_load();
        corrupt_load(&corruption);
        camera = veci2_create(0, 0);
      }

      /* restart */
      if (player_touch('C')) 
      {
        map_index = 1;
        state_load(map_src[MAP_COUNT - 1]);
        player_load();
        corrupt_load(&corruption);
        camera = veci2_create(0, 0); 
      }

      /* animations */
      animator_update(0.5f, GRASS_ANIM);
      animator_update(2.0f, FLOWER_ANIM);
      animator_update(0.25f, WATER_ANIM);

      /* particle */
      particle_float(&player_float_sim, player.pos.x, player.pos.y, 0.1f);
      particle_rain(&rain_sim, camera.x, camera.y, 0.1f);

      /* player camera */
      if (player.pos.x > camera.x + 12 * state.tile_size &&
          camera.x < (state.columns - 16) * state.tile_size) 
      {
        camera.x += round(CAMERA_SPEED);
      } 
      else if (player.pos.x < camera.x + 4 * state.tile_size &&
                 camera.x > 0) 
      {
        camera.x -= round(CAMERA_SPEED);
      } 
      else if (player.pos.y > camera.y + 12 * state.tile_size &&
                 camera.y < (state.columns - 16) * state.tile_size) 
      {
        camera.y += round(CAMERA_SPEED);
      } 
      else if (player.pos.y < camera.y + 4 * state.tile_size &&
                 camera.y > 0) 
      {
        camera.y -= round(CAMERA_SPEED);
      }

      /* corruption */
      veci2 target = veci2_create(player.pos.x, player.pos.y);
      corrupt_update(&corruption, target);
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
    for (u32 i = 0; i < SCREEN_MAX; i++) 
    {
      state.pixels[i] = state.background_color;
    }

    const u32 camera_column = (f32)camera.y / state.tile_size;
    const u32 camera_row = (f32)camera.x / state.tile_size;

    for (u32 i = camera_column; i < camera_column + SCREEN_TILES + 1; i++) 
    {
      for (u32 j = camera_row; j < camera_row + SCREEN_TILES + 1; j++) 
      {
        const i32 x = j * state.tile_size - camera.x;
        const i32 y = i * state.tile_size - camera.y;
        switch (get_type(j, i)) 
        {
        case 't': texture_render(x, y, TILE_TXT); break;
        case 'g': animator_render(x, y, GRASS_ANIM); break;
        case 'f': animator_render(x, y, FLOWER_ANIM); break;
        case 'W': texture_render(x, y, WOOD_TXT); break;
        case 'T': texture_render(x, y, T_TXT); break;
        case '>': texture_render(x, y, NEXT_TXT); break;
        case 'p': texture_render(x, y, P_TXT); break;
        case 'R': texture_render(x, y, ROCK_TXT); break;
        case 'w': animator_render(x, y, WATER_ANIM); break;
        case 'G': texture_render(x, y, GIRL_TXT); break;
        case 'C': texture_render(x, y, CORRUPTION_TXT); break;
        default: texture_render(x, y, BLANK_TXT);
        }
      }
    } 

    /* particles */
    particle_render(&player_float_sim, 0x00FF);

    switch (state.weather)
    {
    case RAIN: particle_render(&rain_sim, 0x008F); break;
    default: break;
    }

 /* girl */
    if (state.girl_show) 
      texture_render(player.prev_pos[0].x - camera.x, 
                     player.prev_pos[0].y - camera.y,
                     GIRL_TXT);

    /* lighting */
    switch (state.light)
    {
    case DARK: flash_light(player.pos.x + (f32)PLAYER_WIDTH_2,
                           player.pos.y + (f32)PLAYER_HEIGHT_2, 64); break;
    default: break;
    }

    /* corruption */
    for (u32 i = 0; i < corruption.count; i++) 
    {
      /* makes is look spooky */
      for (u32 j = 0; j < 64; j++)
      {
        textures[CORRUPTION_TXT].pixels[j] = rand() % 65636;
      }
    }

    /* glowy tiles */
    for (u32 i = camera_column; i < camera_column + SCREEN_TILES; i++) 
    {
      for (u32 j = camera_row; j < camera_row + SCREEN_TILES; j++) 
      {
        const i32 x = j * state.tile_size - camera.x;
        const i32 y = i * state.tile_size - camera.y;
        switch (get_type(j, i)) 
        {
        case 'T': glow(x, y, T_TXT); break;
        case '>': glow(x, y, NEXT_TXT); break;
        case 'p': glow(x, y, P_TXT); break;
        case 'C': glow(x, y, CORRUPTION_TXT); break;
        default: break;
        }
      }
    }
 
    /* player */
    animator_render(player.pos.x - camera.x, player.pos.y - camera.y, 
                    PLAYER_ANIM);
 
    /* text */
    if (text_show) text_render(state.texts[text_index], font, 32, 98);

    const u32 pitch = 2;
    SDL_UpdateTexture(state.texture, NULL, state.pixels, SCREEN_WIDTH * pitch);
    SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(state.renderer, state.texture, &rect, NULL);

    /* pixel grid lines */
    SDL_SetRenderDrawColor(state.renderer, 0.0f, 0.0f, 0.0f, 0.0f);

    for (u32 i = 0; i < SCREEN_HEIGHT; i++) 
    {
      for (u32 j = 0; j < SCALE - 1; j++) 
      {
        SDL_RenderDrawLine(state.renderer, 0, i * SCALE + j,
                           SCREEN_WIDTH * SCALE, i * SCALE + j);
      }
    }

    /* draw to screen */
    SDL_RenderPresent(state.renderer);
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
  texture_destroy(PLAYER_TXT);
  texture_destroy(TILE_TXT);
  texture_destroy(BLANK_TXT);
  texture_destroy(GRASS_TXT);
  texture_destroy(FLOWER_TXT);
  texture_destroy(NEXT_TXT);
  texture_destroy(P_TXT);
  texture_destroy(ROCK_TXT);
  texture_destroy(WATER_TXT);
  texture_destroy(GIRL_TXT);
  texture_destroy(CORRUPTION_TXT);

  font_destroy(&font);
  state_destroy();
  corrupt_destroy(&corruption);
  particle_sim_destroy(&player_float_sim);
  particle_sim_destroy(&rain_sim);
  SDL_Quit();

  return 0;
}
