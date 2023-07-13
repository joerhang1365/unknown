#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int image_initialize();
void texture_create(SDL_Texture **texture, char *path, SDL_Renderer *renderer);
void texture_destroy(SDL_Texture *texture);

#endif
