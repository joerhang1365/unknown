#include "corrupt.h"

u32 get_corrupt_num()
{
  u32 temp = 0;

  for(u32 i = 0; i < state.columns * state.rows; i++)
  {
    if(state.map[i] == 'C')
    {
      temp++;
    }
  }
  
  return temp;
}

void corrupt_load(veci2 *corrupts, u32 corrupt_num)
{
  if(corrupt_num > 0)
  {
    corrupts = malloc(sizeof(veci2) * corrupt_num);
    ASSERT(corrupts == NULL, "failed to allocate memory for corruptions\n");;
    for(u32 i = 0; i < corrupt_num; i++)
    {
      corrupts[i] = find_position('C', i);
    }
  }
}

void corrupt_update(veci2 *corruption, const veci2 target)
{
  // set current char to blank
  state.map[corruption->y * state.columns + corruption->x] = ' ';

  // find dir to target
  veci2 dif;
  VECi2(dif, abs(target.x - corruption->x), abs(target.y - corruption->y));

  if(dif.x >= dif.y)
  {
    if(target.x > corruption->x)
    {
      corruption->x++;
    }
    else
    {
      corruption->x--;
    }
  }
  else if(dif.x < dif.y)
  {
    if(target.y > corruption->y)
    {
      corruption->y++;
    }
    else
    {
      corruption->y--;
    }
  }

  state.map[corruption->y * state.columns + corruption->x] = 'C';
}
