#include "player.h"

void player_load(player *player, SDL_Renderer *renderer, char *source, int x, int y, unsigned int width, unsigned int height, unsigned int number_of_frames) 
{
  texture_create(&player->texture, source, renderer);
  animator_create(&player->animation, player->texture, width, height, number_of_frames);
  player->position.i = x;
  player->position.j = y;
  player->field_of_view = 90;
  player->angle = 0;
}

void player_collision(player *player) 
{
  if (player->velocity.j < 0) 
  {
    player->position.j += SCREEN_SCALE;
    player->bob = false;
  } 
  else if (player->velocity.j > 0) 
  {
    player->position.j -= SCREEN_SCALE;
    player->bob = false;
  }

  if (player->velocity.i > 0) 
  {
    player->position.i -= SCREEN_SCALE;
  } 
  else if (player->velocity.i < 0) 
  {
    player->position.i += SCREEN_SCALE;
  }
}

void player_update(player *player, float delta_time, int arrow_key) 
{
  // animation
  animator_update_clip(&player->animation, 60);

  // movement
  player->velocity.j = 0;
  player->velocity.i = 0;

  switch (arrow_key) {
  case UP:
    player->velocity.j = -1 * SCREEN_SCALE;
    break;
  case DOWN:
    player->velocity.j = 1 * SCREEN_SCALE;
    break;
  case LEFT:
    player->velocity.i = -1 * SCREEN_SCALE;
    break;
  case RIGHT:
    player->velocity.i = 1 * SCREEN_SCALE;
    break;
  }

  // edge boundaries
  if (player->position.j < 0) 
  {
    player->position.j = 0;
  } 
  else if (player->position.j > SCREEN_HEIGHT * SCREEN_SCALE - PLAYER_HEIGHT * SCREEN_SCALE) 
  {
    player->position.j = SCREEN_HEIGHT * SCREEN_SCALE - PLAYER_HEIGHT * SCREEN_SCALE;
  }

  if (player->position.i < 0) 
  {
    player->position.i = 0;
  } 
  else if (player->position.i > SCREEN_WIDTH * SCREEN_SCALE - PLAYER_WIDTH * SCREEN_SCALE) 
  {
    player->position.i = SCREEN_WIDTH * SCREEN_SCALE - PLAYER_WIDTH * SCREEN_SCALE;
  }

  // bob up and down
  if (player->bob) 
  {
    player->time += delta_time;
    player->y_offset += FLOAT_AMPLITUDE * sin(2 * PI * player->time);
    if (player->y_offset > SCREEN_SCALE) 
    {
      player->velocity.j += SCREEN_SCALE;
      player->y_offset = 0;
    } 
    else if (player->y_offset < -SCREEN_SCALE) 
    {
      player->velocity.j -= SCREEN_SCALE;
      player->y_offset = 0;
    }
  }

  player->position.j += player->velocity.j;
  player->position.i += player->velocity.i;
  player->bob = true;
}

void player_render(player player, SDL_Renderer *renderer) 
{
  animator_render(player.animation, renderer, player.position.i, player.position.j, PLAYER_WIDTH * SCREEN_SCALE, PLAYER_HEIGHT * SCREEN_SCALE);
}

void player_unload(player *player) 
{ 
  animator_destroy(&player->animation); 
}
