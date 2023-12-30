#include "lighting.h"
#include "globals.h"

i32 flash_light(const i32 x, const i32 y, const veci2 camera, u32 radius, const f32 time)
{
  i32 overflow = 0;
  u16 temp_pixels[SCREEN_MAX];

  // copy all the pixels
  for(u32 i = 0; i < SCREEN_MAX; i++)
  {
    temp_pixels[i] = state.pixels[i];
  }

  // convert all pixels to black
  SCREEN_CLEAR(state.pixels, SCREEN_MAX);

  // add a little bit a flicker to light
  if((u32)time % 2 == 0)
  {
    radius += LIGHT_FLICKER;
  }

  for(f32 theta = 0; theta < 2 * PI; theta += (f32) PI / LIGHT_ACCURACY)
  {
    for(u32 r = 0; r < radius; r++)
    {
      const i32 cylindrical_x = r * cosf(theta) + x - camera.x;
      const i32 cylindrical_y = r * sinf(theta) + y - camera.y;
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
        state.pixels[pixels_index] = temp_pixels[pixels_index];

        // adjust transparency the further from light source
        state.pixels[pixels_index] = state.pixels[pixels_index] & 0xFFF0;
        state.pixels[pixels_index] += (u16)(16 * (1.0f - (f32)r / radius));
        ALPHA_BLEND(state.pixels[pixels_index]);
      }

        // make light not pass through solid objects
        u32 column = (f32)(cylindrical_x - camera.x) / state.tile_size;
        u32 row = (f32)(cylindrical_y - camera.y) / state.tile_size;
   
        // want light pass through type it detects so must move 
        // forward once depending on light direction 
        if(roundf(cosf(theta)) > 0.0f)
        {
          column--;
        }
        else if(roundf(cosf(theta)) < 0.0f)
        {
          column++;
        }
        if(roundf(sinf(theta)) > 0.0f)
        {
          row--;
        }
        else if(roundf(sinf(theta)) < 0.0f)
        {
          row++;
        }

        column = column & 0x000F;
        row = row & 0x000F;

        char type = state.map[row * state.columns + column];

        if(type == 'R') break;
    }
  }

  return overflow;
}
