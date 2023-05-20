#include <stdio.h>
#include <conio.h> //for testing with getch
#include <sys/nearptr.h>
#include <dpmi.h>
#include <bios.h>
#include "global.h"
#include "gfx.h"

#define BIOS_GFX_VIDEOMODE 0x00
#define BIOS_GFX_INT 0x10
#define VIDEOMODE_VGA256 0x13
#define VIDEOMODE_TEXTMODE 0x03
#define VGA_ADDRESS 0xA0000
byte* screen;

//Ask the BIOS to set the given graphics mode
void set_graphics_mode( char mode ){
	union REGS regs;
	regs.h.ah = BIOS_GFX_VIDEOMODE;
	regs.h.al = mode;
	int86( BIOS_GFX_INT, &regs, &regs );
}

int main(){
	//Cool people don't need memory protection B-)
	__djgpp_nearptr_enable();

	//Add djgpp conventional base to find the vga memory location
	screen = (byte*)(VGA_ADDRESS + __djgpp_conventional_base); 

	//Go VGA mode 0x13, yeah!
	set_graphics_mode( VIDEOMODE_VGA256 );

	//Draw some tests pixels
	for ( int i = 0; i < 200; ++i ){
		gfx_line( 300,100,200,i,i%20 );
	}
	getch();	

	//Return to boredom :(
	set_graphics_mode( VIDEOMODE_TEXTMODE );
	return 0;

}
