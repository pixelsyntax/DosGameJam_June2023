#include <string.h>
#include "gfx.h"

void gfx_clear(){
	memset( screen, 0, SCREEN_WIDTH*SCREEN_HEIGHT );
}

void gfx_rect( uint x, uint y, uint w, uint h, byte colour ){
	
	gfx_line( x, y, x+w, y, colour );
	gfx_line( x+w, y, x+w, y+h, colour );
	gfx_line( x+w, y+h, x, y+h, colour );
	gfx_line( x, y+h, x, y, colour );

}

void gfx_line( uint x1, uint y1, uint x2, uint y2, byte colour ){

	const int dx = x2 - x1;
	const int dy = y2 - y1;

	const int xstep = dx > 0 ? 1 : -1;
	const int ystep = dy > 0 ? 1 : -1;

	const uint absdx = int_abs( dx );
	const uint absdy = int_abs( dy );

	uint x = x1;
	uint y = y1;
	screen[(y*SCREEN_WIDTH+x)%SCREEN_LENGTH] = colour;

	//Slope < 1
	if ( absdx > absdy ){
		int d = 2*absdy - absdx;
		for ( int i = 0; i < absdx; ++i ){
			x += xstep;
			if ( d < 0 ){
				d += 2*absdy;
			} else {
				y += ystep;
				d += 2*absdy - 2*absdx;
			}
			screen[(y*SCREEN_WIDTH+x)%SCREEN_LENGTH] = colour;
		}
	//Slope >= 1
	} else { 
		int d = 2*absdx - absdy;
	        for(int i = 0; i < absdy; ++i){
			y += ystep;
			if( d < 0 ){
				d += 2*absdx;
			} else {
				x += xstep;
				d += 2*absdx - 2*absdy;
			}
			screen[(y*SCREEN_WIDTH+x)%SCREEN_LENGTH] = colour;
		}
	}
}
