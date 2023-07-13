#ifndef TILEMAP_H
#define TILEMAP_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "vector.h"
#include "constants.h"
#include "texture.h"
#include "animator.h"
#include "text.h"

#define TILEMAP_MAX 6
#define TILEMAP_TEXTS 4
#define TILEMAP_TEXT_WIDTH 190 * SCREEN_SCALE
#define TILEMAP_TEXT_HEIGHT 16 * SCREEN_SCALE
#define TILEMAP_COLUMN 32
#define TILEMAP_MESSAGE 120
#define TILEMAP_TRIGGERS 2
#define TILEMAP_ANIMATIONS 4
#define TILEMAP_TEXTURES 14
#define TILEMAP_MAP 256

typedef struct
{
  int rows;
  int columns;
  int tile_size;
  int text_index;
  int text_size;
  char message[TILEMAP_TEXTS * TILEMAP_MESSAGE];
  char map[TILEMAP_MAP];
  bool triggers[TILEMAP_TRIGGERS];
  text text[TILEMAP_TEXTS];
  SDL_Texture *textures[TILEMAP_TEXTURES];
  animator animations[TILEMAP_ANIMATIONS];
} tilemap;

/*
 * f = flower
 * l = grass
 * w = water
 * b = button
 * r = rock
 * g = gravel
 * W = wall
 * F = floor
 * ^ = up arrow
 * > = right arrow
 * p = spawn
 * t = text
 * L = larry
 * # = invisible wall
 */

enum TYPES { FLOWER, GRASS, WATER, BUTTON, ROCK, GRAVEL, WALL, FLOOR, UP_ARROW, RIGHT_ARROW, SPAWN, TEXT, TEXT_BOX, LARRY };
enum TRIGGERS { BUTTON_TRIGGER, TEXT_TRIGGER };
enum KEYS { X };

char tilemap_get_tile(tilemap tilemap, int column, int row);
bool tilemap_is_tile(tilemap tilemap, float x, float y, char c);
bool tilemap_is_collision(tilemap tilemap, float left, float right, float top, float bottom);
bool tilemap_next(tilemap tilemap, float x, float y);
vector2 tilemap_get_position(tilemap tilemap, char c);
void tilemap_change_map(tilemap *tilemap, char *map);
void tilemap_next_text(tilemap *tilemap);
void tilemap_load(tilemap *tilemap, char *source, SDL_Renderer *renderer);
void tilemap_update_animation(tilemap *tilemap);
void tilemap_render_map(tilemap tilemap, SDL_Renderer *renderer);
void tilemap_render_text(tilemap tilemap, SDL_Renderer *renderer, TTF_Font *font, SDL_Color color);
void tilemap_unload(tilemap *tilemap);

#endif
