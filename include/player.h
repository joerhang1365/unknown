#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "constants.h"
#include "vector.h"
#include "texture.h"
#include "animator.h"

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 16
#define PLAYER_FRAMES 4
#define FLOAT_AMPLITUDE 0.5

typedef struct
{
  vector2 position;
  vector2 velocity;
  float field_of_view;
  float angle;
  float y_offset;
  float time;
  bool bob;
  SDL_Texture *texture;
  animator animation;
} player;

void player_load(player *player, SDL_Renderer *renderer, char *source, int x, int y, unsigned int width, unsigned int height, unsigned int number_of_frames);
void player_collision(player *player);
void player_tilemap_update(player *player, float delta_time, int arrow_key);
void player_raycast_update(player *player, int arrow_key);
void player_render(player player, SDL_Renderer *renderer);
void player_unload(player *player);

#endif
