#include <math.h>
#include "raycaster.h"
#include "gfx.h"
#include "input.h"
#include "world.h"
#include "player.h"

typedef struct RaycastHit {
	float hitX, hitY;
	byte colour;
} RaycastHit;

float raycaster_topdown_scale = 8;

void raycaster_cast( Player *player ){
	float cameraX = -1;
	float cameraXStep = 2 / (float)SCREEN_WIDTH;
	
	for ( int x = 0; x <= SCREEN_WIDTH; x += 1 ){
		float rayDirX = player->dirX + player->planeX * cameraX;
		float rayDirY = player->dirY + player->planeY * cameraX;
		int mapX = (int)player->x;
		int mapY = (int)player->y;
		float sideDistX, sideDistY;
		float deltaDistX = rayDirX ? fabs( 1 / rayDirX ) : INFINITY;
		float deltaDistY = rayDirY ? fabs( 1 / rayDirY ) : INFINITY;
		float perpWallDist;

		int stepX, stepY;
		int hit = 0;
		int side = 0;

		if ( rayDirX < 0 ){
			stepX = -1;
			sideDistX = ( player->x -  mapX ) * deltaDistX;
		} else {
			stepX = 1;
			sideDistX = ( mapX + 1.0 - player->x ) * deltaDistX;
		}
		if ( rayDirY < 0 ){
			stepY = -1;
			sideDistY =  ( player->y - mapY ) * deltaDistY;
		} else {
			stepY = 1;
			sideDistY =  ( mapY + 1.0 - player->y ) * deltaDistY;
		}

		//Perform DDA
		while ( !hit ){
			//Jump to next square
			if ( sideDistX < sideDistY ){
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			} else {
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			//Restrict to map
			if ( mapX < 0 || mapX >= WORLD_WIDTH || mapY < 0 || mapY >= WORLD_HEIGHT ){
				hit = 1;
			} else if ( worldMap[mapY][mapX] ) {
				hit = worldMap[mapY][mapX];
			}
		}
		if ( side == 0 ) perpWallDist = ( sideDistX - deltaDistX );
		else perpWallDist = ( sideDistY - deltaDistY );

		int lineHeight = (int)(SCREEN_HEIGHT/perpWallDist);
		int lineStart = -lineHeight / 2  + SCREEN_HEIGHT/2;
		int lineEnd = lineStart + lineHeight;
		if ( lineStart < 0 ) lineStart = 0;
		if ( lineEnd >= SCREEN_HEIGHT )  lineEnd = SCREEN_HEIGHT - 1;

		//Draw
		gfx_vline( x, 0, lineStart, 0 ); //Ceiling
		gfx_vline( x, lineStart, lineEnd-lineStart, hit );
		gfx_vline( x, lineEnd, SCREEN_HEIGHT-1, 0 ); //Floor

		//Prepare for next cast
		cameraX += cameraXStep;
	}

}
void raycaster_cast_topdown( Player *player ){
	for ( int x = 0; x <= SCREEN_WIDTH; x += SCREEN_WIDTH/2 ){
		float cameraX = 2 * ( x / (float)SCREEN_WIDTH ) - 1;
		float rayDirX = player->dirX + player->planeX * cameraX;
		float rayDirY = player->dirY + player->planeY * cameraX;
		int mapX = (int)player->x;
		int mapY = (int)player->y;
		float sideDistX, sideDistY;
		float deltaDistX = rayDirX ? fabs( 1 / rayDirX ) : INFINITY;
		float deltaDistY = rayDirY ? fabs( 1 / rayDirY ) : INFINITY;
		float perpWallDist;

		int stepX, stepY;
		int hit = 0;
		int side = 0;

		if ( rayDirX < 0 ){
			stepX = -1;
			sideDistX = ( player->x -  mapX ) * deltaDistX;
		} else {
			stepX = 1;
			sideDistX = ( mapX + 1.0 - player->x ) * deltaDistX;
		}
		if ( rayDirY < 0 ){
			stepY = -1;
			sideDistY =  ( player->y - mapY ) * deltaDistY;
		} else {
			stepY = 1;
			sideDistY =  ( mapY + 1.0 - player->y ) * deltaDistY;
		}

		//Perform DDA
		while ( !hit ){
			//Jump to next square
			if ( sideDistX < sideDistY ){
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			} else {
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			//Restrict to world
			if ( mapX < 0 || mapX >= WORLD_WIDTH || mapY < 0 || mapY >= WORLD_HEIGHT ){
				hit = 1;
			} else if ( worldMap[mapY][mapX] ) {
				hit = 1;
			}
		}
		if ( side == 0 ) perpWallDist = ( sideDistX - deltaDistX );
		else perpWallDist = ( sideDistY - deltaDistY );

		//Draw debug line
		float endX, endY;
		endX = rayDirX * perpWallDist + player->x;
		endY = rayDirY * perpWallDist + player->y;
		gfx_line( 
			player->x * raycaster_topdown_scale, 
			player->y * raycaster_topdown_scale,
			endX * raycaster_topdown_scale, 
			endY * raycaster_topdown_scale, 
			14 
		);	
	}

}

void raycaster_init(){
}

void raycaster_draw_map_topdown(){
	for ( int ty = 0; ty < WORLD_HEIGHT; ++ty ){
		for ( int tx = 0; tx < WORLD_WIDTH; ++tx ){
			byte colour = worldMap[ty][tx];
		      	if ( colour ){
				gfx_rect(
					tx*raycaster_topdown_scale,
					ty*raycaster_topdown_scale,
					raycaster_topdown_scale-1,
					raycaster_topdown_scale-1,
					colour
				);
			}	
		}
	}
}

void raycaster_draw_camera_topdown( Player* player ){
	//Draw camera plane
	float camera_plane_size = 1;
	gfx_line( 
		( player->x - player->planeX * camera_plane_size ) * raycaster_topdown_scale,
		( player->y - player->planeY * camera_plane_size ) * raycaster_topdown_scale,
		( player->x + player->planeX * camera_plane_size ) * raycaster_topdown_scale,
		( player->y + player->planeY * camera_plane_size ) * raycaster_topdown_scale,
		2
	);
	//Draw forward vector
	gfx_line(
		( player->x + player->dirX * camera_plane_size ) * raycaster_topdown_scale,
		( player->y + player->dirY * camera_plane_size ) * raycaster_topdown_scale,
		player->x * raycaster_topdown_scale, 
		player->y * raycaster_topdown_scale,
		3
	);	
}

void raycaster_draw_topdown( Player *player){
	raycaster_draw_map_topdown();
	raycaster_draw_camera_topdown( player );	
	raycaster_cast_topdown( player );
}

void raycaster_draw_firstperson( Player *player ){
	raycaster_cast( player );
}

void raycaster_update(){


}
