#ifndef CORRUPTION_H
#define CORRUPTION_H

#include <stdlib.h>
#include "globals.h"

void corruption_load(veci2 *corruptions, u32 *corruption_number);
void corruption_update(veci2 *corruption, const veci2 target, char *map, const u32 columns);

#endif
