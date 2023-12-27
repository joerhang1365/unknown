#include "corruption.h"

void corruption_update(corruption_t *corruption, const veci2 target, char *map, const u32 columns)
{
  // set current char to blank
  map[corruption->pos.y * columns + corruption->pos.x] = ' ';

  // find dir to target
  veci2 dif;
  dif.x = abs(target.x - corruption->pos.x);
  dif.y = abs(target.y - corruption->pos.y);

  if(dif.x >= dif.y)
  {
    if(target.x > corruption->pos.x)
    {
      corruption->pos.x++;
    }
    else
    {
      corruption->pos.x--;
    }
  }
  else if(dif.x < dif.y)
  {
    if(target.y > corruption->pos.y)
    {
      corruption->pos.y++;
    }
    else
    {
      corruption->pos.y--;
    }
  }

  map[corruption->pos.y * columns + corruption->pos.x] = 'C';
}
