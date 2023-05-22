#include <math.h>
#include "player.h"
#include "input.h"
#include "world.h"

Player player1;

void player_recalculate_camera( Player* player ){
	player->dirX = cos( player->rotation );
	player->dirY = sin( player->rotation );
	player->planeX = cos( player->rotation + M_PI_2 );
	player->planeY = sin( player->rotation + M_PI_2 );
}

void player_update( Player* player ){
	float walk_speed = 0.25;
	float rotation_speed = 0.1;

	float new_x = player->x;
	float new_y = player->y;
	if ( inputs[input_forward] ){
		new_x += player->dirX * walk_speed;
		new_y += player->dirY * walk_speed;
	} else if ( inputs[input_backward] ){
		new_x -= player->dirX * walk_speed;
		new_y -= player->dirY * walk_speed;
	}
	if ( inputs[input_left] ) {
		new_x -= player->planeX * walk_speed;
		new_y -= player->planeY * walk_speed;
	} else if ( inputs[input_right] ) {
		new_x += player->planeX * walk_speed;
		new_y += player->planeY * walk_speed;
	}
	if ( inputs[input_rotate_left] ){
		player->rotation -= rotation_speed;
	}
	if ( inputs[input_rotate_right] ){
		player->rotation += rotation_speed;
	}

	if ( !worldMap[(int)player->y][(int)new_x] )	
		player->x = new_x;
	if ( !worldMap[(int)new_y][(int)player->x] )
		player->y = new_y;
	player_recalculate_camera( player );
}

void player_init(){
	player1.x = 22;
       	player1.y = 12;
	player1.dirX = -1;
	player1.dirY = 0;
	player1.planeX = 0;
	player1.planeY = 1;
}
