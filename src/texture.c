#include "texture.h"

texture_t textures[TEXTURE_MAX];

i32 texture_create(const char *source, texture_t *texture) 
{
  i32 width, height, bytes_per_pixel;
  i32 success = 0;
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

i32 texture_add(texture_t texture, const i32 x, const i32 y) 
{
  i32 overflow = 0;

  for (u32 i = 0; i < texture.height; i++) 
  {
    for (u32 j = 0; j < texture.width; j++) 
    {
      i32 pixels_index = (i + y) * SCREEN_WIDTH + j + x;
      const u16 texture_pixel = texture.pixels[i * texture.width + j];

      if (pixels_index >= 0 && overflow == 0 && (j + x) >= 0 &&
          (j + x) < SCREEN_WIDTH && (i + y) >= 0 && (i + y) < SCREEN_HEIGHT) 
      {
        pixels_index = pixels_index;
        ALPHA_BLEND_OVER(state.pixels[pixels_index], texture_pixel,
                         state.pixels[pixels_index]);
      }
    }
  }

  return overflow;
}

void texture_destroy(texture_t *texture) 
{
  free(texture->pixels);
  texture->pixels = NULL;
}
