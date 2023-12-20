#include "corruption.h"

void corruption_update(corruption_t *corruption, veci2 target, char *map, i32 columns)
{
  // set current char to blank
  map[corruption->pos.y * columns + corruption->pos.x] = ' ';
  // find dir to target
  if(target.x < corruption->pos.x)
  {
    corruption->pos.x--;
  }
  else if(target.x > corruption->pos.x)
  {
    corruption->pos.x++;
  }
  else if(target.y < corruption->pos.y)
  {
    corruption->pos.y--;
  }
  else if(target.y > corruption->pos.y)
  {
    corruption->pos.y++;
  }

  map[corruption->pos.y * columns + corruption->pos.x] = 'C';
}
