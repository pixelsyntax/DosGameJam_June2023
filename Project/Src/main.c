#include <stdio.h>
#include <conio.h> //for testing with getch
#include <sys/nearptr.h>
#include <dpmi.h>
#include <bios.h>
#include <dos.h>
#include <malloc.h>
#include <string.h>
#include "global.h"
#include "gfx.h"
#include "raycaster.h"
#include "input.h"

#define BIOS_GFX_VIDEOMODE 0x00
#define BIOS_GFX_INT 0x10
#define VIDEOMODE_VGA256 0x13
#define VIDEOMODE_TEXTMODE 0x03
#define VGA_ADDRESS 0xA0000
#define VGA_INPUT_STATUS_1 0x3DA
#define VGA_VSYNC_MASK 0x08

byte* screen; //A memory buffer for drawing to
byte* vga; //actual video ram

//Wait until we enter vsync
void vsync(){
	//Wait until we're not in vsync (hopefully instantly)
	while( inp( VGA_INPUT_STATUS_1 ) & VGA_VSYNC_MASK ){
	}
	//Now wait until vsync
	while( !(inp( VGA_INPUT_STATUS_1 ) & VGA_VSYNC_MASK) ){
	}
}

void blit_screen_to_video(){
	memcpy( vga, screen, SCREEN_WIDTH * SCREEN_HEIGHT );
}

//Ask the BIOS to set the given graphics mode
void set_graphics_mode( char mode ){
	union REGS regs;
	regs.h.ah = BIOS_GFX_VIDEOMODE;
	regs.h.al = mode;
	int86( BIOS_GFX_INT, &regs, &regs );
}

int main(){
	input_init();
	//Cool people don't need memory protection B-)
	__djgpp_nearptr_enable();

	//Add djgpp conventional base to find the vga memory location	
	vga = (byte*)(VGA_ADDRESS + __djgpp_conventional_base); 
	//make a buffer for drawing to
	screen = malloc( SCREEN_WIDTH * SCREEN_HEIGHT );

	//Go VGA mode 0x13, yeah!
	set_graphics_mode( VIDEOMODE_VGA256 );

	raycaster_init();
	//input_init();

	int i = 0;
	int x, y;
	while( 1 ){
		//Wait for vsync and draw previous frame to screen
		vsync();
		blit_screen_to_video();
		gfx_clear();

		//Input polling
		input_update();
		//Logic updates
		raycaster_update();
	
		//Draw the next frame
		//raycaster_draw_topdown();		
		raycaster_draw_firstperson();
	}

	//Return to boredom :(
	free( screen );
	set_graphics_mode( VIDEOMODE_TEXTMODE );
	return 0;

}
