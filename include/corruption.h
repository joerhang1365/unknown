#ifndef CORRUPTION_H
#define CORRUPTION_H

#include <stdlib.h>
#include "globals.h"

/* returns new corrupt number */
u32 corrupt_load(veci2 *corrupts, u32 corrupt_num);
void corrupt_update(veci2 *corrupt, const veci2 target);

#endif
