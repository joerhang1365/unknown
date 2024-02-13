#ifndef LIGHTING_H
#define LIGHTING_H

#include "globals.h"
#include "state.h"
#include "texture.h"

void light_source(const i32 x, const i32 y, u32 radius, const u32 flicker,
          const u32 accuracy, const camera_t camera);
void glow(const i32 x, const i32 y, const u32 type); 

#endif
