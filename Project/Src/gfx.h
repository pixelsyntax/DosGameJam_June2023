#ifndef GFX_H
#define GFX_H

#include "global.h"

void gfx_clear();
void gfx_set_indexed_palette( Colour *colours );
void gfx_vline( uint x, uint y, uint h, byte colour );
void gfx_line( uint x1, uint y1, uint x2, uint y2, byte colour ); 
void gfx_rect( uint x, uint y, uint w, uint h, byte colour );
#endif
