#include "texture.h"
#include "globals.h"

texture_t textures[TEXTURE_MAX];

void texture_create(const char *source, const u32 type) 
{
  FILE *in = fopen(source, "rb");
  ASSERT(in == NULL, "error opening bitmap file\n");

  fscanf(in, "width=%u\n", &textures[type].width);
  fscanf(in, "height=%u\n", &textures[type].height);
  fscanf(in, "bytes_per_pixel=%u\n", &textures[type].bytes_per_pixel);

  if (textures[type].pixels != NULL) free(textures[type].pixels);
  textures[type].pixels = 
    (u16 *)malloc(sizeof(u32) * textures[type].width * textures[type].height);
  ASSERT(textures[type].pixels == NULL, "error allocating memory to pixels\n");

  u32 buffer;
  for (i32 i = 0; i < textures[type].height; i++) 
  {
    for (i32 j = 0; j < textures[type].width; j++) 
    {
      fscanf(in, "%x ", &buffer);
      textures[type].pixels[i * textures[type].width + j] = (u16)buffer;
    }
    fscanf(in, "\n");
  }
}

void texture_render(const i32 x, const i32 y, const u32 type) 
{
  for (u32 i = 0; i < textures[type].height; i++) 
  {
    for (u32 j = 0; j < textures[type].width; j++) 
    {
      i32 pixels_index = (i + y) * SCREEN_WIDTH + j + x;

      if (!is_valid_pixel(pixels_index, x + j, y + i)) continue;
      
      ALPHA_OVER(state.pixels[pixels_index],
          textures[type].pixels[i * textures[type].width + j],
          state.pixels[pixels_index]);
    }
  }
}

void texture_destroy(const u32 type) 
{
  free(textures[type].pixels);
  textures[type].pixels = NULL;
}
