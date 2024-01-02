#include "player.h"
#include "globals.h"

player_t player;

byte player_touch(char c) 
{
  return 
    is_type(player.pos.x, player.pos.y, c) ||
    is_type(player.pos.x + player.width - 1, player.pos.y, c) ||
    is_type(player.pos.x, player.pos.y + player.height - 1, c) ||
    is_type(player.pos.x + player.width - 1, player.pos.y + player.height - 1, c);
}

void player_load() 
{
  player.width = PLAYER_WIDTH;
  player.height = PLAYER_HEIGHT;
  VECi2_TO_VEC(player.pos, find_position('p', 0));
  VECi2(player.dir, 0, 0);

  /* set all prev_pos to current pos */
  for (u32 i = 0; i < PLAYER_PREVIOUS; i++) 
  {
    player.prev_pos[i] = player.pos;
  }
}
