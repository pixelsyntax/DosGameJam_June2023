#ifndef INPUT_H
#define INPUT_H

#define INPUT_DEACTIVATING -1
#define INPUT_INACTIVE 0
#define INPUT_ACTIVE 1

//Game inputs
typedef enum INPUTS {
	input_forward,
	input_backward,
	input_rotate_left,
	input_rotate_right,
	input_left,
	input_right,
	input_count
} INPUTS;

extern int input[input_count];

void input_init();   //Initial setup
void input_update(); //Poll inputs

#endif