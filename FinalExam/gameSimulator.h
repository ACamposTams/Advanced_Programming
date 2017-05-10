#include <stdio.h>
#include <stdlib.h>

//function to generate a single iteration of the simulation
void simulateIteration(int ** setup,int rows,int columns);

//function to evaluate each cell status, the cells around the cell determine if it lives or dies
void cellEvaluation(int ** setup,int ** futureSetup,int i,int j,int rows, int columns);
