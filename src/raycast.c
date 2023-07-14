#include "raycast.h"

void raycast_get_wall_heights(int *wall_heights, int screen_width, int screen_height, char *map, player player)
{
  int precision = 64;
  float increment_angle = player.field_of_view / screen_width;

  float ray_angle = player.angle - player.field_of_view / 2;
  for(int i = 0; i < screen_width; i++)
  {
    vector2 ray = { player.position.i, player.position.j };

    float ray_cos = cosf(ray_angle * PI / 180) / precision;
    float ray_sin = sinf(ray_angle * PI / 180) / precision;

    char wall = ' ';
    while(wall == ' ')
    {
      ray.i += ray_cos;
      ray.j += ray_sin;
      wall = map[(int)(ray.i) * SCREEN_WIDTH + (int)(ray.j)];
    }

    float distance = sqrt(pow(player.position.i - ray.i, 2)) + sqrt(pow(player.position.j - ray.j, 2));

    wall_heights[i] = (float) screen_height / 2 / distance;

    ray_angle += increment_angle;
  }
}

void raycast_render_map(int *wall_heights, SDL_Renderer *renderer)
{
  for(int i = 0; i < SCREEN_WIDTH; i++)
  {
    SDL_SetRenderDrawColor(renderer, wall_heights[i], wall_heights[i], wall_heights[i], wall_heights[i]);
    SDL_RenderDrawLine(renderer, i, SCREEN_HEIGHT / 2 - wall_heights[i], i, SCREEN_HEIGHT / 2 + wall_heights[i]);
  }
}

