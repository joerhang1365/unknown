#ifndef LIGHTING_H
#define LIGHTING_H

#include "globals.h"
#include "state.h"
#include "texture.h"
#include <wchar.h>

i32 light(const i32 x, const i32 y, u32 radius, const u32 flicker,
          const u32 accuracy);
i32 glow(const i32 x, const i32 y, const u32 type); 

#endif
