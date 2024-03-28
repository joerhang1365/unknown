#include "animator.h"
#include "texture.h"

animator_t animations[ANIMATION_MAX];

void animator_create(const u32 width, const u32 height, 
                     const u32 number_of_frames, const u32 type, 
                     const u32 text_type) 
{
  if (animations[type].textures != NULL)
    free(animations[type].textures);

  animations[type].textures = (texture_t*)malloc(sizeof(texture_t) * number_of_frames);
  ASSERT(animations[type].textures == NULL, "error allocating memory to animation textures\n");

  for (u32 i = 0; i < number_of_frames; i++)
  {
    //if (animations[type].textures[i].pixels != NULL)
    //  free(animations[type].textures[i].pixels);

    animations[type].textures[i].pixels = (u16*)malloc(sizeof(u16) * width * height);
    ASSERT(animations[type].textures[i].pixels == NULL, "error allocating memory to animation textures pixels\n");

    animations[type].textures[i].width = width;
    animations[type].textures[i].height = height;
    animations[type].textures[i].bytes_per_pixel = textures[text_type].bytes_per_pixel;

    for (u32 j = 0; j < height; j++) 
    {
      for (u32 k = 0; k < width; k++)
      {
        const u32 pixels_index = j * width + k;
        const u32 animator_index = j * textures[text_type].width + k + width * i;
        const u16 animator_pixel = textures[text_type].pixels[animator_index];

        animations[type].textures[i].pixels[pixels_index] = animator_pixel;
      }
    }
  }

  animations[type].width = width;
  animations[type].height = height;
  animations[type].number_of_frames = number_of_frames;
  animations[type].frame = 0;
  animations[type].index = 0;
}

void animator_set_index(const u32 frame, const u32 type) 
{
  animations[type].index = frame;
}

void animator_update(const f32 framerate, const u32 type) 
{
  animations[type].index = animations[type].frame / framerate;
  animations[type].frame += 1;

  if (animations[type].index > animations[type].number_of_frames - 1) 
  {
    animations[type].frame = 0;
    animations[type].index = 0;
  }
}

void animator_render(const i32 x, const i32 y, const u32 type) 
{
  for (u32 i = 0; i < animations[type].height; i++) 
  {
    for (u32 j = 0; j < animations[type].width; j++) 
    {
      i32 pixels_index = (i + y) * SCREEN_WIDTH + j + x;

      if (!is_valid_pixel(pixels_index, x + j, y + i)) continue;
      
      ALPHA_OVER(state.pixels[pixels_index],
                animations[type].textures[animations[type].index].pixels[i * animations[type].width + j],
                state.pixels[pixels_index]);
    }
  }
}

void animator_destroy(const u32 type)
{
  for (u32 i = 0; i < animations[type].number_of_frames; i++)
  {
    if (animations[type].textures[i].pixels != NULL)
      free(animations[type].textures[i].pixels);

    animations[type].textures[i].pixels = NULL;
  }

  if (animations[type].textures != NULL)
    free(animations[type].textures); 

  animations[type].textures = NULL;
}
