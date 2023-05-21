#include "raycaster.h"
#include "gfx.h"
#include "input.h"
#include <bios.h>
#include <math.h>
#define mapWidth 24
#define mapHeight 24

typedef struct RaycastHit {
	float hitX, hitY;
	byte colour;
} RaycastHit;

typedef struct Camera {
	float x, y;
	float dirX, dirY;
	float planeX, planeY;
	float rotation;
} Camera;

Camera camera;
float raycaster_topdown_scale = 8;

byte worldMap[mapHeight][mapWidth] =
{
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

void raycaster_cast(){
	for ( int x = SCREEN_WIDTH/2; x <= SCREEN_WIDTH/2; ++x ){
		int x = SCREEN_WIDTH/2;
		float cameraX = 2 * x / (float)SCREEN_WIDTH - 1;
		float rayDirX = camera.dirX + camera.planeX * cameraX;
		float rayDirY = camera.dirY + camera.planeY * cameraX;
		int mapX = (int)camera.x;
		int mapY = (int)camera.y;
		float sideDistX, sideDistY;
		float deltaDistX = rayDirX ? fabs( 1 / rayDirX ) : INFINITY;
		float deltaDistY = rayDirY ? fabs( 1 / rayDirY ) : INFINITY;
		float perpWallDist;

		int stepX, stepY;
		int hit = 0;
		int side = 0;

		if ( rayDirX < 0 ){
			stepX = -1;
			sideDistX = ( camera.x -  mapX ) * deltaDistX;
		} else {
			stepX = 1;
			sideDistX = ( mapX + 1.0 - camera.x ) * deltaDistX;
		}
		if ( rayDirY < 0 ){
			stepY = -1;
			sideDistY =  ( camera.y - mapY ) * deltaDistY;
		} else {
			stepY = 1;
			sideDistY =  ( mapY + 1.0 - camera.y ) * deltaDistY;
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
			if ( mapX < 0 || mapX >= mapWidth || mapY < 0 || mapY >= mapHeight ){
				hit = 1;
			} else if ( worldMap[mapY][mapX] ) {
				hit = 1;
			}
		}
		if ( side == 0 ) perpWallDist = ( sideDistX - deltaDistX );
		else perpWallDist = ( sideDistY - deltaDistY );

		//Draw debug line
		float endX, endY;
		endX = camera.dirX * perpWallDist + camera.x;
		endY = camera.dirY * perpWallDist + camera.y;
		gfx_line( 
			camera.x * raycaster_topdown_scale, 
			camera.y * raycaster_topdown_scale,
			endX * raycaster_topdown_scale, 
			endY * raycaster_topdown_scale, 
			14 
		);	
	}

}

void raycaster_init(){
	camera.x = 22;
	camera.y = 12;
	camera.dirX = -1;
	camera.dirY = 0;
	camera.planeX = 0;
	camera.planeY = 1;
}

void raycaster_calculate_camera(){
	camera.dirX = cos( camera.rotation );
	camera.dirY = sin( camera.rotation );
	camera.planeX = cos( camera.rotation + M_PI_2 );
	camera.planeY = sin( camera.rotation + M_PI_2 );
}

void raycaster_draw_map_topdown(){
	for ( int ty = 0; ty < mapHeight; ++ty ){
		for ( int tx = 0; tx < mapWidth; ++tx ){
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

void raycaster_draw_camera_topdown(){
	//Draw camera plane
	float camera_plane_size = 1;
	gfx_line( 
		( camera.x - camera.planeX * camera_plane_size ) * raycaster_topdown_scale,
		( camera.y - camera.planeY * camera_plane_size ) * raycaster_topdown_scale,
		( camera.x + camera.planeX * camera_plane_size ) * raycaster_topdown_scale,
		( camera.y + camera.planeY * camera_plane_size ) * raycaster_topdown_scale,
		2
	);
	//Draw forward vector
	gfx_line(
		( camera.x + camera.dirX * camera_plane_size ) * raycaster_topdown_scale,
		( camera.y + camera.dirY * camera_plane_size ) * raycaster_topdown_scale,
		camera.x * raycaster_topdown_scale, 
		camera.y * raycaster_topdown_scale,
		3
	);	
}

void raycaster_draw_topdown(){
	raycaster_draw_map_topdown();
	raycaster_draw_camera_topdown();	
	raycaster_cast();
}

void raycaster_update(){

	float walk_speed = 0.25;
	float rotation_speed = 0.1;

	raycaster_calculate_camera();
	//Read input and update camera position	
	if ( inputs[input_forward] ){
		camera.x += camera.dirX * walk_speed;
		camera.y += camera.dirY * walk_speed;
	} 
	if ( inputs[input_backward] ){
		camera.x -= camera.dirX * walk_speed;
		camera.y -= camera.dirY * walk_speed;
	}
	if ( inputs[input_left] ){
		camera.x -= camera.planeX * walk_speed;
		camera.y -= camera.planeY * walk_speed;
	}
	if ( inputs[input_right] ){
		camera.x += camera.planeX * walk_speed;
		camera.y += camera.planeY * walk_speed;
	}
	if ( inputs[input_rotate_left] ){
		camera.rotation -= rotation_speed;
	} 
	if ( inputs[input_rotate_right] ){
		camera.rotation += rotation_speed;
	}

}
