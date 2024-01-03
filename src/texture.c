#include "texture.h"
#include "globals.h"

texture_t textures[TEXTURE_MAX];

i32 texture_create(const char *source, const u32 type) 
{
  i32 width, height, bytes_per_pixel;
  i32 success = 0;
  texture_t *texture = &textures[type];
  FILE *image = fopen(source, "rb");
  ASSERT(image == NULL, "error opening bitmap file\n");

  fscanf(image, "width=%i\n", &width);
  fscanf(image, "height=%i\n", &height);
  fscanf(image, "bytes_per_pixel=%i\n", &bytes_per_pixel);

  if (texture->pixels != NULL) free(texture->pixels);
  texture->pixels = (u16 *)malloc(sizeof(u32) * width * height);
  ASSERT(texture->pixels == NULL, "error allocating memory to pixels\n");

  u32 buffer;
  for (i32 i = 0; i < height; i++) 
  {
    for (i32 j = 0; j < width; j++) 
    {
      fscanf(image, "%x ", &buffer);
      texture->pixels[i * width + j] = (u16)buffer;
    }
    fscanf(image, "\n");
  }

  fclose(image);

  texture->width = width;
  texture->height = height;
  texture->bytes_per_pixel = bytes_per_pixel;

  return success;
}

i32 texture_render(const i32 x, const i32 y, const u32 type) 
{
  texture_t texture = textures[type];
  i32 overflow = 0;

  for (u32 i = 0; i < texture.height; i++) 
  {
    for (u32 j = 0; j < texture.width; j++) 
    {
      i32 pixels_index = (i + y) * SCREEN_WIDTH + j + x;

      if (is_valid_pixel(SCREEN_MAX, SCREEN_WIDTH, SCREEN_HEIGHT, 
                         pixels_index, x + j, y + i))
      {
        ALPHA_BLEND_OVER(state.pixels[pixels_index], 
                         texture.pixels[i * texture.width + j],
                         state.pixels[pixels_index]);
      }
      else overflow = 1;
    }
  }

  return overflow;
}

void texture_destroy(const u32 type) 
{
  texture_t *texture = &textures[type];
  free(texture->pixels);
  texture->pixels = NULL;
}
