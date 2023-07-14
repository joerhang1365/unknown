#ifndef LEVEL_H
#define LEVEL_H

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

#define LEVEL_MAX 6
#define LEVEL_TEXTS 4
#define LEVEL_TEXT_WIDTH 190 * SCREEN_SCALE
#define LEVEL_TEXT_HEIGHT 16 * SCREEN_SCALE
#define LEVEL_COLUMN 32
#define LEVEL_MESSAGE 120
#define LEVEL_TRIGGERS 2
#define LEVEL_ANIMATIONS 4
#define LEVEL_TEXTURES 14
#define LEVEL_MAP 256

typedef struct
{
  int rows;
  int columns;
  int tile_size;
  int text_index;
  int text_size;
  char message[LEVEL_TEXTS * LEVEL_MESSAGE];
  char map[LEVEL_MAP];
  bool triggers[LEVEL_TRIGGERS];
  text text[LEVEL_TEXTS];
  SDL_Texture *textures[LEVEL_TEXTURES];
  animator animations[LEVEL_ANIMATIONS];
} level;

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

char level_get_type(level level, int column, int row);
bool level_is_type(level level, float x, float y, char c);
bool level_is_collision(level level, float left, float right, float top, float bottom);
bool level_is_next(level level, float x, float y);
vector2 level_get_position(level level, char c);
void level_change_map(level *level, char *map);
void level_next_text(level *level);
void level_load(level *level, char *source, SDL_Renderer *renderer);
void level_update_animation(level *level);
void level_unload(level *level);

#endif
