#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "state.h"
#include "animator.h"

#define PLAYER_WIDTH 8
#define PLAYER_HEIGHT 8
#define PLAYER_WIDTH_2 PLAYER_WIDTH / 2
#define PLAYER_HEIGHT_2 PLAYER_HEIGHT / 2
#define PLAYER_SPEED 64 * DELTA_TIME
#define PLAYER_PREVIOUS 16

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

byte player_touch(char c);
void player_load();

#endif
