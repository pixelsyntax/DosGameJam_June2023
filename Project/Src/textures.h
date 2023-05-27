#ifndef TEXTURES_H
#define TEXTURES_H

#include "global.h"

#define TEXTURE_SIZE 32
#define TEXTURE_COUNT 256 
typedef struct Texture {
	byte pixels[TEXTURE_SIZE*TEXTURE_SIZE]; //Stored Y,X for vertical strip performance
} Texture;

extern Texture textures[TEXTURE_COUNT];

void texture_load_sheet( const char *path );
Texture *texture_get_by_index( byte index );

#endif
