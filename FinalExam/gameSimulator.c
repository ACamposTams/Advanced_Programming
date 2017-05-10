#include "gameSimulator.h"

//function to generate a single iteration of the simulation
void simulateIteration(int ** setup,int rows,int columns)
{
  //iterators to travel throught the array
  int i = 0;
  int j = 0;

  //a second array is generated to protect the original setup, its memory is allocated
  int ** futureSetup = (int **) malloc(sizeof(int *)*rows);
  for (i = 0; i < rows; i++)
  {
    futureSetup[i] = (int *) malloc(sizeof(int)*columns);
  }

  //iterator restarted
  i = 0;

  //for structure to travel through the array cell by cell to detemrine its future state
  //#pragma omp parallel default(none), private (i,j,rows,columns), shared (setup,futureSetup)
  #pragma omp for
  for (i = 0; i < rows; i++)
  {
    // #pragma omp parallel default(none), private (i,j,rows,columns), shared (setup,futureSetup)
    #pragma for
    for (j = 0; j < columns; j++)
    {
      //a specific function to evaluate the state of the cell is called
      cellEvaluation(setup,futureSetup,i,j,rows,columns);
    }
  }

  //once every cell is evaluate the setup is replaced with the modified one in the iteration
  for (i = 0; i < rows; i++)
  {
    for (j = 0; j < columns; j++)
    {
      setup[i][j] = futureSetup[i][j];
    }
  }

  free(futureSetup);
}

//function to evaluate each cell status, the cells around the cell determine if it lives or dies
void cellEvaluation(int ** setup,int ** futureSetup,int i,int j,int rows,int columns)
{
  //variables for the evaluation of every cell
  int aliveNeighbours = 0;
  int ending = 0;
  int y = 0;
  int x = 0;

  //the position of the cell matters in case its a corner or part of the first or last column or rows
  //this means that when its position in x or y is smaller than 0 or larger than the number of columns or rows
  //then this position doesn't exist in the matrix and can't be evaluated
  for (y = i - 1; y <= i + 1; y++)
  {
    for (x = j - 1; x <= j + 1; x++)
    {
      if ( y >= 0 && x >= 0 && x < columns && y < rows)
      {
        if (setup[y][x] == 1 && (y != i || x != j))
        {
          aliveNeighbours++;
        }
      }
    }
  }

  // if (i != 0 && j != 0)
  // {
  //   //if the cell isn't in the lower right side corner of the matrix
  //   if (i == rows - 1 && j == columns - 1)
  //   {
  //     for (y = i - 1; y < i + 1; y++)
  //     {
  //       for (x = j - 1; x < j + 1; x++)
  //       {
  //         if (setup[y][x] == 1 && (y != i || x != j))
  //         {
  //           aliveNeighbours++;
  //         }
  //       }
  //     }
  //   }
  //   //if the cell is in the final row
  //   else if (i == rows - 1)
  //   {
  //     for (y = i - 1; y < i + 1; y++)
  //     {
  //       for (x = j - 1; x <= j + 1; x++)
  //       {
  //         if (setup[y][x] == 1 && (y != i || x != j))
  //         {
  //           aliveNeighbours++;
  //         }
  //       }
  //     }
  //   }
  //   //if the cell is in the final column
  //   else if (j == columns -1)
  //   {
  //     for (y = i - 1; y <= i + 1; y++)
  //     {
  //       for (x = j - 1; x < j + 1; x++)
  //       {
  //         if (setup[y][x] == 1 && (y != i || x != j))
  //         {
  //           aliveNeighbours++;
  //         }
  //       }
  //     }
  //   }
  //   //if none of this conditions is met then the cell is in any other position inside the matrix
  //   else
  //   {
  //     for (y = i - 1; y <= i + 1; y++)
  //     {
  //       for (x = j - 1; x <= j + 1; x++)
  //       {
  //         if (setup[y][x] == 1 && (y != i || x != j))
  //         {
  //           aliveNeighbours++;
  //         }
  //       }
  //     }
  //   }
  // }
  // //if the cell is in the top left corner
  // else if (i == 0 && j == 0)
  // {
  //   for (y = i; y <= i + 1; y++)
  //   {
  //     for (x = j; x <= j + 1; x++)
  //     {
  //       if (setup[y][x] == 1 &&(y != i || x != j))
  //       {
  //         aliveNeighbours++;
  //       }
  //     }
  //   }
  // }
  // //if the cell is in the first row
  // else if (i == 0)
  // {
  //   for (y = i; y <= i + 1; y++)
  //   {
  //     for (x = j- 1; x <= j + 1; x++)
  //     {
  //       if (setup[y][x] == 1 && (y != i || x != j))
  //       {
  //         aliveNeighbours++;
  //       }
  //     }
  //   }
  // }
  // //if the cell is in the first column
  // else if (j == 0)
  // {
  //   //if the cell is in the lower left side corner
  //   if (i == rows -1)
  //   {
  //     for (y = i - 1; y < i + 1; y++)
  //     {
  //       for (x = j; x <= j + 1; x++)
  //       {
  //         if (setup[y][x] == 1 && (y != i || x != j))
  //         {
  //           aliveNeighbours++;
  //         }
  //       }
  //     }
  //   }
  //   //if none of this conditions is met then the cell is only in the first column
  //   else
  //   {
  //     for (y = i - 1; y <= i + 1; y++)
  //     {
  //       for (x = j; x <= j + 1; x++)
  //       {
  //         if (setup[y][x] == 1 && (y != i || x != j))
  //         {
  //           aliveNeighbours++;
  //         }
  //       }
  //     }
  //   }
  // }

  //check if the cell is alive
  if (setup[i][j] == 1)
  {
    //check how many alive neighbours it has
    if (aliveNeighbours < 2)
    {
      //less than 2 it dies
      futureSetup[i][j] = 0;
      //printf("Cell %d %d died with: %d neigbours\n",i,j,aliveNeighbours);
    }
    else if (aliveNeighbours <= 3)
    {
      //between 2 and 3 it lives
      futureSetup[i][j] = 1;
      //printf("Cell %d %d survived with: %d neigbours\n",i,j,aliveNeighbours);
    }
    else if (aliveNeighbours > 3)
    {
      //more than 3 it dies
      futureSetup[i][j] = 0;
      //printf("Cell %d %d died with: %d neigbours\n",i,j,aliveNeighbours);
    }
  }
  //the cell is dead
  else if (setup[i][j] == 0)
  {
    //check how many alive neighbours it has
    if (aliveNeighbours == 3)
    {
      //exactly 3 then it now lives
      futureSetup[i][j] = 1;
      //printf("Cell %d %d revived with: %d neigbours\n",i,j,aliveNeighbours);
    }
    else
    {
      //less or more than 3 it stays dead
      futureSetup[i][j] = 0;
      //printf("Cell %d %d remained dead with: %d neigbours\n",i,j,aliveNeighbours);
    }
  }

}
