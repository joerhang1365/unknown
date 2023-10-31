#include "texture.h"

i32 texture_create(const char *source, texture_t *texture)
{
  i32 width, height, bytes_per_pixel;
  i32 success = 0;
  FILE *image = fopen(source, "rb");
  if(image == NULL)
  {
    printf("error opening bitmap file\n");
    success = 1;
  }

  fscanf(image, "width=%i\n", &width);
  fscanf(image, "height=%i\n", &height);
  fscanf(image, "bytes_per_pixel=%i\n", &bytes_per_pixel);

  texture->pixels = (u16*) malloc(sizeof(u32) * width * height);
  if(texture->pixels == NULL)
  {
    printf("error allocating memory to pixels\n");
  }

  u32 buffer;
  for(i32 i = 0; i < height; i++)
  {
    for(i32 j = 0; j < width; j++)
    {
      fscanf(image, "%x ", &buffer);
      texture->pixels[i * width + j] = (u16) buffer;
    }
    fscanf(image, "\n");
  }

  fclose(image);

  texture->width = width;
  texture->height = height;
  texture->bytes_per_pixel = bytes_per_pixel;

  return success;
}

i32 texture_add(texture_t texture, const i32 x, const i32 y, u16 *pixels, const i32 pixels_width, const i32 pixels_max)
{
  i32 overflow = 0;
  for(i32 i = 0; i < texture.height; i++)
  {
    for(i32 j = 0; j < texture.width; j++)
    {
      const i32 pixel_index = (i + y) * pixels_width + j + x;
      if(pixel_index > 0 && pixel_index < pixels_max && (j + x) >= 0 && (j + x) < pixels_width)
      {
        pixels[pixel_index] = texture.pixels[i * texture.width + j];
      }
    }
  }
  return overflow;
}

void texture_destroy(texture_t texture)
{
  free(texture.pixels);
}
