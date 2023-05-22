#ifndef PLAYER_H
#define PLAYER_H

typedef struct Player {
	float x, y; //Location
	float dirX, dirY; //Forward vector
	float planeX, planeY; //Camera plane (Left/Right vector)
	float rotation; //Heading
} Player;

extern Player player1;

void player_init();
void player_update( Player *player );
#endif
