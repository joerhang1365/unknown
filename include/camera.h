#ifndef CAMERA_H
#define CAMERA_H

#include "globals.h"

#define CAMERA_SPEED round(64 * DELTA_TIME)

typedef vec2 camera_t;

void camera_follow(const vec2 target, const f32 offset, const f32 lazy, camera_t *camera);

#endif
