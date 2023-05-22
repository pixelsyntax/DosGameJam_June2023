#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "global.h"
#include "gfx.h"
#include "raycaster.h"
#include "player.h"
#include "input.h"

#ifdef linux
#include "SDL2/SDL.h"
#define SDL
#endif

#ifdef MSDOS
#include <sys/nearptr.h>
#include <dpmi.h>
#include <bios.h>
#include <dos.h>
#define BIOS_GFX_VIDEOMODE 0x00
#define BIOS_GFX_INT 0x10
#define VIDEOMODE_VGA256 0x13
#define VIDEOMODE_TEXTMODE 0x03
#define VGA_ADDRESS 0xA0000
#define VGA_INPUT_STATUS_1 0x3DA
#define VGA_VSYNC_MASK 0x08
byte* vga; //actual video ram
#else
SDL_Window *window = NULL;
SDL_Renderer *renderer;
SDL_Event event;
SDL_Surface *screen_surface; //Our virtual screen
SDL_Surface *window_surface; //The OS window surface for final output
#endif


byte* screen; //A memory buffer for drawing to

//Wait until we enter vsync
#ifdef MSDOS
void vsync(){
	//Wait until we're not in vsync (hopefully instantly)
	while( inp( VGA_INPUT_STATUS_1 ) & VGA_VSYNC_MASK ){
	}
	//Now wait until vsync
	while( !(inp( VGA_INPUT_STATUS_1 ) & VGA_VSYNC_MASK) ){
	}
}
#else
void vsync(){

}
#endif

void blit_screen_to_video(){
#ifdef MSDOS
	memcpy( vga, screen, SCREEN_WIDTH * SCREEN_HEIGHT );
#else
	SDL_UnlockSurface( screen_surface );

	SDL_BlitScaled( screen_surface, 0, window_surface, 0 );
	SDL_UpdateWindowSurface( window );
	SDL_LockSurface( screen_surface );
#endif
}

//Ask the BIOS to set the given graphics mode
void set_graphics_mode( char mode ){
#ifdef MSDOS
	union REGS regs;
	regs.h.ah = BIOS_GFX_VIDEOMODE;
	regs.h.al = mode;
	int86( BIOS_GFX_INT, &regs, &regs );
#endif
}

#ifdef MSDOS
void msdos_init(){
	//Cool people don't need memory protection B-)
	__djgpp_nearptr_enable();

	//Add djgpp conventional base to find the vga memory location	
	vga = (byte*)(VGA_ADDRESS + __djgpp_conventional_base); 
	
	//Choo choo
	set_graphics_mode( VIDEOMODE_VGA256 );
	
}
void msdos_teardown(){
	set_graphics_mode( VIDEOMODE_TEXTMODE );
}
#else
void sdl_init(){
	SDL_Init( SDL_INIT_EVERYTHING );
	window = SDL_CreateWindow(
		"DIVER",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN
	);
	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_PRESENTVSYNC
	);
	SDL_RenderSetIntegerScale( renderer, 1 );
	
	screen_surface = SDL_CreateRGBSurfaceFrom(
		screen,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		8,
		SCREEN_WIDTH,
		0, 0, 0, 0
	);
	if ( !screen_surface ){
		printf("%s\n", SDL_GetError());
	} else {
		SDL_Palette *palette = screen_surface->format->palette;
		int palette_size = palette->ncolors;
		palette->colors[0].r = 0;
		palette->colors[0].g = 0;
		palette->colors[0].b = 0;
		//Set sometest colours
		for ( int i = 1; i < palette_size; ++i ){
			SDL_Color *colour = &palette->colors[i];
			colour->r = (32*i) % 255;
			colour->g = (32*i+64) % 255;
			colour->b = (16*i+128) % 255;
		}
		printf("Screen surface %d x %d %s\n",
			screen_surface->w,
			screen_surface->h,
			SDL_GetPixelFormatName( screen_surface->format->format )
		);
	}
	window_surface = SDL_GetWindowSurface( window );
	if ( !window_surface ){
	  	printf("%s\n", SDL_GetError());
	} else {
		printf("Window surface %d x %d %s\n", 
			window_surface->w,
			window_surface->h,	
			SDL_GetPixelFormatName( window_surface->format->format )
		);
	}
}
void sdl_teardown(){
	SDL_FreeSurface( screen_surface );
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	SDL_Quit();
}
#endif


int main(){
	input_init();
	//make a buffer for drawing to
	screen = malloc( SCREEN_WIDTH * SCREEN_HEIGHT );
#ifdef MSDOS
	msdos_init();	
#else
	sdl_init();
#endif

	raycaster_init();
	player_init();

	int i = 0;
	int x, y;
	byte isPlaying = 1;
	while( isPlaying ){

#ifdef SDL
		while( SDL_PollEvent( &event ) ){
			switch( event.type ){
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					input_sdl_event( &event );
					break;
				case SDL_WINDOWEVENT:
//					if  ( event.window.event== SDL_WINDOWEVENT_SIZE_CHANGED ){
						window_surface =SDL_GetWindowSurface( window );
//					}
					break;
				case SDL_QUIT:
					isPlaying = 0;
					break;		
			}
		}
#endif
		//Wait for vsync and draw previous frame to screen
		vsync();
		blit_screen_to_video();
		//gfx_clear();

		//Input polling
		input_update();
		//Logic updates
		player_update( &player1 );
		raycaster_update();
	
		//Draw the next frame
		//raycaster_draw_topdown( &player1 );		
		raycaster_draw_firstperson( &player1 );
	}

	//Return to boredom :(
	free( screen );

#ifdef MSDOS
	msdos_teardown();

#endif
	return 0;

}
