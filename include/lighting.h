#ifndef LIGHTING_H
#define LIGHTING_H

#include "globals.h"
#include "state.h"

#define LIGHT_ACCURACY 360
#define LIGHT_FLICKER 5

i32 flash_light(const i32 x, const i32 y, const veci2 camera, u32 radius);

#endif
