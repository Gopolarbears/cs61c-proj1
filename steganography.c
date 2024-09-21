/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	int width = image->cols;
	Color* color = *(image->image) + row * width + col;
	if (color->B % 2 == 0) {
		Color* newColor = malloc(sizeof(Color));
		newColor->R = 0;
		newColor->G = 0;
		newColor->B = 0;
		return newColor;
	} else {
		Color* newColor = malloc(sizeof(Color));
		newColor->R = 255;
		newColor->G = 255;
		newColor->B = 255;
		return newColor;
	}
	
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	Image* newImage = malloc(sizeof(Image));
	newImage->cols = image->cols;
	newImage->rows = image->rows;
	Color* colors = malloc(image->cols * image->rows * sizeof(Color));
	Color** ptr = malloc(sizeof(Color*));
	*ptr = colors;
	newImage->image = ptr;
	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {
			Color* newColor = evaluateOnePixel(image, i, j);
			colors->R = newColor->R;
			colors->G = newColor->G;
			colors->B = newColor->B;
			colors++;
			free(newColor);
		}
	}
	return newImage;

}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	Image* image = readData(argv[1]);
	Image* newImage = steganography(image);
	writeData(newImage);
	
	freeImage(image);
	freeImage(newImage);
	return 0;
}
