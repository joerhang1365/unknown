#include "raycast.h"
#include "constants.h"

void raycast_get_wall_heights(int wall_heights[SCREEN_WIDTH], level level, player player)
{
  int precision = 64;
  float increment_angle = player.field_of_view / SCREEN_WIDTH;

  float ray_angle = player.angle - player.field_of_view / 2;
  for(int i = 0; i < SCREEN_WIDTH; i++)
  {
    vector2 ray = { player.position.i, player.position.j };

    float ray_cos = cosf(ray_angle * PI / 180) / precision;
    float ray_sin = sinf(ray_angle * PI / 180) / precision;

    char wall = ' ';
    while(wall == ' ')
    {
      ray.i += ray_cos;
      ray.j += ray_sin;
      wall = level.map[(int)(ray.i) * level.columns + (int)(ray.j)];
    }

    float distance = sqrt(pow(player.position.i - ray.i, 2)) + sqrt(pow(player.position.j - ray.j, 2));

    wall_heights[i] = (float) SCREEN_HEIGHT / 2 / distance;

    ray_angle += increment_angle;
  }
}

void raycast_render_map(int wall_heights[SCREEN_WIDTH], SDL_Renderer *renderer)
{
  for(int i = 0; i < SCREEN_WIDTH; i++)
  {
    SDL_SetRenderDrawColor(renderer, wall_heights[i], wall_heights[i], wall_heights[i], wall_heights[i]);
    SDL_RenderDrawLine(renderer, i, SCREEN_HEIGHT / 2 - wall_heights[i], i, SCREEN_HEIGHT / 2 + wall_heights[i]);
  }
}

