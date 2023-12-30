#ifndef CORRUPT_H
#define CORRUPT_H

#include "globals.h"
#include "state.h"


/* returns new corrupt number */
u32 get_corrupt_num();
void corrupt_load(veci2 *corrupts, u32 corrupt_num);
void corrupt_update(veci2 *corrupt, const veci2 target);

#endif
