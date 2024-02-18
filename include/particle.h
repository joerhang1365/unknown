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

void particle_sim_create(particle_sim_t *sim, const u32 capacity);
void particle_add(particle_sim_t *sim, const i32 x, const i32 y);
void particle_render(particle_sim_t *sim, const u16 color);
void particle_float(particle_sim_t *sim, const i32 x, const i32 y, 
                    const f32 update_time);
void particle_rain(particle_sim_t *sim, const i32 x, const i32 y, 
                   const veci2 dir, const f32 update_time);
void particle_wind(particle_sim_t *sim, const veci2 dir, const f32 update_time);
void particle_sim_destroy(particle_sim_t *sim);

#endif
