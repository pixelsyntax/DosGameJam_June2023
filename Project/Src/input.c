#include "input.h"
#include <stdio.h>
#include "global.h"
#ifdef MSDOS
#include <dos.h>
#include <bios.h>
#include <dpmi.h>
#include <go32.h>
#define BIOS_KEYBOARD_SERVICES 	0x16
#define BIOS_KEYBOARD_STATUS 	0x02
#define KEYBOARD_VECTOR 	0x09
#define PORT_KEYBOARD_BUFFER 	0x60
#define PORT_KEYBOARD_CONTROL	0x61
#define PORT_PIC 		0x20
#define KEYCODE_W 		17
#define KEYCODE_A 		30
#define KEYCODE_S 		31
#define KEYCODE_D 		32
#define KEYCODE_UP 		72
#define KEYCODE_LEFT 		75
#define KEYCODE_RIGHT 		77
#define KEYCODE_DOWN 		80
#define KEYCODE_Q		16
#define KEYCODE_E 		18
#else
#define KEYCODE_W 		SDL_SCANCODE_W
#define KEYCODE_A 		SDL_SCANCODE_A
#define KEYCODE_S 		SDL_SCANCODE_S
#define KEYCODE_D 		SDL_SCANCODE_D
#define KEYCODE_UP 		SDL_SCANCODE_UP
#define KEYCODE_LEFT 		SDL_SCANCODE_LEFT
#define KEYCODE_RIGHT 		SDL_SCANCODE_RIGHT
#define KEYCODE_DOWN 		SDL_SCANCODE_DOWN
#define KEYCODE_Q		SDL_SCANCODE_Q
#define KEYCODE_E 		SDL_SCANCODE_E
#endif
#ifdef MSDOS
//DOS Keyboard handler
_go32_dpmi_seginfo old_isr, keyboard_isr;
#endif

byte input_keyboard_state[128];

int inputs[input_count];

#ifdef MSDOS
void input_keyboard_isr(){
	//Get key event
	int scancode = inportb( PORT_KEYBOARD_BUFFER );
	scancode &= 0b0000000011111111;
	
	if ( scancode < 128 )
		input_keyboard_state[scancode] = 1;
	else
		input_keyboard_state[scancode-128] = 0;

	//Acknowledge interrupt
	int control_keyboard = inportb( PORT_KEYBOARD_CONTROL );
	outportb( PORT_KEYBOARD_CONTROL, control_keyboard | 0x80 );
	outportb( PORT_KEYBOARD_CONTROL, control_keyboard );
	//End of interrupt command
	outportb( PORT_PIC, inportb( PORT_PIC ) );
}

//Install a keyboard handler
void input_install_keyboard(){

	LOCK_FUNCTION( input_keyboard_isr );
	LOCK_VARIABLE( input_keyboard_state );
	//Stash old vector
	_go32_dpmi_get_protected_mode_interrupt_vector( KEYBOARD_VECTOR, &old_isr ); 
	keyboard_isr.pm_offset = (int)input_keyboard_isr;
	keyboard_isr.pm_selector = _go32_my_cs();
	_go32_dpmi_chain_protected_mode_interrupt_vector(
		KEYBOARD_VECTOR,
		&keyboard_isr
	);
}

void input_uninstall_keyboard(){
	_go32_dpmi_set_protected_mode_interrupt_vector(
		KEYBOARD_VECTOR, 
		&old_isr
	);
}
#endif

void input_init(){
#ifdef MSDOS
	input_install_keyboard();
#endif
	for ( int i = 0; i < input_count; ++i ){
		inputs[i] = INPUT_INACTIVE;
	}
}

//Update an individual input 
void input_update_input( INPUTS input, byte isActive ){
	int value = inputs[input];
	//Increment active inputs or set them to deactivating
	if ( value >= INPUT_ACTIVE ){
		inputs[input] = isActive ? value + 1 : INPUT_DEACTIVATING;
	} else { //Set a newly active input to active or leave inactive input as inactive
		inputs[input] = isActive ? INPUT_ACTIVE : INPUT_INACTIVE;
	}
}

//Update the inputs array
void input_update(){
	input_update_input( input_forward, input_keyboard_state[KEYCODE_W] | input_keyboard_state[KEYCODE_UP] );
	input_update_input( input_backward, input_keyboard_state[KEYCODE_S] | input_keyboard_state[KEYCODE_DOWN] );
	input_update_input( input_left, input_keyboard_state[KEYCODE_A] | input_keyboard_state[KEYCODE_LEFT] );
	input_update_input( input_right, input_keyboard_state[KEYCODE_D] | input_keyboard_state[KEYCODE_RIGHT] );
	input_update_input( input_rotate_left, input_keyboard_state[KEYCODE_Q] );
	input_update_input( input_rotate_right, input_keyboard_state[KEYCODE_E] );
}

#ifndef MSDOS
void input_sdl_event( SDL_Event *event ){
	int scancode;
	switch( event->type ){
		case SDL_KEYDOWN:
			scancode = event->key.keysym.scancode;
			if ( scancode < 128 )
				input_keyboard_state[scancode] = 1;		
			break;

		case SDL_KEYUP:
			scancode = event->key.keysym.scancode;
			if ( scancode < 128 )
				input_keyboard_state[scancode] = 0;		
			break;
	}
}
#endif
