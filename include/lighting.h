#ifndef LIGHTING_H
#define LIGHTING_H

#include "globals.h"
#include "state.h"
#include "texture.h"

void light_source(const i32 x, const i32 y, u32 radius, const u32 flicker,
          const u32 accuracy);
void glow(const i32 x, const i32 y, const u32 type);
void sun_shadows(const veci2 dir);

#endif
