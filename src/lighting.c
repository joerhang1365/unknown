#include "lighting.h"
#include "animator.h"
#include "globals.h"
#include "state.h"
#include "texture.h"

i32 flash_light(const i32 x, const i32 y, u32 radius)
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

  // add flicker to light
  if((u32)TIME % 2 == 0)
  {
    radius += LIGHT_FLICKER;
  }

  for(f32 theta = 0; theta < PI2; theta += (f32) PI / LIGHT_ACCURACY)
  { 
    byte solid = 0;

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
        // check if type is solid
        // if so wait until type is no longer solid then break
        const u32 column = (f32)(cylindrical_x + camera.x) / state.tile_size;
        const u32 row = (f32)(cylindrical_y + camera.y) / state.tile_size; 
        const char type = get_type(column, row); 
        texture_t texture;

        // making shadows pixel accurate and shit
        if(type == 'R') texture = textures[ROCK_TXT];
        else if(type == 'G') texture = textures[GIRL_TXT];
        else texture = textures[BLANK_TXT];

        const u16 texture_pixel = texture.pixels[
              (cylindrical_y - (row * state.tile_size - camera.y)) * 
              textures[ROCK_TXT].width + 
               cylindrical_x - (column * state.tile_size - camera.x)];

        if(texture_pixel != 0x0000) solid = 1;
        if(solid == 1 && texture_pixel == 0x0000) break;

        // add da pixel color
        state.pixels[pixels_index] = temp_pixels[pixels_index];

        // adjust transparency the further from light source
        state.pixels[pixels_index] = state.pixels[pixels_index] & 0xFFF0;
        state.pixels[pixels_index] += (u16)(16.0f * (1.0f - (f32)r / radius));
        ALPHA_BLEND(state.pixels[pixels_index]);
      }
    }
  }

  // pos pixel does not automaticy get rendered forsum reason
  state.pixels[(y - camera.y) * SCREEN_WIDTH + x - camera.x] = 
    temp_pixels[(y - camera.y) * SCREEN_WIDTH + x - camera.x];

  return overflow;
}

i32 glow(const i32 x, const i32 y, texture_t texture)
{
  i32 overflow = 0;
  u16 alpha = fabs(sinf(TIME) * 16.0f);

  // change texture alpha value
  for(u32 i = 0; i < texture.height; i++)
  {
    for(u32 j = 0; j < texture.width; j++)
    {
      const u32 texture_index = i * texture.width + j;
      const u32 pixels_index = (y - camera.y + i) * SCREEN_WIDTH + x - camera.x + j;
      if(texture.pixels[texture_index] != 0x0000)
      {
        // check if current alpha is greater than alpha
        u16 current_alpha = state.pixels[pixels_index] & 0x000F;
        if(current_alpha > alpha) alpha = current_alpha;
        // clear current alpha
        texture.pixels[texture_index] = texture.pixels[texture_index] & 0xFFF0;
        // update current alpha
        texture.pixels[texture_index] = texture.pixels[texture_index] + alpha;
      }
    }
  }

  overflow = texture_add(texture, x - camera.x, y - camera.y);

  return overflow;
}
