#include "input.h"
#include "global.h"
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

//Keyboard handler
_go32_dpmi_seginfo old_isr, keyboard_isr;

byte input_keyboard_state[128];
int input_last_code;

int input[input_count];

void input_keyboard_isr(){
	//Get key event
	int scancode = inportb( PORT_KEYBOARD_BUFFER );
	input_last_code = scancode;
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

void input_init(){
	input_install_keyboard();
	for ( int i = 0; i < input_count; ++i ){
		input[i] = INPUT_INACTIVE;
	}
}

void input_update(){
	input[input_forward] = input_keyboard_state[17];	
}
