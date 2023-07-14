#ifndef TILEMAP_H
#define TILEMAP_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include "constants.h"
#include "texture.h"
#include "animator.h"
#include "text.h"
#include "level.h"

void tilemap_render_map(level level, SDL_Renderer *renderer);
void tilemap_render_text(level level, SDL_Renderer *renderer, TTF_Font *font, SDL_Color color);

#endif
