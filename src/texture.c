#include "texture.h"

int image_initialize() 
{
  int result = 0;
  int imageFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imageFlags) & imageFlags)) 
  {
    printf("error unknown image type\n");
    result = -1;
  }
  return result;
}

void texture_create(SDL_Texture **texture, char *path, SDL_Renderer *renderer) 
{
  SDL_Surface *surface = IMG_Load(path);
  if (surface == NULL) 
  {
    printf("error creating texture surface\n");
  } 
  else 
  {
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) 
    {
      printf("error creating texture\n");
    }
  }
  SDL_FreeSurface(surface);
}

void texture_destroy(SDL_Texture *texture) 
{
  SDL_DestroyTexture(texture);
  texture = NULL;
}
