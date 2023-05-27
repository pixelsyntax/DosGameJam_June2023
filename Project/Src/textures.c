#include <stdio.h>
#include "gfx.h"
#include "textures.h"
Texture textures[TEXTURE_COUNT];

struct bmp_header {
   unsigned short int type;                //2 /* Magic identifier            */
   unsigned int size;                      //4 /* File size in bytes          */
   unsigned short int reserved1; 	   //2
   unsigned short int reserved2;  	   //2
   unsigned int offset;                    //4 /* Offset to image data, bytes */
};

struct bmp_info {
	unsigned int size;
	int width, height;
	unsigned short int planes;
	unsigned short int bits;
	unsigned int compression;
	unsigned int imagesize;
	int xres, yres;
	unsigned int ncolours;
	unsigned int importantcolours;
};

//Load texture sheet, expecting a 512*512 256 colour indexed bmp only
void texture_load_sheet( const char *path ){
	FILE *file = fopen( path, "rb" );
	if ( !file ){
		printf( "Error: No file %s\n", path );
		return;
	}
	struct bmp_header header;
	fread( &header.type, sizeof( unsigned short int ), 1, file );
	fread( &header.size, sizeof( int ), 1, file );
	fread( &header.reserved1, sizeof( unsigned short int ), 1, file );
	fread( &header.reserved2, sizeof( unsigned short int ), 1, file );
	fread( &header.offset, sizeof( unsigned int ), 1, file );
	printf("Header supplied file size: %d, data offset: %d\n", header.size, header.offset);	
	struct bmp_info info;
	fread( &info.size, sizeof( unsigned int ), 1, file );
	fread( &info.width, sizeof( int ), 1, file );
	fread( &info.height, sizeof( int ), 1, file );
	fread( &info.planes, sizeof( unsigned short int ), 1, file );
	fread( &info.bits, sizeof( unsigned short int ), 1, file );
	fread( &info.compression, sizeof( int ), 1, file );
	fread( &info.imagesize, sizeof( int ), 1, file );
	fread( &info.xres, sizeof( int ), 1, file );
	fread( &info.yres, sizeof( int ), 1, file );
	fread( &info.ncolours, sizeof( unsigned int ), 1, file );
	fread( &info.importantcolours, sizeof( unsigned int ), 1, file );

	printf("Info supplied header size: %u, dimensions %d %d (%u) with %u colours\n",
		info.size, info.width, info.height, info.imagesize, info.ncolours );
	printf("Compression type is %u\n", info.compression );
	getch();
	
	Colour colours[256];
	//Read 256 colours
	fread( &colours, 4, 256, file ); 

	//Write the palette to VGA 
	gfx_set_indexed_palette( colours );

	//Load texture sheet
	byte pixel_buffer[512*512];
	fseek( file, header.offset, SEEK_SET );  
	fread( &pixel_buffer, 512, 512, file );
	fclose( file );
	for ( int py = 0; py < 512; ++py ){
		for ( int px = 0; px < 512; ++px ){
			int texture_index = ( py / 32 ) * 16 + ( px / 32 );
			//Stored Y,X
			int dest_pixel_index = (px % TEXTURE_SIZE)*TEXTURE_SIZE + 
						py % TEXTURE_SIZE;
			int src_pixel_index = (512-py) * 512 + px; //BMP is bottom up
			textures[texture_index].pixels[dest_pixel_index] = 
				pixel_buffer[src_pixel_index];
			
		}
	}

}	
