#include "animator.h"
#include "corrupt.h"
#include "globals.h"
#include "lighting.h"
#include "particle.h"
#include "player.h"
#include "state.h"
#include "text.h"
#include "texture.h"
#include "camera.h"
#include "audio.h"

#define SCREEN_TILES SCREEN_WIDTH / state.tile_size

particle_sim_t player_float_sim;
particle_sim_t rain_sim;
particle_sim_t wind_sim;

corruption_t corruption;

static void initialize()
{
  ASSERT(SDL_Init(SDL_INIT_VIDEO) > 0, "failed to initialize video\n");
  ASSERT(SDL_Init(SDL_INIT_AUDIO) > 0, "failed to initialize audio\n");

  /* window */
  state.window = SDL_CreateWindow("unknown", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * SCALE,
                                  SCREEN_HEIGHT * SCALE, SDL_WINDOW_SHOWN);
  ASSERT(state.window == NULL, "failed to create window\n");

  /* renderer */
  state.renderer = SDL_CreateRenderer(state.window, -1, 
                                      SDL_RENDERER_ACCELERATED | 
                                      SDL_RENDERER_PRESENTVSYNC);
  ASSERT(state.renderer == NULL, "failed to create renderer\n");

  /* texture */
  state.texture = SDL_CreateTexture(state.renderer, SDL_PIXELFORMAT_RGBA4444,
                                    SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
                                    SCREEN_HEIGHT);
  ASSERT(state.texture == NULL, "failed to create texture\n");

  initAudio();
  /* map */
  maps_create();
  state_load();
  player_load();

  /* particles */
  particle_sim_create(&player_float_sim, 16);
  particle_sim_create(&rain_sim, 128);
  particle_sim_create(&wind_sim, 128);

  /* corruption */
  //corruption.corrupts = NULL;
  corrupt_load(&corruption);

  /* font */
  //state.font.data = NULL; // need this for sum reason
  font_create(&state.font, 0xFFFF, "font_data");

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
  texture_create("textures/button.txt", BUTTON_TXT);

  /* animations */
  animator_create(8, 8, 5, PLAYER_ANIM, PLAYER_TXT);
  animator_create(8, 8, 4, GRASS_ANIM, GRASS_TXT);
  animator_create(8, 8, 2, FLOWER_ANIM, FLOWER_TXT);
  animator_create(8, 8, 2, WATER_ANIM, WATER_TXT);
  animator_create(8, 8, 2, BUTTON_ANIM, BUTTON_TXT);

  playMusic("audio/drowning.wav", 10);
}

