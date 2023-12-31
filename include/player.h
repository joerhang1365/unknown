#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "state.h"
#include "animator.h"

#define PLAYER_WIDTH 8
#define PLAYER_HEIGHT 8
#define PLAYER_WIDTH_2 PLAYER_WIDTH / 2
#define PLAYER_HEIGHT_2 PLAYER_HEIGHT / 2
#define PLAYER_SPEED 2
#define PLAYER_PREVIOUS 8

typedef struct
{
  u32 width;
  u32 height;
  veci2 pos;
  veci2 dir;
  veci2 prev_pos[PLAYER_PREVIOUS];
} player_t;

/* global */
extern player_t player;

void player_movement();
void player_animation();
byte player_touch(char c);
void player_collision();
void player_load();

#endif
