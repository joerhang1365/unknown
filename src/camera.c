#include "camera.h"

veci2 camera_reset()
{
  veci2 temp;
  VECi2(temp, 0, 0);

  return temp;
}

void camera_follow(veci2 target, veci2 *camera)
{
  if(camera->x + state.columns / 2 * state.tile_size < target.x)
  {
    camera->x += CAMERA_SPEED;
  }
  else if(camera->x + state.columns / 2 * state.tile_size > target.x)
  {
    camera->x -= CAMERA_SPEED;
  }
}
