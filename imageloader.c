/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	FILE* file = fopen(filename, "r");
	int width, height, scale;
	fscanf(file, "%*s %d %d %d", &width, &height, &scale);
	Image* image = (Image*)malloc(sizeof(Image));
	image->cols = width;
	image->rows = height;
	Color* colors = (Color*)malloc(width * height * sizeof(Color));
	// Color* head = (Color*)colors;
	// image->image = &head;
	Color** ptr = (Color**)malloc(sizeof(Color*));
	*ptr = colors;
	image->image = &(*ptr);
	int count = 0;
	int r, g, b;
	while (fscanf(file, "%d %d %d", &r, &g, &b) != -1) {
		colors->R = r;
		colors->G = g;
		colors->B = b;
		count++;
		colors++;
	}
	fclose(file);
	
	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	FILE* file = fopen("studentOutputs/JohnConway.ppm", "w");
	int width = image->cols;
	int height = image->rows;
	fprintf(file, "P3\n");
	fprintf(file, "%d %d\n", width, height);
	fprintf(file, "%d\n", 255);
	Color* color = *(image->image);
	for (int i = 0; i < width * height; i++) {
		fprintf(file, "%3d %3d %3d", color->R, color->G, color->B);
		if ((i + 1) % width == 0) {
			fprintf(file, "\n");
		} else {
			fprintf(file, "   ");
		}
		color++;
	}
	fclose(file);
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	free(*(image->image));
	free(image->image);
	free(image);
}