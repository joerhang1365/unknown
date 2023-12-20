#ifndef CORRUPTION_H
#define CORRUPTION_H

#include "vector.h"

typedef struct
{
  veci2 pos;
} corruption_t;

void corruption_update(corruption_t *corruption, veci2 target, char *map, i32 columns);

#endif
