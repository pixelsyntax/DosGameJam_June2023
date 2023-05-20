#ifndef GLOBAL_H
#define GLOBAL_H

#define CHAR_BIT 8
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

typedef unsigned char byte;
typedef unsigned int uint;


extern byte* screen;

static inline int int_abs( int a ){
	const int mask = a >> sizeof( int ) * CHAR_BIT - 1;
	return ( a + mask ) ^ mask;
}
#endif
