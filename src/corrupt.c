#include "corrupt.h"
#include "globals.h"

void corrupt_load(corruption_t *corruption)
{
  u32 count = 0;

  for(u32 i = 0; i < state.columns * state.rows; i++)
  {
    if(state.map[i] == 'C')
    {
      count++;
    }
  }

  //if(corruption->corrupts != NULL) free(corruption->corrupts);
  corruption->corrupts = malloc(sizeof(corrupt_t) * count);
  ASSERT(corruption->corrupts == NULL, "failed to allocate memory for corrupts\n");

  if(count > 0)
  {
    for(u32 i = 0; i < count; i++)
    {
      corruption->corrupts[i] = find_position('C', i);
    }
  }

  corruption->count = count;
}

static f32 last_update_time = 0;

void corrupt_update(corruption_t *corruption, const veci2 target)
{
  if(TIME - last_update_time < CORRUPT_TIME) return;
  last_update_time = TIME;

  for(u32 i = 0; i < corruption->count; i++)
  {
    i32 x = corruption->corrupts[i].x;
    i32 y = corruption->corrupts[i].y;
    // set current char to blank
    state.map[y * state.columns + x] = ' ';
  
    // find dir to target
    veci2 dif;
    VECi2(dif, abs(target.x - x), abs(target.y - y));

    if(dif.x >= dif.y)
    {
      if(target.x > x) x++;
      else x--;
    }
    else if(dif.x < dif.y)
    {
      if(target.y > y) y++;
      else y--;
    }

    state.map[y * state.columns + x] = 'C';
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
