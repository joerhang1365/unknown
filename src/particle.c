#include "particle.h"
#include "globals.h"

void particle_sim_create(particle_sim_t *sim, u32 capacity) 
{
  if (sim->particles == NULL)
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

void particle_render(particle_sim_t *sim, const u16 color, const camera_t camera)
{
  for (u32 i = 0; i < sim->size; i++) 
  {
    const i32 x_adj = sim->particles[i].x - camera.x;
    const i32 y_adj = sim->particles[i].y - camera.y;
    const i32 pixels_index = y_adj * SCREEN_WIDTH + x_adj; 

    // is in screen boundary
    if (!is_valid_pixel(pixels_index, x_adj, y_adj)) continue;
    
    state.pixels[pixels_index] = color;
  }
}

static f32 last_update_time = 0.0f;

void particle_float(particle_sim_t *sim, const i32 x, const i32 y, 
                   f32 update_time) 
{
  if (TIME - last_update_time < update_time) return;
  
  last_update_time = TIME;
  particle_add(sim, x + rand() % state.tile_size, y);

  // update current particles upward
  for (u32 i = 0; i < sim->size; i++) 
  {
    sim->particles[i].y--;
  }
}

static u32 wind_update_time = 0;

void particle_rain(particle_sim_t *sim, const i32 x, const i32 y, const veci2 dir, const f32 update_time)
{
  if (TIME - wind_update_time < update_time) return;
  
  wind_update_time = TIME;
  particle_add(sim, x + rand() % SCREEN_WIDTH - SCREEN_WIDTH_2 * dir.x, y);

  // make rain movement
  for (u32 i = 0; i < sim->size; i++)
  {
    sim->particles[i].x += dir.x;
    sim->particles[i].y += dir.y;
  }
}

void particle_sim_destroy(particle_sim_t *sim) 
{
  if (sim->particles != NULL) free(sim->particles);
  sim->size = 0;
  sim->capacity = 0;
}
