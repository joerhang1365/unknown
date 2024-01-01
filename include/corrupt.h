#ifndef CORRUPT_H
#define CORRUPT_H

#include "globals.h"
#include "state.h"

#define CORRUPT_SPEED state.tile_size
#define CORRUPT_TIME 4

typedef veci2 corrupt_t;

typedef struct
{
  corrupt_t *corrupts;
  u32 count;
} corruption_t;

void corrupt_load(corruption_t *corruption);
void corrupt_update(corruption_t * corruption, veci2 target);
void corrupt_destroy(corruption_t *corruption);

#endif
