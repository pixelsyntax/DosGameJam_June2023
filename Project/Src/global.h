#ifndef GLOBAL_H
#define GLOBAL_H

#define CHAR_BIT 8
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define SCREEN_LENGTH ( SCREEN_WIDTH * SCREEN_HEIGHT )
#define LOCK_VARIABLE(x)    _go32_dpmi_lock_data((void *)&x,(long)sizeof(x));
#define LOCK_FUNCTION(x)    _go32_dpmi_lock_code(x,(long)sizeof(x));

typedef unsigned char byte;
typedef unsigned int uint;

typedef struct Colour {
	byte b,g,r,a;
} Colour;

extern byte* screen;

static inline int int_abs( int a ){
	const int mask = a >> sizeof( int ) * CHAR_BIT - 1;
	return ( a + mask ) ^ mask;
}
#endif
