#include <string.h>
#include "gfx.h"
#ifdef MSDOS
#include <dos.h>
#include <bios.h>
#include <dpmi.h>
#include <go32.h>

#define PORT_PALETTE_MASK  0x03C6
#define PORT_PALETTE_WRITE 0x03C8
#define PORT_PALETTE_DATA  0x03C9

#endif

void gfx_clear(){
	memset( screen, 0, SCREEN_WIDTH*SCREEN_HEIGHT );
}

void gfx_rect( uint x, uint y, uint w, uint h, byte colour ){
	
	gfx_line( x, y, x+w, y, colour );
	gfx_line( x+w, y, x+w, y+h, colour );
	gfx_line( x+w, y+h, x, y+h, colour );
	gfx_line( x, y+h, x, y, colour );

}

//Draw a line downward from x,y for height h
void gfx_vline( uint x, uint y, uint h, const byte colour ){

	if ( x < 0 || x >= SCREEN_WIDTH ) return;
	if ( y < 0 ) y = 0;
	if ( y + h >= SCREEN_HEIGHT ) h = SCREEN_HEIGHT - y;
	uint i = y*SCREEN_WIDTH+x;
	for ( int t = 0; t < h; ++t ){
		screen[i] = colour;
		i += SCREEN_WIDTH;
	}
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

//Set 256 colours in the palette
void gfx_set_indexed_palette( Colour *colours ){
#ifdef MSDOS
	Colour *colour;
	for ( int i = 0; i <= 256; ++i ){
		colour = &colours[i]; 
		outportb( PORT_PALETTE_MASK, 0xFF );
		outportb( PORT_PALETTE_WRITE, (byte)i );		
		outportb( PORT_PALETTE_DATA, colour->r >> 2 );
		outportb( PORT_PALETTE_DATA, colour->g >> 2 );
		outportb( PORT_PALETTE_DATA, colour->b >> 2 );
	}
	
#endif
}
