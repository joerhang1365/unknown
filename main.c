#include "animator.h"
#include "constants.h"
#include "raycast.h"
#include "tilemap.h"
#include "player.h"
#include "text.h"
#include "texture.h"
#include "vector.h"
#include "level.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define FRAMERATE 60

int main() 
{
  // window initialization
  SDL_Window *window = NULL;
  if (SDL_Init(SDL_INIT_VIDEO) > 0) 
  {
    printf("video initialization error\n");
    return 1;
  } 
  else 
  {
    window = SDL_CreateWindow("unknown", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE, SDL_WINDOW_SHOWN);
    if (window == NULL) 
    {
      printf("window creation error\n");
      return 1;
    }
  }

  // renderer initialization
  SDL_Renderer *renderer = NULL;
  renderer = SDL_CreateRenderer(window, -1, 
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) 
  {
    printf("renderer creation error\n");
    return 1;
  }

  image_initialize();
  TTF_Font *font = NULL;
  SDL_Color color = {0, 255, 0};
  font_initialize(&font, "fonts/monopixel.ttf", 12);

  // key event initialization
  SDL_Event event;
  enum KEYS arrow_key;
  enum KEYS event_key;

  // levels
  int level_index = 3;
  char *level_sources[] = 
  {
    "levels/0.level", 
    "levels/1.level", 
    "levels/2.level",
    "levels/3.level", 
    "levels/4.level"
  };

  level levels[LEVEL_MAX];
  level_load(&levels[level_index], level_sources[level_index], renderer);

  player the_player;
  player_load(&the_player, renderer, "images/player.png", 0, 0, 16, 16, PLAYER_FRAMES);
  the_player.position = level_get_position(levels[level_index], 'p');

  int wall_heights[SCREEN_WIDTH];

  // loop
  bool run = true;
  float frame_time = 0;
  float delta_time = 0;
  int previous_time = 0;
  int current_time = 0;
  while (run) 
  {
    // key events
    while (SDL_PollEvent(&event) != 0) 
    {
      if (event.type == SDL_QUIT) 
      {
        run = false;
      } 
      else if (event.type == SDL_KEYDOWN) 
      {
        if(event.key.keysym.sym == SDLK_UP)
        {
          arrow_key = UP;
        }
        if(event.key.keysym.sym == SDLK_RIGHT)
        {
          arrow_key = RIGHT;
        }
        if(event.key.keysym.sym == SDLK_DOWN)
        {
          arrow_key = DOWN;
        }
        if(event.key.keysym.sym == SDLK_LEFT)
        {
          arrow_key = LEFT;
        }
        if(event.key.keysym.sym == SDLK_x)
        {
          event_key = X;
        } 
      } 
      else if (event.type == SDL_KEYUP) 
      {
        arrow_key = NONE;
      }
    }

    // update
    previous_time = current_time;
    current_time = SDL_GetTicks();
    delta_time = (current_time - previous_time) / 1000.0f;
    frame_time += delta_time;
    if (frame_time >= (float)1 / FRAMERATE) 
    {
      frame_time = 0;

      level_update_animation(&levels[level_index]);
      player_update(&the_player, delta_time, arrow_key);

      // collision
      bool is_collision = level_is_collision(
          levels[level_index], the_player.position.i,
          the_player.position.i + PLAYER_WIDTH * SCREEN_SCALE,
          the_player.position.j,
          the_player.position.j + PLAYER_HEIGHT * SCREEN_SCALE);
      if (is_collision) 
      {
        player_collision(&the_player);
      }

      // sends player to next level
      if (level_is_next(levels[level_index],
                     the_player.position.i + (float)PLAYER_WIDTH / 2,
                     the_player.position.j + (float)PLAYER_HEIGHT / 2)) 
      {
        level_unload(&levels[level_index]);
        level_index++;
        level_load(&levels[level_index], level_sources[level_index], renderer);
        levels[level_index].triggers[TEXT_TRIGGER] = false;
        player_load(&the_player, renderer, "images/player.png", 0, 0, 16, 16, PLAYER_FRAMES);
        the_player.position = level_get_position(levels[level_index], 'p');
      }

      // trips trigger if tile is t
      if (level_is_type(
              levels[level_index], the_player.position.i + (float)PLAYER_WIDTH / 2,
              the_player.position.j + (float)PLAYER_HEIGHT / 2, 't')) 
      {
        levels[level_index].triggers[TEXT_TRIGGER] = true;
      } 
      else 
      {
        levels[level_index].triggers[TEXT_TRIGGER] = false;
        levels[level_index].text_index = 0;
      }

      // trips trigger if tile is b
      if (level_is_type(
              levels[level_index], the_player.position.i + (float)PLAYER_WIDTH / 2,
              (float)the_player.position.j + (float)PLAYER_HEIGHT / 2, 'b')) 
      {
        levels[level_index].triggers[BUTTON_TRIGGER] = true;
      }

      // level 4 event when button is pressed
      if (levels[3].triggers[BUTTON_TRIGGER]) 
      {
        char map[] = "        ^       "
                     "             r  "
                     "            r   "
                     "                "
                     "                "
                     "wwwwwww  wwwwwww"
                     "wwwwwww  wwwwwww"
                     "                "
                     "                "
                     "  b             "
                     "        t       "
                     "                "
                     "                "
                     "                "
                     "                "
                     "        p       ";
        level_change_map(&levels[3], map);
      }

      // change text
      if (event_key == X) 
      {
        level_next_text(&levels[level_index]);
        event_key = NONE;
      }

      if(levels[level_index].mode == RAYCAST)
      {
        raycast_get_wall_heights(wall_heights, levels[level_index], the_player);
      }
    }

    // render
    // clear screen
    SDL_SetRenderDrawColor(renderer, 1.0f, 1.0f, 1.0f, 1.0f);
    SDL_RenderClear(renderer);

    if(levels[level_index].mode == TILEMAP)
    {
      tilemap_render_map(levels[level_index], renderer);
      player_render(the_player, renderer);
      tilemap_render_text(levels[level_index], renderer, font, color);
    }

    if(levels[level_index].mode == RAYCAST)
    {
      raycast_render_map(wall_heights, renderer);
    }

    // pixel grid lines
    if(SCREEN_SCALE > 1)
    {
      SDL_SetRenderDrawColor(renderer, 1.0f, 1.0f, 1.0f, 1.0f);
      for (int i = 0; i < SCREEN_HEIGHT * SCREEN_SCALE; i += SCREEN_SCALE) 
      {
        SDL_RenderDrawLine(renderer, 0, i, SCREEN_WIDTH * SCREEN_SCALE, i);
        SDL_RenderDrawLine(renderer, i, 0, i, SCREEN_HEIGHT * SCREEN_SCALE);
      }
    }

    // draw to screen
    SDL_RenderPresent(renderer);
  }

  // destroy components
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  level_unload(&levels[level_index]);
  player_unload(&the_player);
  window = NULL;
  renderer = NULL;
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}
