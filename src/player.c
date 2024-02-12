#include "player.h"

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
  player.pos = find_position('p', 0);
  player.dir = veci2_create(0, 0);

  /* set all prev_pos to current pos */
  for (u32 i = 0; i < PLAYER_PREVIOUS; i++) 
  {
    player.prev_pos[i] = player.pos;
  }
}

void player_movement(const u32 key)
{
  player.dir.x = 0;
  player.dir.y = 0;

  switch (key) 
  {
    case LEFT:  player.dir.x = -1; break;
    case RIGHT: player.dir.x = 1; break;
    case UP:    player.dir.y = -1; break;
    case DOWN:  player.dir.y = 1; break;
    default:   player.dir.x = 0; player.dir.y = 0;
  }
      
  if (key == LEFT ||
      key == RIGHT ||
      key == UP ||
      key == DOWN)
  {
    LEFT_SHIFT(player.prev_pos, player.pos, PLAYER_PREVIOUS);
  }

  player.pos.x += round(player.dir.x * PLAYER_SPEED);
  player.pos.y += round(player.dir.y * PLAYER_SPEED);
}

void player_collision(const u32 columns, const u32 rows, const u32 tile_size)
{
  if (player_touch('R') || player_touch('w') ||
      player_touch('G') || player.pos.x < 0 ||
      player.pos.x > (columns - 1) * tile_size ||
      player.pos.y < 1 ||
      player.pos.y > (rows - 1) * tile_size) 
  {
    player.pos.x -= round(player.dir.x * PLAYER_SPEED);
    player.pos.y -= round(player.dir.y * PLAYER_SPEED);
  } 
}

void player_animation(const u32 key)
{
  switch (key)
  {
    case LEFT:  animator_set_index(2, PLAYER_ANIM); break;
    case RIGHT: animator_set_index(1, PLAYER_ANIM); break;
    case UP:    animator_set_index(3, PLAYER_ANIM); break;
    case DOWN:  animator_set_index(4, PLAYER_ANIM); break;
    default:    animator_set_index(0, PLAYER_ANIM);
  }
}

