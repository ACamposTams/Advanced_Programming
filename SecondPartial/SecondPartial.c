//Alfredo Campos Tams A01020939
//A slot machine simulation capable of playing up to three lines of three reel
//the compilation line is gcc -o secondPartial -pthread SecondPartial.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

//Declare global varibles for the size of the reel, the  number of threads and the speed of eeach reel
#define REEL_ZISE 8
#define NUMBER_OF_THREADS 4
#define REEL_1_SPEED 1
#define REEL_2_SPEED 3
#define REEL_3_SPEED 7

//Declaration of each reel's content
char reel1[] = {'A','B','C','D','C','B','D','A'};
char reel2[] = {'B','C','D','A','D','C','B','A'};
char reel3[] = {'D','A','B','A','B','C','D','C'};
char tempArray[REEL_ZISE] = {' '};

//Declaration of the state arrays for each reel to evaluate if the player wins or looses
char reel1State[3] = {' '};
char reel2State[3] = {' '};
char reel3State[3] = {' '};

//mutex is declared to control de critical points
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//function to print the current state of the reel, in this case the result
void printReelStates();
//function to make the reels move
void moveReel(char * reel,int speed);
//function to start the threads of the program
void startThreads();
//function that runs the reel 1 movement
void * reel1Process();
//function that runs the reel 2 movement
void * reel2Process();
//function that runs the reel 3 movement
void * reel3Process();
//function that controls the user interface, where the player decides to play and how many lines to play
void * userInterface();
//function to get the results and evaluate each line
void getResults(int lines);

//main function
int main ()
{
  //threads are started
  startThreads();
  return 0;
}

//function to start the threads of the program
void startThreads()
{
  pthread_t tid[NUMBER_OF_THREADS];
  int status;

  //the thread for the first reel is started
  status = pthread_create(&tid[0], NULL, &reel1Process, NULL);
  if (status)
  {
    fprintf(stderr, "ERROR: pthread_create %d\n", status);
    exit(EXIT_FAILURE);
  }
  //the thread for the second reel is started
  status = pthread_create(&tid[1], NULL, &reel2Process, NULL);
  if (status)
  {
    fprintf(stderr, "ERROR: pthread_create %d\n", status);
    exit(EXIT_FAILURE);
  }
  //the thread for the third reel is started
  status = pthread_create(&tid[2], NULL, &reel3Process, NULL);
  if (status)
  {
    fprintf(stderr, "ERROR: pthread_create %d\n", status);
    exit(EXIT_FAILURE);
  }
  //the thread for the user interface is started
  status = pthread_create(&tid[3], NULL, &userInterface, NULL);
  if (status)
  {
    fprintf(stderr, "ERROR: pthread_create %d\n", status);
    exit(EXIT_FAILURE);
  }
  //the threads are prevented from ending
  pthread_exit(NULL);
}

//function that runs the reel 1 movement
void * reel1Process()
{
  //the movement continues until the program ends
  while(1)
  {
    moveReel(reel1,REEL_1_SPEED);
  }
  return NULL;
}

//function that runs the reel 2 movement
void * reel2Process()
{
  //the movement continues until the program ends
  while(1)
  {
    moveReel(reel2,REEL_2_SPEED);
  }
  return NULL;
}

//function that runs the reel 3 movement
void * reel3Process()
{
  //the movement continues until the program ends
  while(1)
  {
    moveReel(reel3,REEL_3_SPEED);
  }
  return NULL;
}

void * userInterface()
{
  int i = 4;
  int choice = 1;
  //the program continues until the user decides to end
  while(choice != 0)
  {
    //the user is prompted to decide how many lines to play
    printf("Press 1 to play one line, 2 to play two lines, 3 to play three lines or 0 to finish the program\n");
    scanf("%i",&choice);
    //if the command is correct then the results are printed
    if (choice == 1 || choice == 2 || choice == 3)
    {
      //the reels are printed and the results are shown
      printReelStates();
      getResults(choice);
    }
    //if the command is 0 the program ends
    else if (choice == 0)
    {
      printf("See ya\n");
      exit(0);
    }
    //if the number isn't recognized then the user is asked again for a choice
    else
    {
      printf("Unrecognized command\n");
    }
  }
  return NULL;
}

