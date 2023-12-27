#ifndef LIGHTING_H
#define LIGHTING_H

#include "constants.h"
#include "type.h"
#include "math.h"

#define LIGHT_ACCURACY 360

i32 spotlight_add(const i32 x, const i32 y, const u16 color, const u32 radius, const char *map, u16 *pixels, const u32 pixels_width, const u32 pixels_max);

#endif
