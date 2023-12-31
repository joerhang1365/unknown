#include "lighting.h"
#include "globals.h"

i32 flash_light(const i32 x, const i32 y, const veci2 camera, u32 radius, const f32 time)
{
  i32 overflow = 0;
  u16 temp_pixels[SCREEN_MAX];
  i32 x_adj = x - camera.x;
  i32 y_adj = y - camera.y;

  // copy all the pixels
  for(u32 i = 0; i < SCREEN_MAX; i++)
  {
    temp_pixels[i] = state.pixels[i];
  }

  // convert all pixels to black
  SCREEN_CLEAR(state.pixels, SCREEN_MAX);

  // add flicker to light
  if((u32)time % 2 == 0)
  {
    radius += LIGHT_FLICKER;
  }

  for(f32 theta = 0; theta < PI2; theta += (f32) PI / LIGHT_ACCURACY)
  {
    byte solid = 0;

    for(u32 r = 0; r < radius; r++)
    {
      const i32 cylindrical_x = r * cosf(theta) + x_adj;
      const i32 cylindrical_y = r * sinf(theta) + y_adj;
      const i32 pixels_index = cylindrical_y * SCREEN_WIDTH + cylindrical_x;
      overflow = pixels_index > SCREEN_MAX; 

      // is in screen boundary
      if(pixels_index >= 0 && 
        overflow == 0 && 
        cylindrical_x >= 0 && 
        cylindrical_x < SCREEN_WIDTH &&
        cylindrical_y >= 0 &&
        cylindrical_y < SCREEN_HEIGHT)
      {
        // check if type is solid
        // if so wait until type is no longer solid then break
        u32 column = (f32)(cylindrical_x + camera.x) / state.tile_size;
        u32 row = (f32)(cylindrical_y + camera.y) / state.tile_size; 
        char type = state.map[row * state.columns + column];

        if(type == 'R') solid = 1;
        if(solid == 1 && type != 'R') break;

        state.pixels[pixels_index] = temp_pixels[pixels_index];

        // adjust transparency the further from light source
        state.pixels[pixels_index] = state.pixels[pixels_index] & 0xFFF0;
        state.pixels[pixels_index] += (u16)(16.0f * (1.0f - (f32)r / radius));
        ALPHA_BLEND(state.pixels[pixels_index]);
      }
    }
  }

  // pos pixel does not automaticy get rendered for sum reason
  state.pixels[y_adj * SCREEN_WIDTH + x_adj] = temp_pixels[y_adj * SCREEN_WIDTH + x_adj];

  return overflow;
}
