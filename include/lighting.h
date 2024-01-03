#ifndef LIGHTING_H
#define LIGHTING_H

#include "globals.h"
#include "state.h"
#include "texture.h"
#include <wchar.h>

#define LIGHT_ACCURACY 360
#define LIGHT_FLICKER 5

i32 flash_light(const i32 x, const i32 y, u32 radius);
i32 glow(const i32 x, const i32 y, const u32 type); 

#endif
