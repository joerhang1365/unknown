#include "player.h"
#include "globals.h"

player_t player;

void player_movement()
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
      case DOWN: player.dir.y = 1; break;
      default: player.dir.x = 0; player.dir.y = 0;
    }
    if(state.girl_show == 1)
    {     
      LEFT_SHIFT(player.prev_pos, player.pos, PLAYER_PREVIOUS);
    }
  }

  player.pos.x += round(player.dir.x * PLAYER_SPEED);
  player.pos.y += round(player.dir.y * PLAYER_SPEED);
}

void player_animation()
{
  if(state.key != NONE)
  {
    switch(state.key)
    {
      case LEFT: animator_set_index(&animations[PLAYER_ANIM], 2); break;
      case RIGHT: animator_set_index(&animations[PLAYER_ANIM], 1); break;
      case UP: animator_set_index(&animations[PLAYER_ANIM], 3); break;
      case DOWN: animator_set_index(&animations[PLAYER_ANIM], 4); break;
      default: animator_set_index(&animations[PLAYER_ANIM], 0);
    }
  }
  else
  {
    animator_set_index(&animations[PLAYER_ANIM], 0);
  }
}

byte player_touch(char c)
{
  return 
    is_type(player.pos.x, player.pos.y, c) ||
    is_type(player.pos.x + player.width - 1, player.pos.y, c) ||
    is_type(player.pos.x, player.pos.y + player.height - 1, c) ||
    is_type(player.pos.x + player.width - 1, player.pos.y + player.height - 1, c);
}

void player_collision()
{
  if(player_touch('R') == 1||
     player_touch('w') == 1||
     player.pos.x < 0 ||
     player.pos.x > (state.columns - 1) * state.tile_size ||
     player.pos.y < 1 ||
     player.pos.y > (state.rows - 1) * state.tile_size)
  {
    player.pos.x -= round(player.dir.x * PLAYER_SPEED);
    player.pos.y -= round(player.dir.y * PLAYER_SPEED);
  }
}

void player_load()
{
  player.width = PLAYER_WIDTH;
  player.height = PLAYER_HEIGHT;
  VECi2_TO_VEC(player.pos, find_position('p', 0)); 
  VECi2_MULTI(player.pos, state.tile_size);
  VECi2(player.dir, 0, 0);

  /* set all prev_pos to current pos */
  for(u32 i = 0; i < PLAYER_PREVIOUS; i++)
  {
    player.prev_pos[i] = player.pos;
  }
}
