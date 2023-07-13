#include "animator.h"
#include "constants.h"
#include "tilemap.h"
#include "player.h"
#include "text.h"
#include "texture.h"
#include "vector.h"
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
  enum ARROW_KEYS key;
  bool x_key = false;

  // levels
  int level_index = 0;
  char *level_sources[] = 
  {
    "levels/0.level", 
    "levels/1.level", 
    "levels/2.level",
    "levels/3.level", 
    "levels/4.level"
  };

  tilemap levels[TILEMAP_MAX];
  tilemap_load(&levels[level_index], level_sources[level_index], renderer);

  player the_player;
  player_load(&the_player, renderer, "images/player.png", 0, 0, 16, 16, PLAYER_FRAMES);
  the_player.position = tilemap_get_position(levels[level_index], 'p');

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
        switch (event.key.keysym.sym) 
        {
          case SDLK_UP:
            key = UP;
            break;
          case SDLK_DOWN:
            key = DOWN;
            break;
          case SDLK_LEFT:
            key = LEFT;
            break;
          case SDLK_RIGHT:
            key = RIGHT;
            break;
          case SDLK_x:
            x_key = true;
            break;
        }
      } 
      else if (event.type == SDL_KEYUP) 
      {
        key = NONE;
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

      tilemap_update_animation(&levels[level_index]);
      player_update(&the_player, delta_time, key);

      // collision
      bool is_collision = tilemap_is_collision(
          levels[level_index], the_player.position.i,
          the_player.position.i + PLAYER_WIDTH * SCREEN_SCALE,
          the_player.position.j,
          the_player.position.j + PLAYER_HEIGHT * SCREEN_SCALE);
      if (is_collision) 
      {
        player_collision(&the_player);
      }

      // sends player to next level
      if (tilemap_next(levels[level_index],
                     the_player.position.i + (float)PLAYER_WIDTH / 2,
                     the_player.position.j + (float)PLAYER_HEIGHT / 2)) 
      {
        tilemap_unload(&levels[level_index]);
        level_index++;
        tilemap_load(&levels[level_index], level_sources[level_index], renderer);
        levels[level_index].triggers[TEXT_TRIGGER] = false;
        player_load(&the_player, renderer, "images/player.png", 0, 0, 16, 16, PLAYER_FRAMES);
        the_player.position = tilemap_get_position(levels[level_index], 'p');
      }

      // trips trigger if tile is t
      if (tilemap_is_tile(
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
      if (tilemap_is_tile(
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
        tilemap_change_map(&levels[3], map);
      }
    }

    // change text
    if (x_key) 
    {
      tilemap_next_text(&levels[level_index]);
      x_key = false;
    }

    // render
    // clear screen
    SDL_SetRenderDrawColor(renderer, 1.0f, 1.0f, 1.0f, 1.0f);
    SDL_RenderClear(renderer);

    tilemap_render_map(levels[level_index], renderer);
    tilemap_render_text(levels[level_index], renderer, font, color);
    player_render(the_player, renderer);

    // pixel grid lines
    SDL_SetRenderDrawColor(renderer, 1.0f, 1.0f, 1.0f, 1.0f);
    for (int i = 0; i < SCREEN_HEIGHT * SCREEN_SCALE; i += SCREEN_SCALE) 
    {
      SDL_RenderDrawLine(renderer, 0, i, SCREEN_WIDTH * SCREEN_SCALE, i);
      SDL_RenderDrawLine(renderer, i, 0, i, SCREEN_HEIGHT * SCREEN_SCALE);
    }

    // draw to screen
    SDL_RenderPresent(renderer);
  }

  // destroy components
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  tilemap_unload(&levels[level_index]);
  player_unload(&the_player);
  window = NULL;
  renderer = NULL;
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}
