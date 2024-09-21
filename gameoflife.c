/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

int rowMove[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int colMove[] = {-1, 0, 1, -1, 1, -1, 0, 1};

int fixRowPosition(Image* image, int tempRow) {
	if (tempRow < 0) {
		tempRow = image->rows - 1;
		return tempRow;
	}
	if (tempRow >= image->rows) {
		tempRow = 0;
		return tempRow;
	}
	return tempRow;
}

int fixColPosition(Image* image, int tempCol) {
	if (tempCol < 0) {
		tempCol = image->cols - 1;
		return tempCol;
	}
	if (tempCol >= image->cols) {
		tempCol = 0;
		return tempCol;
	}
	return tempCol;
}

int evaluateRedArround(Image* image, int row, int col, uint32_t rule, int bitPosition) {
	int arroundAlive = 0;
	for (int i = 0; i < sizeof(rowMove) / sizeof(rowMove[0]); i++) {
		int tempRow = row + rowMove[i];
		int tempCol = col + colMove[i];
		tempRow = fixRowPosition(image, tempRow);
		tempCol = fixColPosition(image, tempCol);
		Color* tempColor = *(image->image) + tempRow * image->cols + tempCol;
		arroundAlive += (tempColor->R >> bitPosition) & 1;
	}

	Color* currentColor = *(image->image) + row * image->cols + col;
	int isAlive = (currentColor->R >> bitPosition) & 1;
	if (!isAlive) {
		if ((rule >> arroundAlive) & 1) {
			return 1;
		} else {
			return 0;
		}
	} else{
		if ((rule >> (arroundAlive + 9)) & 1) {
			return 1;
		} else {
			return 0;
		}
	}
}

int evaluateGreenArround(Image* image, int row, int col, uint32_t rule, int bitPosition) {
	int arroundAlive = 0;
	for (int i = 0; i < sizeof(rowMove) / sizeof(rowMove[0]); i++) {
		int tempRow = row + rowMove[i];
		int tempCol = col + colMove[i];
		tempRow = fixRowPosition(image, tempRow);
		tempCol = fixColPosition(image, tempCol);
		Color* tempColor = *(image->image) + tempRow * image->cols + tempCol;
		arroundAlive += (tempColor->G >> bitPosition) & 1;
	}
	
	Color* currentColor = *(image->image) + row * image->cols + col;
	int isAlive = (currentColor->G >> bitPosition) & 1;
	if (!isAlive) {
		if ((rule >> arroundAlive) & 1) {
			return 1;
		} else {
			return 0;
		}
	} else{
		if ((rule >> (arroundAlive + 9)) & 1) {
			return 1;
		} else {
			return 0;
		}
	}
}

int evaluateBlueArround(Image* image, int row, int col, uint32_t rule, int bitPosition) {
	int arroundAlive = 0;
	for (int i = 0; i < sizeof(rowMove) / sizeof(rowMove[0]); i++) {
		int tempRow = row + rowMove[i];
		int tempCol = col + colMove[i];
		tempRow = fixRowPosition(image, tempRow);
		tempCol = fixColPosition(image, tempCol);
		Color* tempColor = *(image->image) + tempRow * image->cols + tempCol;
		arroundAlive += (tempColor->B >> bitPosition) & 1;
	}
	
	Color* currentColor = *(image->image) + row * image->cols + col;
	int isAlive = (currentColor->B >> bitPosition) & 1;
	if (!isAlive) {
		if ((rule >> arroundAlive) & 1) {
			return 1;
		} else {
			return 0;
		}
	} else{
		if ((rule >> (arroundAlive + 9)) & 1) {
			return 1;
		} else {
			return 0;
		}
	}
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	Color* nextColor = malloc(sizeof(Color));
	nextColor->R = 0;
	nextColor->G = 0;
	nextColor->B = 0;

	for (int i = 7; i >= 0; i--) {
		nextColor->R = 2 * nextColor->R + evaluateRedArround(image, row, col, rule, i);
	}

	for (int i = 7; i >= 0; i--) {
		nextColor->G = 2 * nextColor->G + evaluateGreenArround(image, row, col, rule, i);
	}

	for (int i = 7; i >= 0; i--) {
		nextColor->B = 2 * nextColor->B + evaluateBlueArround(image, row, col, rule, i);
	}
	

	return nextColor;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	Image* newImage = malloc(sizeof(Image));
	newImage->cols = image->cols;
	newImage->rows = image->rows;
	Color* newColors = malloc(image->cols * image->rows * sizeof(Color));
	Color** ptr = malloc(sizeof(Color*));
	*ptr = newColors;
	newImage->image = ptr;

	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {
			Color* newColor = evaluateOneCell(image, i, j, rule);
			newColors->R = newColor->R;
			newColors->G = newColor->G;
			newColors->B = newColor->B;
			newColors++;
			free(newColor);
		}
	}
	return newImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc < 3) {
		printf("usage: ./gameOfLife filename rule\n");
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808.");
		exit(-1);
	}

	char* fileName = argv[1];
	uint32_t rule = strtol(argv[2], NULL, 16);

	Image* image = readData(fileName);
	Image* newImage = life(image, rule);
	writeData(newImage);
	freeImage(image);
	freeImage(newImage);
	return 0;
}
