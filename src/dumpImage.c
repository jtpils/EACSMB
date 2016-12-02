 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <unistd.h>

#include "dumpImage.h"

#include <png.h>
#include <setjmp.h>


// data is densely packed
int writePNG(char* path, int channels, char* data, int w, int h) {
	
	FILE* f;
	png_byte sig[8];
	png_bytep* rowPtrs;
	int i;
	
	png_structp pngPtr;
	png_infop infoPtr;

	printf("png write | w: %d, h: %d \n", w, h);

	// file stuff
	f = fopen(path, "wb");
	if(!f) {
		fprintf(stderr, "Could not open \"%s\" (writePNG).\n", path);
		return NULL;
	}
	
	/*
	if(png_sig_cmp(sig, 0, 8)) {
		fprintf(stderr, "\"%s\" is not a valid PNG file.\n", path);
		fclose(f);
		return NULL;
	}
	*/
	// init stuff
	pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pngPtr) {
		return NULL;
	}
	//png_destroy_write_struct (&pngPtr, (png_infopp)NULL);
	infoPtr = png_create_info_struct(pngPtr);
	if (!infoPtr) {
		return NULL;
	}
	//if(infoPtr != NULL) png_free_data(pngPtr, infoPtr, PNG_FREE_ALL, -1);
	// header stuff
	if (setjmp(png_jmpbuf(pngPtr))) {
		return NULL;
	}
	png_init_io(pngPtr, f);


	if (setjmp(png_jmpbuf(pngPtr))) {
		return NULL;
	}
	png_set_IHDR(pngPtr, infoPtr, w, h,
		8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	
	png_write_info(pngPtr, infoPtr);

	rowPtrs = malloc(h * sizeof(png_bytep));
	for(i = 0; i < h; i++) {
		rowPtrs[i] = data + (i * w);
	}
	
	// write data
	if (setjmp(png_jmpbuf(pngPtr))) {
		return NULL;
	}
	png_write_image(pngPtr, rowPtrs);

	if (setjmp(png_jmpbuf(pngPtr))) {
		return NULL;
	}
	png_write_end(pngPtr, NULL);
	
	// cleanup
	fclose(f);
	free(rowPtrs);
	
	return 0;
}



