#include "animator.h"

void animator_create(animator *animator, SDL_Texture *texture_map, unsigned int width, unsigned int height, unsigned int number_of_frames) 
{
  for (int i = 0; i < ANIMATOR_MAX_CLIPS; i++) 
  {
    animator->clips[i].x = i * width;
    animator->clips[i].y = 0;
    animator->clips[i].w = width;
    animator->clips[i].h = height;
  }
  animator->texture_map = texture_map;
  animator->current_clip = animator->clips[0];
  animator->number_of_frames = number_of_frames;
  animator->frame = 0;
}

void animator_set_frame(animator *animator, unsigned int frame) 
{
  animator->current_clip = animator->clips[frame];
}

void animator_update_clip(animator *animator, unsigned int framerate) 
{
  animator->current_clip = animator->clips[animator->frame / framerate];
  ++animator->frame;
  if (animator->frame / framerate > animator->number_of_frames - 1) 
  {
    animator->frame = 0;
  }
}

void animator_render(animator animator, SDL_Renderer *renderer, int x, int y, unsigned int width, unsigned int height) 
{
  SDL_Rect quad = {x, y, width, height};
  SDL_RenderCopy(renderer, animator.texture_map, &animator.current_clip, &quad);
}

void animator_destroy(animator *animator) 
{
  texture_destroy(animator->texture_map);
}
