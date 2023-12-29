#include "corruption.h"
#include "state.h"

void corruption_load(veci2 *corruptions, u32 *corruption_number)
{
  // get number of corruptions
  for(u32 i = 0; i < state.columns * state.rows; i++)
  {
    if(state.map[i] == 'C')
    {
      (*corruption_number)++;
    }
  }

  if(*corruption_number > 0)
  {
    corruptions = malloc(sizeof(veci2) * (*corruption_number));
    ASSERT(corruptions == NULL, "failed to allocate memory for corruptions\n");

    for(u32 i = 0; i < *corruption_number; i++)
    {
      corruptions[i] = find_position('C', i);
    }
  }
}

void corruption_update(veci2 *corruption, const veci2 target, char *map, const u32 columns)
{
  // set current char to blank
  map[corruption->y * columns + corruption->x] = ' ';

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

  map[corruption->y * columns + corruption->x] = 'C';
}
