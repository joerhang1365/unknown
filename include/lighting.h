#ifndef LIGHTING_H
#define LIGHTING_H

#include "constants.h"
#include "type.h"
#include "math.h"
#include <stdlib.h>

#define LIGHT_ACCURACY 360

i32 flash_light(const i32 x, const i32 y, const u32 radius, const char *map, u16 *pixels, const u32 pixels_width, const u32 pixels_height);

#endif