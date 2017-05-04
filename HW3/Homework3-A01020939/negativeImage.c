//Alfredo Campos Tams A01020939 ITC Programación Avanzada
#include "ppmImageHandler.h"

int main(int argc, char ** argv)
{
  // the original file is declared
  FILE * originalFile;

  // the original file is opened for reading
  originalFile = fopen(argv[1],"rb");

  // if the file is not found an error is displayed, if it is found the process of invertion begins
  if (!originalFile)
  {
    printf("There is no file with that name or the file hasn't been specified in the command line\n");
    return 1;
  }
  else
  {
    // the function of invertion is called
    invertImage(originalFile);
    printf("File inverted saved in a the file inverted-file.ppm\n");
    return 1;
  }
}
