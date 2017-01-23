//Alfredo Campos Tams A01020939 ITC Programación Avanzada

#include <stdio.h>
#include <string.h>

int const lineBuffer = 100;
int const wordBuffer = 10;

// function used to find how many time a certain word is repeated in a sentence, the funciton returns an int
int findWord(char[lineBuffer],char[wordBuffer],int,int);

int main ()
{
  // array of chars used to fill in with the sentence
  char lineEntered[lineBuffer];
  memset(lineEntered,' ', lineBuffer*sizeof(char));
  //*(lineEntered+0) = " ";
  // array of chars used to fill with the word that will be searched
  char word[wordBuffer];
  memset(word,' ', wordBuffer*sizeof(char));

  // the user is asked for the line, it is limited to 100 characters
  printf("Enter the line in which the word will be searched for (limit to 100 characters): ");
  fgets(lineEntered,lineBuffer,stdin);

  // the yser is asked for the word that will be searched for, ot is limited to 10 characters
  printf("Enter the word that will be searched for in the line (limit to 10 characters): ");
  fgets(word,wordBuffer,stdin);

  //int variable used to save the length of the sentence that will be analized
  int sizeLine = strlen(lineEntered) - 1;

  // int variable used to save the length of the word that will be searched for
  int sizeWord = strlen(word) - 1;

  // int variable that recieves the number of times the word is found in a sentence
  int timesFound = findWord(lineEntered,word,sizeLine,sizeWord);

  // the user is shown how many times the word is repeated in the sentence
  printf("The word was found %i times\n",timesFound);
}

// function used to find how many time a certain word is repeated in a sentence, the funciton returns an int
int findWord(char line[lineBuffer], char word[wordBuffer], int sizeLine, int sizeWord)
{
  // the variables to travel along the arrays are set, i and j
  int i = 0;
  int j = 0;

  // the variable that tell how many times the word appears is set
  int repetitions = 0;

  // for structure used to travel along the array of the sentence seearching for the asked word
  for (i=0; i< sizeLine;i++)
  {
    // the character in the sentence is compared to the character in the word
    if (*(line+i) == *(word+j))
    {
      // if the characters match then the next character in the word will be compare dto the next character in the sentence
      j++;
      // if j is the same as the sixe of the word, then the same word has been found in the line
      if (j == sizeWord)
      {
        // another if is used to determine it is the exact same word, for example the word for is found in the word form but that doesn't mean the word for has appeared
        // therefore the next character in the sentence is evaluated
        if (*(line+i+1) == ' ' || *(line+i+1) == '\n' || *(line+i+1) == ',' || *(line+i+1) == '.' || *(line+i+1) == ';')
        {
          // if the next character in the sentence is something different than a letter then the word has been found in the sentence and a repetition is found
          repetitions++;
          // j is set to 0 to start comparing from the first character in the word to the next character in the sentence
          j = 0;
        }
        else
        {
          // if the next character in the sentence is a letter, then the word hasn't been found and j is set to 0 to start comparing the first character in the word again
          j = 0;
        }
      }
    }
    else
    {
      // if the character doesn't match then j is set to 0 to start comparing from the start of the word again
      j = 0;
    }
  }

  // once the for structure finishes the number of repetitions found is returned
  return repetitions;
}
