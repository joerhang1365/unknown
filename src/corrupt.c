#include "corrupt.h"

void corrupt_load(corruption_t *corruption) 
{
  u32 count = 0;

  for (u32 i = 0; i < state.columns * state.rows; i++) 
  {
    if (state.map[i] == 'C') count++;
  }

  // if(corruption->corrupts != NULL) free(corruption->corrupts);
  corruption->corrupts = malloc(sizeof(corrupt_t) * count);
  ASSERT(corruption->corrupts == NULL,
         "failed to allocate memory for corrupts\n");

  if (count > 0) 
  {
    for (u32 i = 0; i < count; i++) 
    {
      corruption->corrupts[i] = find_position('C', i);
    }
  }

  corruption->count = count;
}

static u32 corrupt_last_update = 0;

void corrupt_update(corruption_t *corruption, veci2 target) 
{
  if (TIME - corrupt_last_update < CORRUPT_TIME) return;

  corrupt_last_update = TIME;

  for (u32 i = 0; i < corruption->count; i++) 
  {
    i32 x = corruption->corrupts[i].x;
    i32 y = corruption->corrupts[i].y;

    // remove char from map
    state.map[y / state.tile_size * state.columns + x / state.tile_size] = ' ';

    // find dir to target
    veci2 dif;
    VECi2(dif, abs(target.x - x), abs(target.y - y));

    if (dif.x >= dif.y) 
    {
      if (target.x > x) x += CORRUPT_SPEED;
      else x -= CORRUPT_SPEED;
    } 
    else if (dif.x < dif.y) 
    {
      if (target.y > y) y += CORRUPT_SPEED;
      else y -= CORRUPT_SPEED;
    }

    state.map[y / state.tile_size * state.columns + x / state.tile_size] = 'C';

    corruption->corrupts[i].x = x;
    corruption->corrupts[i].y = y;
  }
}

void corrupt_destroy(corruption_t *corruption) 
{
  free(corruption->corrupts);
  corruption->corrupts = NULL;
  corruption->count = 0;
}
