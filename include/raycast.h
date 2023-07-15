#ifndef RAYCAST_H
#define RAYCAST_H

#include <SDL2/SDL.h>
#include <math.h>
#include "constants.h"
#include "vector.h"
#include "player.h"
#include "level.h"

void raycast_get_wall_heights(int wall_heights[SCREEN_WIDTH], level level, player player);
void raycast_render_map(int wall_heights[SCREEN_WIDTH], SDL_Renderer *renderer);

#endif
