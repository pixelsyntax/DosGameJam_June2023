#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "player.h"

void raycaster_init();
void raycaster_draw_topdown( Player *player );
void raycaster_draw_firstperson( Player *player );
void raycaster_update();

#endif
