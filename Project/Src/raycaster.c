#include "raycaster.h"
#include "gfx.h"
#include "input.h"
#include <bios.h>
#include <math.h>
#define mapWidth 24
#define mapHeight 24


typedef struct Camera {
	float x, y;
	float dirX, dirY;
	float planeX, planeY;
	float rotation;
} Camera;

Camera camera;
float raycaster_topdown_scale = 3;

int worldMap[mapWidth][mapHeight] =
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

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

void raycaster_draw_camera_topdown(){
	//Draw camera plane
	float camera_plane_size = 5;
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
	raycaster_draw_camera_topdown();	
	

}

void raycaster_update(){

	float walk_speed = 0.5;
	float rotation_speed = 0.05;

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
