//Alfredo Campos Tams A01020939 ITC Programación Avanzada
#include "ppmImageHandler.h"

// function used to set the settings values from the image to a designated settings structure
struct settings setSettings (char * line, int counter, struct settings currentImage)
{
  // variable used to calculate the width and the height more easilly
  int tempValue = 0;
  // variable used to store the width of the image
  int width = 0;
  // variable used to store the height of the image
  int height = 0;
  // variable used to store the max color of the image
  int maxColor = 0;

  // the variable counter is evaluted, it works as a state in order to get the information from the image settings
  if (counter == 1)
  {
    // the file type is obtained
    if (line[1] == '3')
    {
      // the file type is assigned to the settings structure
      currentImage.fileType = 3;
    }
    else if (line[1] == '6')
    {
      // the file type is assigned to the settings structure
      currentImage.fileType = 6;
    }
  }
  else if (counter == 2)
  {
    // the file width and height are obtained
    int i = 0;
    int done = 0;
    // while structure used to move along the line to get the width and height
    while (i < sizeof(line) && done == 0)
    {
      if (*(line+i) != ' ' && *(line+i) != '\n')
      {
        // if the char is the second value then it is multiplied in order to get the exact number
        if (tempValue != 0)
        {
          tempValue = (tempValue * 10) + (*(line+i) - '0');
          i++;
        }
        // if the char is the  first value then it is only added in order to get the exact number
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
          // the width is assigned to the settings structure
          currentImage.width = width;
          i++;
        }
        else
        {
          height = tempValue;
          // the height is assigned to the settings structure
          currentImage.height = height;
          done = 1;
        }
      }
    }
  }
  else if (counter == 3)
  {
    // the file max color is obtained
    int i = 0;
    int done = 0;
    // while structure used to move along the line to get the value of the color
    while (i < sizeof(line) && done == 0)
    {
      if (*(line+i) != ' ' && *(line+i) != '\n')
      {
        // the color is evaluated to generate the exact number
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
    // the max color is assigned to the settings structure
    currentImage.maxColor = maxColor;
  }
  // the settings structure is returned
  return currentImage;
}

// function used to get the rgb numbers from every pixel in the image into an array the structure colors
void getImageColors(int height, int width, FILE * originalFile, struct colors ** imageColors)
{
  // the buffer size is calculated with the width of the image, number of pixels, multiplied by 3 (r,g and b) and by 6
  // since the documentation states the biggest number for a color can be only 5 digits long, a sixth is added for space
  int bufferSize = width*3*6;

  // the memory for the line is allocated
  char * line = (char *) malloc(sizeof(char * )*bufferSize);

  // variables used to travel the line and fill the colors array
  int i = 0;
  int j = 0;
  int k = 0;
  int currentColorValue = 0;
  int currentColorType = 0;
  int done = 0;

  // colors structure used to fill the array
  struct colors currentColors;

  // while structure that moves along the file line by line
  while (fgets(line,bufferSize,originalFile)!= NULL)
  {
    // while structure thath moves along the file
    while (k != bufferSize && done == 0)
    {
      // if the char is a number then it will be evaluated as a number
      if (line[k] >= 48 && line[k] <= 57)
      {
        currentColorValue = line[k] - '0';
        k++;
        // while structure used so that the next character is evaluated in order to get number longer than a single digit, if it is a space it ends
        while (line[k] >= 48 && line[k] <= 57)
        {
          currentColorValue = (currentColorValue * 10) + (line[k] - '0');
          k++;
        }
        currentColorType++;
        // the color type is evaluated to assign the red, blue and green respectively in the structure
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
        // if the line ends then the program moves to the next line to be evaluated and the colors stored in a matrix
        k = 0;
        done = 1;
      }
      else
      {
        // if is it a space then the next character is evaluated
        k++;
      }
      // if it is the blue color then the next color need to be red and the structure needs to be filled again while
      // the current structured is stored in the array
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
void writeInversionToFileType3(struct colors ** invertedImageColors,struct settings currentImage)
{
  // a new file is created to store the new inverrted color matrix
  FILE * invertedFile;
  invertedFile = fopen("inverted-file.ppm","w");

  // the inverte images settings are written in the file
  fprintf(invertedFile, "P%i\n",currentImage.fileType);
  fprintf(invertedFile, "%i %i\n",currentImage.width,currentImage.height);
  fprintf(invertedFile, "%i\n",currentImage.maxColor);

  // for structure used to fill the new file with the inverted colors
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
  fclose(invertedFile);
}

void writeInversionToFileType6(struct colors * invertedImageColors,struct settings currentImage)
{
  // the inverted file is declared and opened
  FILE * invertedFile;
  invertedFile = fopen("inverted-file.ppm","w");

  // the inverted images settings are written in the file
  fprintf(invertedFile, "P%i\n",currentImage.fileType);
  fprintf(invertedFile, "%i %i\n",currentImage.width,currentImage.height);
  fprintf(invertedFile, "%i\n",currentImage.maxColor);

  // the inverted color information is written in the file as a single stream using de fwrite function and the array of
  // color structures which hold de rgb values for every pixel
  fwrite(invertedImageColors, 12 * currentImage.width, currentImage.height, invertedFile);

  fclose(invertedFile);
}

//function used to take the colors from the original image array and convert them to the inverted colors to be laet on added to a ppm file
void type3Invertion(struct settings currentImage,FILE * originalFile)
{
  // an array of image colors is created to store the colors from the file, the memory is allocated to fits these numbers
  struct colors ** imageColors = (struct colors **) malloc(sizeof (struct colors *)*currentImage.height);
  for (int a = 0; a < currentImage.height; a++)
  {
    imageColors[a] = (struct colors *) malloc(sizeof(struct colors)*currentImage.width);
  }
  // an array of inverted image colors is created to store the colors for the inverted file, the memory is allocated to fits these numbers
  struct colors ** invertedImageColors = (struct colors **) malloc(sizeof (struct colors *)*currentImage.height);
  for (int a = 0; a < currentImage.height; a++)
  {
    invertedImageColors[a] = (struct colors *) malloc(sizeof(struct colors)*currentImage.width);
  }

  // function called to get the original colors from the image and store them in a matrix
  getImageColors(currentImage.height,currentImage.width,originalFile,imageColors);

  // the inverte matrix is filled with the inverted values of the original matrix
  for (int i = 0; i < currentImage.height; i++)
  {
    for (int j = 0; j < currentImage.width; j++)
    {
      invertedImageColors[i][j].red = currentImage.maxColor - imageColors[i][j].red;
      invertedImageColors[i][j].green = currentImage.maxColor - imageColors[i][j].green;
      invertedImageColors[i][j].blue = currentImage.maxColor - imageColors[i][j].blue;
    }
  }

  // the memory for the original matrix is freed
  for (int a = 0; a < currentImage.height; a++)
  {
    free(imageColors[a]);
  }
  free(imageColors);

  // function called to store the inverted colors matrix in a new file
  writeInversionToFileType3(invertedImageColors,currentImage);

  // the memory for the inverted matrix is freed
  for (int a = 0; a < currentImage.height; a++)
  {
    free(invertedImageColors[a]);
  }
  free(invertedImageColors);

}

// function used to take the colors from the original binary file and convert them in the correct colors por the P6 type of files
void type6Invertion(struct settings currentImage,FILE * originalFile)
{
  // array of color structures for the original and inverted image which will hold all the color data for the original and inverted files
  struct colors * originalImage;
  struct colors * invertedImage;

  // memory is allocated for both structures
  originalImage = (struct colors *)malloc(currentImage.width * currentImage.height * sizeof(struct colors));
  invertedImage = (struct colors *)malloc(currentImage.width * currentImage.height * sizeof(struct colors));

  // the information in the binary file is read as a single line, with fread it is stored in the originalImage array
  // number 12 is used since there are 3 different numbers (r,g and b) and each one weights 4 bytes
  fread(originalImage, 12 * currentImage.width, currentImage.height, originalFile);

  // for structure used to save the inverted data in the inverted array which will be written in the inverted file
  for (int i = 0; i < currentImage.width * currentImage.height; i++)
  {
    invertedImage[i].red = currentImage.maxColor - originalImage[i].red;
    invertedImage[i].green = currentImage.maxColor - originalImage[i].green;
    invertedImage[i].blue = currentImage.maxColor - originalImage[i].blue;
  }

  // the function is called to write the inverted information in the intended inverted file
  writeInversionToFileType6(invertedImage,currentImage);

  // memory is freed for both the original and inverted arrays
  free(originalImage);
  free(invertedImage);
}

// function used to start reading the original image file and call the type of invertion according to the type of format the image has
void invertImage(FILE * originalFile)
{
  // array of chars used to stroe the values from the line, memory is allocated
  char * line = (char *) malloc(sizeof(char *)*20);
  // settings structured initialized to store image values
  struct settings currentImage = {0,0,0,0};
  int done = 0;

  int counter = 0;

  // default width value used to store the first values, settings, from the image
  currentImage.width = 20;

  // while structure to travel line by line in the file to get the information
  while (fgets(line,(currentImage.width*3*6), originalFile)!= NULL && done == 0)
  {
    if (line[0] != '#')
    {
      // if the line isn't a comment it is read
      if (counter < 3)
      {
        // if counter is < 3 that means the program is still reading the settings
        counter++;
        // function called to set the images settings in a settings structure
        currentImage = setSettings(line, counter, currentImage);
        if (counter == 3)
        {
          if (currentImage.fileType == 6)
          {
            // if the image is P6 then it calls this function
            type6Invertion(currentImage,originalFile);
            done = 1;
          }
          else if (currentImage.fileType == 3)
          {
            // if the image is P3 then it calls this function
            type3Invertion(currentImage,originalFile);
            done = 1;
          }
        }
      }
    }
  }

  free(line);
}
