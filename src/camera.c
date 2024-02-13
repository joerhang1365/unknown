#include "camera.h"
#include "globals.h"
#include "state.h"

#define ROW_MAX state.rows * state.tile_size - SCREEN_HEIGHT
#define COLUMN_MAX state.columns * state.tile_size - SCREEN_WIDTH

void camera_follow(const vec2 target, const f32 offset, const f32 lazy,
                   camera_t *camera)
{
  if (target.x < camera->x + (offset - lazy) * state.tile_size && 
      camera->x > 0)
  {
    camera->x -= CAMERA_SPEED;
  }
  else if (target.x > camera->x + (offset + lazy) * state.tile_size && 
           camera->x < COLUMN_MAX)
  {
    camera->x += CAMERA_SPEED;
  }
  else if (target.y < camera->y + (offset - lazy) * state.tile_size && 
           camera->y > 0)
  {
    camera->y -= CAMERA_SPEED;
  }
  else if (target.y > camera->y + (offset + lazy) * state.tile_size && 
           camera->y < ROW_MAX)
  {
    camera->y += CAMERA_SPEED;
  }
}
