#include "imageHandler.h"

//function to create the image of the current iteration
void createImage (int ** setup,int iteration,int rows,int columns)
{
  //variables to generate the file
  FILE * iterationImage;
  char * fileName = (char *) malloc(sizeof(char));
  sprintf(fileName,"iterationNumber%d.pgm",iteration);
  iterationImage = fopen(fileName,"w");

  //the pgm format header is written with the type of file, the widht and height of the image and the max color
  fprintf(iterationImage, "P2\n");
  fprintf(iterationImage, "%i %i\n",columns,rows);
  fprintf(iterationImage, "%i\n",255);

  //variables to iterate throught the array
  int i = 0;
  int j = 0;

  //the information from the current setup is written in the file, dead cells are a 0 and live cells a 255
  //meaning the black space in the image are dead cells and the white space live cells
  for (i = 0; i < rows; i++)
  {
    for (j = 0; j < columns; j++)
    {
      if (setup[i][j] == 0)
      {
        fprintf(iterationImage, "0 ");
      }
      else
      {
        fprintf(iterationImage, "255 ");
      }
    }
    fprintf(iterationImage, "\n");
  }

  //file is closed
  fclose(iterationImage);
}