//function to get the results and evaluate each line
void getResults(int lines)
{
  //auxiliary variables to determine if the player lost
  int line1Win = 0;
  int line2Win = 0;
  int line3Win = 0;

  //if only one line is played then only the first line is evaluated
  if (lines == 1)
  {
    //the first line is evaluated using the state arrays for each reel
    if (reel1State[0] == reel2State[0] && reel1State[0] == reel3State[0])
    {
      printf("Won line 1\n\n");
    }
    else
    {
      printf("You lost\n\n");
    }
  }
  //if two lines are played then only the first two lines are evaluated
  else if (lines == 2)
  {
    //the first line is evaluated using the state arrays
    if (reel1State[0] == reel2State[0] && reel1State[0] == reel3State[0])
    {
      printf("Won line 1\n\n");
      line1Win = 1;
    }

    //the second line is evaluated using the state arrays for each reel
    if (reel1State[1] == reel2State[1] && reel1State[1] == reel3State[1])
    {
      printf("Won line 2\n\n");
      line2Win = 1;
    }

    //if both lines loose then the player looses
    if (line1Win == 0 && line2Win == 0)
    {
      printf("You lost\n\n");
    }
  }
  //if three lines are played then all lines are evaluated using the state arrays for each reel
  else
  {
    if (reel1State[0] == reel2State[0] && reel1State[0] == reel3State[0])
    {
      printf("Won line 1\n\n");
      line1Win = 1;
    }

    if (reel1State[1] == reel2State[1] && reel1State[1] == reel3State[1])
    {
      printf("Won line 2\n\n");
      line2Win = 1;
    }

    if (reel1State[2] == reel2State[2] && reel1State[2] == reel3State[2])
    {
      printf("Won line 3\n\n");
      line3Win = 1;
    }

    //if the three lines loose then the player looses
    if (line1Win == 0 && line2Win == 0 && line3Win == 0)
    {
      printf("You lost\n\n");
    }
  }
}

//function to print the current state of the reel, in this case the result
void printReelStates()
{
  //the mutex is locked
  pthread_mutex_lock(&mutex);
  //the array of the reel 1 is filled with the reel values for each line
  reel1State[0] = reel1[4];
  reel1State[1] = reel1[5];
  reel1State[2] = reel1[6];

  //the array of the reel 2 is filled with the reel values for each line
  reel2State[0] = reel2[4];
  reel2State[1] = reel2[5];
  reel2State[2] = reel2[6];

  //the array of the reel 3 is filled with the reel values for each line
  reel3State[0] = reel3[4];
  reel3State[1] = reel3[5];
  reel3State[2] = reel3[6];

  int i = 0;

  //the lines are printed to the user so that he knows what results he got
  for (i = 0; i < 3; i++)
  {
    printf("Line %i: %c %c %c\n",i+1,reel1State[i],reel2State[i],reel3State[i]);
  }

  printf("\n");
  //the mutex is unlocked
  pthread_mutex_unlock(&mutex);
}

//function to make the reels move,
//it uses the mutex to allow each reel process to access the temporal array and not interfere with the other reels
void moveReel(char * reel,int speed)
{
  int i = 0;
  //mutek locked
  pthread_mutex_lock(&mutex);
  //the temporal array is filled with the new positions based on the speed of the reel
  for (i = 0; i < REEL_ZISE; i++)
  {
    tempArray[i] = reel[(i + speed) % REEL_ZISE];
  }
  //the temporal array is copied to the reel with the new positions
  for (i = 0; i < REEL_ZISE; i++)
  {
    reel[i] = tempArray[i];
  }
  //mutex unlocked
  pthread_mutex_unlock(&mutex);
}
