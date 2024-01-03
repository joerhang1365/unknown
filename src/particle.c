#include "particle.h"
#include "globals.h"

void particle_sim_create(particle_sim_t *sim, u32 capacity) 
{
  sim->particles = malloc(sizeof(particle_t) * capacity);
  sim->size = 0;
  sim->capacity = capacity;
}

void particle_add(particle_sim_t *sim, const i32 x, const i32 y)
{
  RIGHT_SHIFT(sim->particles, sim->size);
  sim->particles[0].x = x;
  sim->particles[0].y = y;
  if (sim->size < sim->capacity) sim->size++;
}

i32 particle_render(particle_sim_t *sim, u16 color)
{
  i32 overflow = 0;

  for (u32 i = 0; i < sim->size; i++) 
  {
    const i32 x_adj = sim->particles[i].x - camera.x;
    const i32 y_adj = sim->particles[i].y - camera.y;
    const i32 pixels_index = y_adj * SCREEN_WIDTH + x_adj; 

    // is in screen boundary
    if (is_valid_pixel(SCREEN_MAX, SCREEN_WIDTH, SCREEN_HEIGHT, 
                      pixels_index, x_adj, y_adj))
    {
      state.pixels[pixels_index] = color;
    }
    else overflow = 1;
  }

  return overflow;
}

static f32 last_update_time = 0.0f;

void particle_float(particle_sim_t *sim, const i32 x, const i32 y, 
                   f32 update_time) 
{
  if (TIME - last_update_time >= update_time) 
  {
    last_update_time = TIME;
    particle_add(sim, x + rand() % state.tile_size, y);

    // update current particles upward
    for (u32 i = 0; i < sim->size; i++) 
    {
      sim->particles[i].y--;
    }
  }
}

static u32 wind_update_time = 0;

void particle_rain(particle_sim_t *sim, const i32 x, const i32 y, const f32 update_time)
{
  if (TIME - wind_update_time >= update_time)
  {
    wind_update_time = TIME;
    particle_add(sim, x + rand() % SCREEN_WIDTH * 2 - SCREEN_WIDTH, y);

    // make wind movement
    for (u32 i = 0; i < sim->size; i++)
    {
      sim->particles[i].x++;
      sim->particles[i].y++;
    }
  }
}

void particle_sim_destroy(particle_sim_t *sim) 
{
  free(sim->particles);
  sim->size = 0;
  sim->capacity = 0;
}
