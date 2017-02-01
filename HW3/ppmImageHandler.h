//Alfredo Campos Tams A01020939 ITC Programación Avanzada

#include <stdio.h>
#include <stdlib.h>

// structure used to save the image's settings such as heigh, widht, etc.
struct settings
{
  int fileType;
  int width;
  int height;
  int maxColor;
};

// structure used to save the rgb colors to be used in an array
struct colors
{
  int red;
  int blue;
  int green;
};

// function used to set the settings values from the image to a designated settings structure
struct settings setSettings (char * line, int counter, struct settings currentImage);

// function used to get the rgb numbers from every pixel in the image into an array the structure colors
void getImageColors(int height, int width, FILE * originalFile, struct colors ** imageColors);

// function used to write the inverted colors in a new ppm file along with the settings header
void writeInversionToFile(struct colors ** invertedImageColors,struct settings currentImage);

//function used to call take the colors from the original image array and convert them to the inverted colors to be laet on added to a ppm file
void type3Invertion(struct settings currentImage,FILE * originalFile);

// function used to start reading the original image file and call the type of invertion according to the type of format the image has
void invertImage(FILE * originalFile);
