#include "particle.h"
#include "globals.h"

void particle_sim_create(particle_sim_t *sim, u32 capacity) 
{
  if (sim->particles != NULL)
    free(sim->particles);

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

void particle_render(particle_sim_t *sim, const u16 color)
{
  for (u32 i = 0; i < sim->size; i++) 
  {
    const i32 x_adj = sim->particles[i].x - state.camera.x;
    const i32 y_adj = sim->particles[i].y - state.camera.y;
    const i32 pixels_index = y_adj * SCREEN_WIDTH + x_adj; 

    // is in screen boundary
    if (!is_valid_pixel(pixels_index, x_adj, y_adj)) continue;
    
    state.pixels[pixels_index] = color;
  }
}

static f64 float_update_time = 0;

void particle_float(particle_sim_t *sim, const i32 x, const i32 y, 
                   f32 update_time) 
{
  if (TIME - float_update_time < update_time) return;
  
  float_update_time = TIME;
  particle_add(sim, x + rand() % state.tile_size + 1, y);

  // update current particles upward
  for (u32 i = 0; i < sim->size; i++) 
  {
    sim->particles[i].y--;
  }
}

static f64 rain_update_time = 0;

void particle_rain(particle_sim_t *sim, const i32 x, const i32 y, const veci2 dir, const f32 update_time)
{
  if (TIME - rain_update_time < update_time) return;
  
  rain_update_time = TIME;

  u32 rain_x = x + rand() % (SCREEN_WIDTH * 2) + 1 - SCREEN_WIDTH_2;
  rain_x -= SCREEN_WIDTH_2 * dir.x;

  particle_add(sim, rain_x, y);

  // make rain movement
  for (u32 i = 0; i < sim->size; i++)
  {
    sim->particles[i].x += dir.x;
    sim->particles[i].y += dir.y;
  }
}

static f64 wind_update_time = 0;
static u32 wind_line_count = 0;
static u32 wind_y = 0;

void particle_wind(particle_sim_t *sim, const veci2 dir, const f32 update_time)
{
  if (TIME - wind_update_time < (double) update_time) return;

  wind_update_time = TIME;

  if (wind_line_count > 16)
  {
    wind_y = rand() % (SCREEN_HEIGHT * 2) + 1 - SCREEN_HEIGHT_2;
    wind_y -= SCREEN_HEIGHT_2 * dir.y;
    wind_line_count = 0;
  }

  particle_add(sim, (dir.x > 0) ? 0: SCREEN_WIDTH, wind_y);
  wind_line_count++;

  // main movement
  f32 wind_theta = 0;
  for (u32 i = 0; i < sim->size; i++)
  {
    sim->particles[i].x += dir.x;
    sim->particles[i].y += dir.y + roundf(sinf(wind_theta));
    wind_theta += PI / 4;
  }
}

void particle_sim_destroy(particle_sim_t *sim) 
{
  if (sim->particles != NULL) free(sim->particles);
  sim->size = 0;
  sim->capacity = 0;
}
