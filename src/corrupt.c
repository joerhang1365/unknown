#include "corrupt.h"

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
      VECi2_MULTI(corruption->corrupts[i], state.tile_size);
    }
  }

  corruption->count = count;
}

void corrupt_destroy(corruption_t *corruption)
{
  free(corruption->corrupts);
  corruption->corrupts = NULL;
  corruption->count = 0;
}