static void key_input(SDL_Event event)
{
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

static void next(const u32 index)
{
  state.map_index = index;
  state_load();
  player_load();
  corrupt_load(&corruption);
}

static void debug(const u32 key)
{
  if (key == F1 && state.debug == 0)
  {
    state.debug = 1;
    printf("debuging mode activated\n");
  }

  if (state.debug != 1) return;

  switch (key)
  {
    case ONE: printf("enter map index: ");
              scanf("%u", &state.map_index);
              next(state.map_index);
  }
}

static void update()
{
  debug(state.key);

  player_movement(state.key);
  player_collision(state.columns, state.rows, state.tile_size);
  player_animation(state.key);

  /* text box */
  state.text_show = 0;
  if (is_tile(player.pos.x, player.pos.y, player.width, player.height, 'T'))
    state.text_show = 1;
  /* button */
  else if (is_tile(player.pos.x, player.pos.y, player.width, player.height, 'B')) 
    state.button = 1;
  /* next map */
  else if (is_tile(player.pos.x, player.pos.y, player.width, player.height, '>')) 
    next(++state.map_index);
  /* corruption */
  else if (is_tile(player.pos.x, player.pos.y, player.width, player.height, 'C')) 
    next(2);
      
  /* animations */
  animator_update(24, GRASS_ANIM);
  animator_update(48, FLOWER_ANIM);
  animator_update(24, WATER_ANIM);

  if (state.text_show)
  {
    if (state.key == X && state.text_index < state.text_size - 1) 
    {
      state.text_index++;
      state.key = NONE;
    }
  }
  else state.text_index = 0;

  if (state.button) animator_set_index(1, BUTTON_ANIM);
  else animator_set_index(0, BUTTON_ANIM);

  /* particle */
  particle_float(&player_float_sim, player.pos.x, player.pos.y, 0.1f);
  particle_rain(&rain_sim, state.camera.x, state.camera.y, veci2_create(0, 1), 0.01f);
  particle_wind(&wind_sim, veci2_create(1, 0), 0.01f);

  //camera_update();
  camera_follow(player.pos, 8, 4, &state.camera);
 
  /* corruption */
  corrupt_update(&corruption, veci2_create(player.pos.x, player.pos.y));

  for (u32 i = 0; i < corruption.count; i++) 
  {
    /* makes is look spooky */
    for (u32 j = 0; j < 64; j++)
    {
      textures[CORRUPTION_TXT].pixels[j] = rand() % 65636;
    }
  }
}

static void render()
{
  /* set all pixels to 0x0000 */
  SCREEN_CLEAR(state.pixels, SCREEN_MAX);

  /* background color */
  for (u32 i = 0; i < SCREEN_MAX; i++) 
  {
    state.pixels[i] = state.background_color;
  }

  const u32 camera_column = (f32)state.camera.y / state.tile_size;
  const u32 camera_row = (f32)state.camera.x / state.tile_size;

  /* map tiles */
  for (u32 i = camera_column; i < camera_column + SCREEN_TILES + 1; i++) 
  {
    for (u32 j = camera_row; j < camera_row + SCREEN_TILES + 1; j++) 
    {
      const i32 x = j * state.tile_size - state.camera.x;
      const i32 y = i * state.tile_size - state.camera.y;
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
        case 'B': animator_render(x, y, BUTTON_ANIM); break;
        default:  texture_render(x, y, BLANK_TXT);
      }
    }
  } 

  /* weather */
  switch (state.weather)
  {
    case CLEAR: break;
    case RAIN: particle_render(&rain_sim, 0x008F); break;
    case WIND: particle_render(&wind_sim, 0xFFFF); break;
    default: break;
  } 

  /* lighting */
  switch (state.light)
  {
    case LIGHT: 
      sun_shadows(veci2_create(0, 0)); 
      break;
    case DARK:
      ALPHA_SET(state.pixels, SCREEN_MAX, 0);
      light_source(player.pos.x + PLAYER_WIDTH_2,
            player.pos.y + PLAYER_HEIGHT_2,
            32, 2, 360);

      /* glowy tiles */
      for (u32 i = camera_column; i < camera_column + SCREEN_TILES; i++) 
      {
        for (u32 j = camera_row; j < camera_row + SCREEN_TILES; j++) 
        {
          const i32 x = j * state.tile_size - state.camera.x;
          const i32 y = i * state.tile_size - state.camera.y;
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
      break;
    default: break;
  }

  /* girl */
  if (state.girl_show) texture_render(player.prev_pos[0].x - state.camera.x, 
                                      player.prev_pos[0].y - state.camera.y,
                                      GIRL_TXT);

  /* player */
  particle_render(&player_float_sim, 0x00FF);
  animator_render(player.pos.x - state.camera.x, player.pos.y - state.camera.y, 
                  PLAYER_ANIM);
 
  /* text */
  if (state.text_show) text_render(state.texts[state.text_index], state.font, 32, 98);

  /* change color value based on alpha value */
  for(u32 i = 0; i < SCREEN_MAX; i++)
  {
    ALPHA_BLEND(state.pixels[i]);
  }

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

static void destroy()
{
  for (u32 i = 0; i < TEXTURE_MAX; i++)
  {
    texture_destroy(i);
  }

  for (u32 i = 0; i < ANIMATION_MAX; i++)
  {
    animator_destroy(i);
  }

  maps_destroy();
  state_destroy();
  corrupt_destroy(&corruption);
  particle_sim_destroy(&player_float_sim);
  particle_sim_destroy(&rain_sim);
  particle_sim_destroy(&wind_sim);
  font_destroy(&state.font);
  endAudio();
  SDL_Quit();
}

i32 main(i32 argc, char *argv[]) 
{
  initialize();

  while (!state.quit) 
  {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
      key_input(event);
    }

    PREVIOUS_TIME = CURRENT_TIME;
    CURRENT_TIME = SDL_GetTicks();
    DELTA_TIME = (CURRENT_TIME - PREVIOUS_TIME) / 1000.0f;
    TIME += DELTA_TIME;
    FRAME_TIME += DELTA_TIME;

    if (FRAME_TIME >= (f32)1 / FRAMERATE) 
    {
      FRAME_TIME = 0;
      update();
    } 

    render();
  }

  destroy();

  return 0;
}
