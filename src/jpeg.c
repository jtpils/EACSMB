#include <stdlib.h>
#include <stdio.h>
#include <string.h>

   

#include "jpeg.h"        



#ifndef HAVE_JPEG
int writeJPEG_RGB(char* path, int width, int height, void* data, int quality) {
	printf("No libJPEG support compiled in.\n");
}
int writeJPEG_RGBA(char* path, int width, int height, void* data, uint32_t alphaColor, int quality) {
	printf("No libJPEG support compiled in.\n");
}
#else



#include <jpeglib.h>


// quality is 1..100
int writeJPEG_RGB(char* path, int width, int height, void* data, int quality) {
	
	struct jpeg_compress_struct ci;
	struct jpeg_error_mgr err;
	FILE* f;
	JSAMPROW* rows; 
	
	f = fopen(path, "wb");
	if(f == NULL) {
		return 1;
	}
	
	
	ci.err = jpeg_std_error(&err);
	jpeg_create_compress(&ci);

	jpeg_stdio_dest(&ci, f);
	
	ci.image_width = width;
	ci.image_height = height;
	ci.input_components = 3;
	ci.in_color_space = JCS_RGB; 
	
	jpeg_set_defaults(&ci);
	
	
	
	jpeg_set_quality(&ci, (quality > 100 ? 100 : (quality < 1 ? 1 : quality)), 1);
	
	jpeg_start_compress(&ci, 1);
	
	rows = malloc(sizeof(*rows) * height);

	// set up the pointers and write the image
	for(int i = 0; i < height; i++) {
		rows[i] = data + (i * (width * 3));
	}

	jpeg_write_scanlines(&ci, rows, height);
	
	
	// finish up
	jpeg_finish_compress(&ci);
	
	fclose(f);
	free(rows);
	
	jpeg_destroy_compress(&ci);
	
	return 0;
}




// quality is 1..100
int writeJPEG_RGBA(char* path, int width, int height, void* data, uint32_t alphaColor, int quality) {
	
	struct jpeg_compress_struct ci;
	struct jpeg_error_mgr err;
	FILE* f;
	JSAMPROW row;
	float a_r, a_b, a_g;
	
	f = fopen(path, "wb");
	if(f == NULL) {
		return 1;
	}
	
	ci.err = jpeg_std_error(&err);
	jpeg_create_compress(&ci);

	jpeg_stdio_dest(&ci, f);
	
	ci.image_width = width;
	ci.image_height = height;
	ci.input_components = 3;
	ci.in_color_space = JCS_RGB; 
	
	jpeg_set_defaults(&ci);
	
	
	
	jpeg_set_quality(&ci, (quality > 100 ? 100 : (quality < 1 ? 1 : quality)), 1);
	
	jpeg_start_compress(&ci, 1);

	row = malloc(width * 3 * sizeof(*row));
	a_r = (alphaColor & 0xff000000) >> 24;
	a_g = (alphaColor & 0x00ff0000) >> 16;
	a_b = (alphaColor & 0x0000ff00) >> 8;
	
	// convert and write one row at a time
	for(int i = 0; i < height; i++) {
		
		// blend in the alpha color
		for(int x = 0; x < width; x++) {
			union {
				uint8_t b[4];
				uint32_t n;
			} u;
			u.n = ((uint32_t*)data)[i * width + x];
			
			if(u.b[3] == 255) {
				row[(x * 3) + 0] = u.b[0];
				row[(x * 3) + 1] = u.b[1];
				row[(x * 3) + 2] = u.b[2];
			}
			else {
				float a = (float)u.b[3] / 256.0;
				float oma = 1.0 - a;
				
				row[(x * 3) + 0] = (float)u.b[0] * a + a_r * oma; 
				row[(x * 3) + 1] = (float)u.b[1] * a + a_g * oma; 
				row[(x * 3) + 2] = (float)u.b[2] * a + a_b * oma; 
			}
		}
		
		jpeg_write_scanlines(&ci, row, 1);
	}

	
	
	// finish up
	jpeg_finish_compress(&ci);
	
	fclose(f);
	free(row);
	
	jpeg_destroy_compress(&ci);
	
	return 0;
}




#endif


