#include "particle.h"
#include "globals.h"

void particle_sim_create(particle_sim_t *sim, u32 capacity)
{
  sim->particles = malloc(sizeof(particle_t) * capacity);
  sim->size = 0;
  sim->capacity = capacity;
}

static f32 last_update_time = 0.0f;

i32 particle_float(particle_sim_t *sim, i32 x, i32 y, u16 color, f32 update_time)
{
  if(TIME - last_update_time >= update_time)
  {
    // add new particle
    RIGHT_SHIFT(sim->particles, sim->size);
    sim->particles[0].x = x + rand() % state.tile_size;
    sim->particles[0].y = y;
    if(sim->size < sim->capacity) sim->size++;

    // update current particles upward
    for(u32 i = 0; i < sim->size; i++)
    {
      sim->particles[i].y--;
    }
    last_update_time = TIME;
  }

  // render particles
  i32 overflow;

  for(u32 i = 0; i < sim->size; i++)
  {
    const i32 x_adj = sim->particles[i].x - camera.x;
    const i32 y_adj = sim->particles[i].y - camera.y;
    const i32 pixels_index = y_adj * SCREEN_WIDTH + x_adj;
    overflow = pixels_index > SCREEN_MAX; 

    // is in screen boundary
    if(pixels_index >= 0 && 
       overflow == 0 && 
       x_adj >= 0 && 
       x_adj < SCREEN_WIDTH &&
       y_adj >= 0 &&
       y_adj < SCREEN_HEIGHT)
    {
      state.pixels[pixels_index] = color;
    }
  }

  return overflow;
}

void particle_sim_destroy(particle_sim_t *sim)
{
  free(sim->particles);
  sim->size = 0;
  sim->capacity = 0;
}
