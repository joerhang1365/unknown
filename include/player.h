#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "state.h"
#include "animator.h"

#define PLAYER_WIDTH 8
#define PLAYER_HEIGHT 8
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

// global player
extern player_t player;

void player_load();

byte player_touch(char c);
void player_movement();
void player_animation(animator_t *animator);
void player_collision();

#endif
