#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "state.h"
#include "animator.h"

#define PLAYER_WIDTH 8
#define PLAYER_HEIGHT 8
#define PLAYER_SPEED 2
#define PLAYER_PREVIOUS 8

static struct
{
  u32 width;
  u32 height;
  veci2 pos;
  veci2 dir;

  veci2 prev_pos[PLAYER_PREVIOUS];
} player;

static inline void player_load()
{
  player.width = PLAYER_WIDTH;
  player.height = PLAYER_HEIGHT;
  player.pos = find_position('p', 0);
  VECi2_MULTI(player.pos, state.tile_size);
  VECi2(player.dir, 0, 0);
  
  /* set all prev_pos to current pos */
  for(u32 i = 0; i < PLAYER_PREVIOUS; i++)
  {
    player.prev_pos[i] = player.pos;
  }
}

static inline byte player_touch(char c)
{
  return 
    is_type(player.pos.x, player.pos.y, c) ||
    is_type(player.pos.x + player.width - 1, player.pos.y, c) ||
    is_type(player.pos.x, player.pos.y + player.height - 1, c) ||
    is_type(player.pos.x + player.width - 1, player.pos.y + player.height - 1, c);
}

static inline void player_movement()
{
  player.dir.x = 0;
  player.dir.y = 0;
  
  if(state.key != NONE)
  {
    switch(state.key)
    {
      case LEFT: player.dir.x = -1; break;
      case RIGHT: player.dir.x = 1; break;
      case UP: player.dir.y = -1; break;
      default: player.dir.y = 1;
    }
    if(state.girl_show == 1)
    {     
      LEFT_SHIFT(player.prev_pos, player.pos, PLAYER_PREVIOUS);
    }
  }

  player.pos.x += player.dir.x * PLAYER_SPEED;
  player.pos.y += player.dir.y * PLAYER_SPEED;
}

static inline void player_animation(animator_t *animator)
{
  if(state.key != NONE)
  {
    switch(state.key)
    {
      case LEFT: animator_set_index(animator, 2); break;
      case RIGHT: animator_set_index(animator, 1); break;
      case UP: animator_set_index(animator, 3); break;
      default: animator_set_index(animator, 4);
    }
  }
  else
  {
    animator_set_index(animator, 0);
  }
}

static inline void player_collision()
{
  if(player_touch('R') == 1||
     player_touch('w') == 1||
     player.pos.x < 0 ||
     player.pos.x / state.tile_size > state.columns - 1 ||
     player.pos.y < 0 ||
     player.pos.y / state.tile_size > state.rows - 1)
  {
    player.pos.x -= player.dir.x * PLAYER_SPEED;
    player.pos.y -= player.dir.y * PLAYER_SPEED;
  }
}

#endif
