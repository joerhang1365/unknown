#ifndef RAYCAST_H
#define RAYCAST_H

#include <SDL2/SDL.h>
#include <math.h>
#include "vector.h"
#include "player.h"

void raycast_get_wall_heights(int *wall_heights, int screen_width, int screen_height, char *map, player player);
void raycast_render_map(int *wall_heights, SDL_Renderer *renderer);

#endif
