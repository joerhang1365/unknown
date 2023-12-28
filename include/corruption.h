#ifndef CORRUPTION_H
#define CORRUPTION_H

#include <stdlib.h>
#include "globals.h"

typedef struct
{
  veci2 pos;
} corruption_t;

void corruption_update(corruption_t *corruption, const veci2 target, char *map, const u32 columns);

#endif
