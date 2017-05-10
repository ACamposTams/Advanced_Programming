//Alfredo Campos Tams A01020939 ITC Programación Avanzada
#include "imageHandler.h"
#include "gameSimulator.h"

//function to start the game
void startGame(int numberOfIterations, FILE * initialSetup);

//function to generate each iteration and its corresponding image
void gameProcess(int ** setup, int numberOfIterations,int rows,int columns);

int main(int argc, char ** argv)
{
  // the original file is declared
  FILE * initialSetup;
  int numberOfIterations = 0;

  numberOfIterations = atoi(argv[1]);
  // the original file is opened for reading
  initialSetup = fopen(argv[2],"rb");

  // if the file is not found an error is displayed, if it is found the process of invertion begins
  if (!initialSetup)
  {
    printf("There is no file with that name or the file hasn't been specified in the command line\n");
    return 1;
  }
  else
  {
    // the function of invertion is called
    startGame(numberOfIterations,initialSetup);
    printf("Game completed, images of each iteration created\n");
    return 1;
  }
}

//function to start the game
void startGame (int numberOfIterations,FILE * initialSetup)
{
  // array of chars used to stroe the values from the line, memory is allocated
  //char * line = (char *) malloc(sizeof(char *)*20);
  int columns;
  int rows;

  fscanf(initialSetup, "%d", &rows);
  fscanf(initialSetup, "%d", &columns);

  int ** setup = (int **) malloc(sizeof(int *)*rows);
  int i = 0;
  for (i = 0; i < rows; i++)
  {
    setup[i] = (int *) malloc(sizeof(int)*columns);
  }

  int currentValue = 0;
  i = 0;
  int j = 0;

  while(fscanf(initialSetup,"%d",&currentValue) > 0)
  {
    setup[i][j] = currentValue;
    j++;
    if (j == columns)
    {
      i++;
      j = 0;
    }
  }

  gameProcess(setup,numberOfIterations,rows,columns);

  // for (i = 0; i < rows; i++)
  // {
  //   for (j = 0; j < columns; j++)
  //   {
  //     printf("%d ",setup[i][j]);
  //   }
  //   printf("\n");
  // }

  free(setup);
}

//function to generate each iteration and its corresponding image
void gameProcess(int ** setup, int numberOfIterations,int rows,int columns)
{
  //variable to control the iterations
  int iteration = 0;
  //while structure to do each iteration until the desired iterations are done
  while (iteration <= numberOfIterations)
  {
    //the image file of the current state is made
    createImage(setup,iteration,rows,columns);
    //a new iteration will be done
    iteration ++;
    //the function responsible for the evaluation of the cells does an iteration
    simulateIteration(setup,rows,columns);

    // int i = 0;
    // int j = 0;
    //
    // for (i = 0; i < rows; i++)
    // {
    //   for (j = 0; j < columns; j++)
    //   {
    //     printf("%d ",setup[i][j]);
    //   }
    //   printf("\n");
    // }
  }
}
