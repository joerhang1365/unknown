#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <SDL2/SDL.h>
#include "texture.h"

#define ANIMATOR_MAX_CLIPS 16

typedef struct
{
  SDL_Texture *texture_map;
  SDL_Rect clips[ANIMATOR_MAX_CLIPS];
  SDL_Rect current_clip;
  unsigned int number_of_frames;
  unsigned int frame;
} animator;

void animator_create(animator *animator, SDL_Texture *texture_map, unsigned int width, unsigned int height, unsigned int number_of_frames);
void animator_set_frame(animator *animator, unsigned int frame);
void animator_update_clip(animator *animator, unsigned int framerate);
void animator_render(animator animator, SDL_Renderer *renderer, int x, int y, unsigned int width, unsigned int height);
void animator_destroy(animator *animator);

#endif
