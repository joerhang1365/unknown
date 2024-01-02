#ifndef PARTICLE_H
#define PARTICLE_H

#include "globals.h"
#include "state.h"

typedef veci2 particle_t;

typedef struct
{
  particle_t *particles;
  u32 size;
  u32 capacity;
} particle_sim_t;

void particle_sim_create(particle_sim_t *sim, u32 capacity);
i32 particle_float(particle_sim_t *sim, i32 x, i32 y, u16 color, 
                   f32 update_time);
void particle_sim_destroy(particle_sim_t *sim);

#endif
