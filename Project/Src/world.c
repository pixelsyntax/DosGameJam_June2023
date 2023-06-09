#include "world.h"

byte worldMap[WORLD_HEIGHT][WORLD_WIDTH] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,3,3,3,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,2,0,2,0,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,0,4,4,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,4,0,0,4,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,4,0,4,0,4,4,0,4,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,4,0,0,0,0,4,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,4,0,4,4,4,0,4,4,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,4,0,4,0,4,0,4,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,4,0,4,0,4,0,4,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,4,0,4,0,4,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,4,0,0,0,4,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

