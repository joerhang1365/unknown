#include "lighting.h"
#include "globals.h"
#include "state.h"
#include "texture.h"
#include <stdio.h>

void light(const i32 x, const i32 y, u32 radius, const u32 flicker,
          const u32 accuracy)
{
  // add flicker to light
  if ((u32)TIME % 2 == 0) radius += flicker; 

  for (f32 theta = 0; theta < PI2; theta += PI2 / accuracy) 
  {
    byte solid = 0;

    for (u32 r = 0; r < radius; r++) 
    {
      const i32 cylindrical_x = r * cosf(theta) + x - camera.x;
      const i32 cylindrical_y = r * sinf(theta) + y - camera.y;
      const i32 pixels_index = cylindrical_y * SCREEN_WIDTH + cylindrical_x;
  
      // is in screen boundary
      if (!is_valid_pixel(pixels_index, cylindrical_x, cylindrical_y)) continue;

      // check if type is solid
      // if so wait until type is no longer solid then break
      const u32 column = (f32)(cylindrical_x + camera.x) / state.tile_size;
      const u32 row = (f32)(cylindrical_y + camera.y) / state.tile_size;
      const char type = get_type(column, row);

      // making shadows pixel accurate and shit
      texture_t texture;
      switch (type) 
      {
        case 'R': texture = textures[ROCK_TXT]; break;
        case 'G': texture = textures[GIRL_TXT]; break;
        default: texture = textures[BLANK_TXT]; break;
      }

      const u16 texture_pixel = 
        texture.pixels[(cylindrical_y - (row * state.tile_size - camera.y)) *
        textures[ROCK_TXT].width + cylindrical_x - (column * state.tile_size - camera.x)];

      if (texture_pixel != 0x0000) solid = 1;
      if (solid == 1 && texture_pixel == 0x0000) break;

      // add da pixel color
      state.pixels[pixels_index] &= 0xFFF0;
      state.pixels[pixels_index] += (u16)(15.0f * (1.0f - (f32)r / radius));
    }
  }
}

void glow(const i32 x, const i32 y, const u32 type) 
{
  u16 alpha = fabs(sinf(TIME) * 15.0f);
  alpha &= 0x000F; // make sure between 0 and 15

  const texture_t texture = textures[type];

  for (u32 i = 0; i < texture.height; i++) 
  {
    for (u32 j = 0; j < texture.width; j++) 
    {
      const u32 pixels_index = (y + i) * SCREEN_WIDTH + x + j;
      const u32 texture_pixel = texture.pixels[i * texture.width + j];

      // check if is on screen
      if (texture_pixel == 0x0000 || !is_valid_pixel(pixels_index, (x + j), (y + i)))
        continue;

      if ((state.pixels[pixels_index] & 0x000F) > alpha) continue;

      state.pixels[pixels_index] &= 0xFFF0;
      state.pixels[pixels_index] += alpha;
    }
  }
}
