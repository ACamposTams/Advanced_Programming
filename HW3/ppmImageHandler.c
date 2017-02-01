#include "ppmImageHandler.h"

// function used to set the settings values from the image to a designated settings structure
struct settings setSettings (char * line, int counter, struct settings currentImage)
{
  int tempValue = 0;
  int width = 0;
  int height = 0;
  int maxColor = 0;

  if (counter == 1)
  {
    if (line[1] == '3')
    {
      currentImage.fileType = 3;
    }
    else if (line[1] == '6')
    {
      currentImage.fileType = 6;
    }
  }
  else if (counter == 2)
  {
    int i = 0;
    int done = 0;
    while (i < sizeof(line) && done == 0)
    {
      if (*(line+i) != ' ' && *(line+i) != '\n')
      {
        if (tempValue != 0)
        {
          tempValue = (tempValue * 10) + (*(line+i) - '0');
          i++;
        }
        else
        {
          tempValue = (*(line+i) - '0');
          i++;
        }
      }
      else
      {
        if (width == 0)
        {
          width = tempValue;
          tempValue = 0;
          currentImage.width = width;
          i++;
        }
        else
        {
          height = tempValue;
          currentImage.height = height;
          done = 1;
        }
      }
    }
  }
  else if (counter == 3)
  {
    int i = 0;
    int done = 0;
    while (i < sizeof(line) && done == 0)
    {
      if (*(line+i) != ' ' && *(line+i) != '\n')
      {
        if (maxColor != 0)
        {
          maxColor = (maxColor * 10) + (*(line+i) - '0');
          i++;
        }
        else
        {
          maxColor = (*(line+i) - '0');
          i++;
        }
      }
      else
      {
        done = 1;
      }
    }
    currentImage.maxColor = maxColor;
  }
  return currentImage;
}

// function used to get the rgb numbers from every pixel in the image into an array the structure colors
void getImageColors(int height, int width, FILE * originalFile, struct colors ** imageColors)
{
  int bufferSize = width*3*6;
  char * line = (char *) malloc(sizeof(char * )*bufferSize);
  int i = 0;
  int j = 0;
  int k = 0;
  int currentColorValue = 0;
  int currentColorType = 0;
  int done = 0;

  struct colors currentColors;

  while (fgets(line,bufferSize,originalFile)!= NULL)
  {
    while (k != bufferSize && done == 0)
    {
      if (line[k] >= 48 && line[k] <= 57)
      {
        currentColorValue = line[k] - '0';
        k++;
        while (line[k] >= 48 && line[k] <= 57)
        {
          currentColorValue = (currentColorValue * 10) + (line[k] - '0');
          k++;
        }
        currentColorType++;
        if (currentColorType == 1)
        {
          currentColors.red = currentColorValue;
          currentColorValue = 0;
        }
        else if (currentColorType == 2)
        {
          currentColors.green = currentColorValue;
          currentColorValue = 0;
        }
        else if (currentColorType == 3)
        {
          currentColors.blue = currentColorValue;
          currentColorValue = 0;
        }
      }
      else if (line[k] == '\n')
      {
        k = 0;
        done = 1;
      }
      else
      {
        k++;
      }
      if (currentColorType == 3)
      {
        currentColorType = 0;
        imageColors[i][j] = currentColors;
        j++;
      }
    }
    j = 0;
    i++;
    done = 0;
  }
}

// function used to write the inverted colors in a new ppm file along with the settings header
void writeInversionToFile(struct colors ** invertedImageColors,struct settings currentImage)
{
  FILE * invertedFile;
  invertedFile = fopen("inverted-test.ppm","w");

  fprintf(invertedFile, "P%i\n",currentImage.fileType);
  fprintf(invertedFile, "%i %i\n",currentImage.width,currentImage.height);
  fprintf(invertedFile, "%i\n",currentImage.maxColor);

  for (int i = 0; i < currentImage.height; i++)
  {
    for (int j = 0; j < currentImage.width; j++)
    {
      fprintf(invertedFile, "%i ",invertedImageColors[i][j].red);
      fprintf(invertedFile, "%i ",invertedImageColors[i][j].green);
      fprintf(invertedFile, "%i  ",invertedImageColors[i][j].blue);
    }
    fprintf(invertedFile, "\n");
  }
}

//function used to call take the colors from the original image array and convert them to the inverted colors to be laet on added to a ppm file
void type3Invertion(struct settings currentImage,FILE * originalFile)
{
  struct colors ** imageColors = (struct colors **) malloc(sizeof (struct colors *)*currentImage.height);
  for (int a = 0; a < currentImage.height; a++)
  {
    imageColors[a] = (struct colors *) malloc(sizeof(struct colors)*currentImage.width);
  }

  struct colors ** invertedImageColors = (struct colors **) malloc(sizeof (struct colors *)*currentImage.height);
  for (int a = 0; a < currentImage.height; a++)
  {
    invertedImageColors[a] = (struct colors *) malloc(sizeof(struct colors)*currentImage.width);
  }

  getImageColors(currentImage.height,currentImage.width,originalFile,imageColors);

  for (int i = 0; i < currentImage.height; i++)
  {
    for (int j = 0; j < currentImage.width; j++)
    {
      invertedImageColors[i][j].red = currentImage.maxColor - imageColors[i][j].red;
      invertedImageColors[i][j].green = currentImage.maxColor - imageColors[i][j].green;
      invertedImageColors[i][j].blue = currentImage.maxColor - imageColors[i][j].blue;
    }
  }

  for (int a = 0; a < currentImage.height; a++)
  {
    free(imageColors[a]);
  }
  free(imageColors);

  writeInversionToFile(invertedImageColors,currentImage);

  for (int a = 0; a < currentImage.height; a++)
  {
    free(invertedImageColors[a]);
  }
  free(invertedImageColors);

}

// function used to start reading the original image file and call the type of invertion according to the type of format the image has
void invertImage(FILE * originalFile)
{
  char * line = (char *) malloc(sizeof(char *)*20);
  struct settings currentImage = {0,0,0,0};
  int done = 0;

  int counter = 0;

  currentImage.width = 20;

  while (fgets(line,(currentImage.width*3*6), originalFile)!= NULL && done == 0)
  {
    if (line[0] != '#')
    {
      if (counter < 3)
      {
        counter++;
        currentImage = setSettings(line, counter, currentImage);
        if (counter == 3)
        {
          line = (char *) malloc(sizeof(char * )*currentImage.width*3*6);
          if (currentImage.fileType == 6)
          {
            //typ6Invertion(currentImage,originalFile);
            //done = 1;
          }
          else if (currentImage.fileType == 3)
          {
            type3Invertion(currentImage,originalFile);
            done = 1;
          }
        }
      }
    }
  }
}
