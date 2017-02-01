//Alfredo Campos Tams A01020939 ITC Programación Avanzada
#include "ppmImageHandler.h"

int main()
{
  // the original file is declared
  FILE * originalFile;

  // the original file is opened for reading
  originalFile = fopen("n_skull-icon.ppm","rb");

  // if the file is not found an error is displayed, if it is found the process of invertion begins
  if (!originalFile)
  {
    printf("There is no file");
    return 1;
  }
  else
  {
    // the function of invertion is called
    invertImage(originalFile);
    return 1;
  }
}
