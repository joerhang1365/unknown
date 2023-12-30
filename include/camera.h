#ifndef CAMERA_H
#define CAMERA_H

#include "state.h"

#define CAMERA_SPEED 2
#define CAMERA_CONSTRAINT 12 * state.tile_size

veci2 camera_reset();
void camera_follow(veci2 target, veci2 *camera);

#endif
