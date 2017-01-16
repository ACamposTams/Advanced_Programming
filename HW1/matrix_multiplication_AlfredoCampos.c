//Alfredo Campos Tams A01020939 ITC Programación Avanzada

#include <stdio.h>

int main ()
{
  // Declaration of the two text files where the numbers are written and a third file where the result will be written
  FILE *file1;
  FILE *file2;
  FILE *file3;

  // The files with the numbers are open for reading and the third file is open for writing
  file1 = fopen("matrizA.txt","r");
  file2 = fopen("matrizB.txt","r");
  file3 = fopen("matrizC.txt","w");

  // Three matrix are made to store the numbers for the operation and the final result
  double matrix1[3][3];
  double matrix2[3][3];
  double matrix3[3][3];

  // Variable where the number taken from the file will be stored
  double digit;

  // Variables for moving along the matrix
  int i = 0;
  int j = 0;

  // While structure used to take the numbers from the first file and storing them in the matrix1
  // Each line of the file is read and if a number is found then it is stored in the variable digit
  while(fscanf(file1, "%lf", &digit) > 0 )
  {
    matrix1[i][j] = digit;
    j++;
    if (j == 3 && i != 0)
    {
      i++;
    }
  }

  // Closing the first file
  fclose(file1);

  // Reset the variables to move along the second file and matrix
  i = 0;
  j = 0;

  // While structure used to take the numbers from the second file and storing them in the matrix2
  // Each line of the file is read and if a number is found then it is stored in the variable digit
  while(fscanf(file2, "%lf", &digit) > 0 )
  {
    matrix2[i][j] = digit;
    j++;
    if (j == 3 && i != 0)
    {
      i++;
    }
  }

  // Closing the second file
  fclose(file2);

  // Declaration of the variables used to move along the final array and to make the matrix multiplication
  int k;
  int l;
  int m;

  // For structures used to multiply both matrix and store the values in the third resulting matrix
  for (k=0; k < 3; k++)
  {
    for (l=0; l < 3; l++)
    {
      for (m=0; m < 3; m++)
      {
        matrix3[k][l] += (matrix1[k][m] * matrix2[m][l]);
      }
    }
  }

  // For structures used to print the resulting matrix and write it in a third file using the same format as the read files
  for (k=0; k < 3; k++)
  {
    for (l=0; l < 3; l++)
    {
      printf("%.3f ",matrix3[k][l]);
      fprintf(file3, "%.3f ", matrix3[k][l]);
    }
    printf("\n");
    fprintf(file3, "\n");
  }

  // Closing the third file
  fclose(file3);

  return 0;
}
