#ifndef CORRUPT_H
#define CORRUPT_H

#include "globals.h"
#include "state.h"

#define CORRUPT_TIME 4

/* returns new corrupt number */
u32 corrupt_load(veci2 *corrupts);
void corrupt_update(veci2 *corrupt, const veci2 target);

#endif
