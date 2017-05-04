//Alfredo Campos Tams A01020939
//A slot machine simulation capable of playing up to three lines of three reel
//the compilation line is gcc -o secondPartial -pthread SecondPartial.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <netdb.h>

//Declare global varibles for the size of the reel, the  number of threads and the speed of eeach reel
#define REEL_ZISE 8
#define NUMBER_OF_THREADS 4
#define NUMBER_OF_REELS 5
#define REEL_1_SPEED 1
#define REEL_2_SPEED 3
#define REEL_3_SPEED 7
#define REEL_4_SPEED 7
#define REEL_5_SPEED 7
//variables for the server functionality
#define SERVICE_PORT 8642
#define MAX_QUEUE 5
#define BUFFER_SIZE 1024

struct connectionData
{
  int connection_fd;
  int clientId;
};

//Declaration of each reel's content
char reel1[] = {'A','B','C','D','C','B','D','A'};
char reel2[] = {'B','C','D','A','D','C','B','A'};
char reel3[] = {'D','A','B','A','B','C','D','C'};
char reel4[] = {'D','A','B','A','B','C','D','C'};
char reel5[] = {'D','A','B','A','B','C','D','C'};
char tempArray[REEL_ZISE] = {' '};

char * reels[] = {reel1,reel2,reel3,reel4,reel5};
int reelsSpeed[] = {REEL_1_SPEED,REEL_2_SPEED,REEL_3_SPEED,REEL_4_SPEED,REEL_5_SPEED};

//Declaration of the state arrays for each reel to evaluate if the player wins or looses
char reel1State[3] = {' '};
char reel2State[3] = {' '};
char reel3State[3] = {' '};
char reel4State[3] = {' '};
char reel5State[3] = {' '};

char * reelStates[] = {reel1State,reel2State,reel3State,reel4State,reel5State};


//mutex is declared to control de critical points
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//function to print the current state of the reel, in this case the result
void printReelStates();
//function to make the reels move
void moveReel(char * reel,int speed);
//function to start the threads of the program
void startThreads(struct connectionData connectData);

void * reelProcess(void * numberofReel);
//function to get the results and evaluate each line
void getResults(int lines,char * buffer);

void usage(char * program);
void startServer(char * port);
void waitForConnections(int server_fd);
void * startGame(void * connectData);

//main function
int main (int argc, char * argv[])
{
  //threads are started

  if (argc != 2)
  {
    usage(argv[0]);
  }

  startServer(argv[1]);

  return 0;
}

// Show the user how to run this program
void usage(char * program)
{
    printf("Usage:\n%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}

// Initialize the server to be ready for connections
void startServer(char * port)
{
    struct addrinfo hints;
    struct addrinfo * server_info;
    int server_fd;

    // Prepare the information to determine the local address
    // Clear all fields
    bzero(&hints, sizeof hints);
    // Use internet sockets with IPv4
    hints.ai_family = AF_INET;
    // Use connected sockets
    hints.ai_socktype = SOCK_STREAM;
    // Determine the ip of this machine automatically
    hints.ai_flags = AI_PASSIVE;

    ///// GETADDRINFO
    // Get the actual address of this computer
    if ( getaddrinfo(NULL, port, &hints, &server_info) == -1 )
    {
        perror("ERROR: getaddrinfo");
        exit(EXIT_FAILURE);
    }

    ///// SOCKET
    // Use the information obtained by getaddrinfo
    server_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if ( server_fd == -1 )
    {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }

    ///// BIND
    // Connect with the desired port
    if ( bind(server_fd, server_info->ai_addr, server_info->ai_addrlen) == -1 )
    {
        perror("ERROR: bind");
        exit(EXIT_FAILURE);
    }

    // Release the memory of the list of addrinfo
    freeaddrinfo(server_info);

    ///// LISTEN
    // Prepare for connections
    if ( listen(server_fd, MAX_QUEUE) == -1 )
    {
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }

    printf("Server ready and waiting!\n");
    // Stand by to receive connections from the clients
    waitForConnections(server_fd);

    // Close the socket
    close(server_fd);
}

// Stand by for connections by the clients
void waitForConnections(int server_fd)
{
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int connection_fd;
    //the process id variable is declared
    pid_t pid;

    // Loop to wait for client connections
    while (1)
    {
        ///// ACCEPT
        // Receive incomming connections
        // Get the size of the structure where the address of the client will be stored
        client_address_size = sizeof client_address;
        // Receive the data from the client and open a new socket connection
        connection_fd = accept(server_fd, (struct sockaddr *) &client_address, &client_address_size);
        if ( connection_fd == -1 )
        {
            perror("ERROR: accept");
            exit(EXIT_FAILURE);
        }

        // Identify the client
        // Get the ip address from the structure filled by accept
        inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
        printf("Received connection from: %s : %d\n", client_presentation, client_address.sin_port);

        //fork is generate to create the child processes
        pid = fork();

        //if there is a problem with the fork the program ends
        if (pid < 0)
        {
          perror("ERROR: on fork");
          exit(1);
        }
        //if the process is a child
        else if (pid==0)
        {
          close(server_fd); //the server file descriptor is closed since the child doesn't need it
          struct connectionData connectData;
          connectData.connection_fd = connection_fd;
          connectData.clientId = client_address.sin_port;
          //startGame(connectData);
          startThreads(connectData);

          close(connection_fd); //the file descriptor connected to the client is closed
          exit(0);
        }
        //if it is the parent
        else
        {
          close(connection_fd); //the parent isn't communicating so it doesn't need the connection's file descriptor
        }
    }
}

//function to start the threads of the program
void startThreads(struct connectionData connectData)
{
  pthread_t tid[NUMBER_OF_THREADS];
  int status;
  int i = 0;

  for (i = 0; i < NUMBER_OF_REELS; i++)
  {
    //the thread for the first reel is started
    int * arg = malloc(sizeof(*arg));
    *arg = i;
    status = pthread_create(&tid[i], NULL, &reelProcess, arg);
    if (status)
    {
      fprintf(stderr, "ERROR: pthread_create %d\n", status);
      exit(EXIT_FAILURE);
    }
  }
  //thread for interface and game
  status = pthread_create(&tid[3], NULL, &startGame, (void *)&connectData);
  if (status)
  {
    fprintf(stderr, "ERROR: pthread_create %d\n", status);
    exit(EXIT_FAILURE);
  }
  pthread_exit(NULL);
}

void * startGame(void * connectData)
{
  struct connectionData * connectedData = (struct connectionData *)connectData;
  char buffer[BUFFER_SIZE]; //buffer for the communication
  int chars_read;//validation that the message is received
  int numberOfLines = 4;

  while (numberOfLines != 0)
  {
    chars_read = recv(connectedData->connection_fd, buffer, BUFFER_SIZE, 0);
    // Error when reading
    if ( chars_read == -1 )
    {
        perror("ERROR: recv");
        exit(EXIT_FAILURE);
    }

    //the server receives the player's number of cards and hand value
    sscanf(buffer,"%d", &numberOfLines);
    bzero(buffer, BUFFER_SIZE);

    //the server displays the players final hand values
    printf("\nPlayer %d's number of lines played: %d\n",connectedData->clientId, numberOfLines);

    printReelStates(buffer);
    //sprintf(helpBuffer + strlen(helpBuffer),"%s\n",getResults(numberOfLines,helpBuffer));
    getResults(numberOfLines,buffer);

    printf("%s",buffer);

    if ( send(connectedData->connection_fd, buffer, strlen(buffer)+1, 0) == -1 )
    {
        perror("ERROR: send");
        exit(EXIT_FAILURE);
    }

    bzero(buffer,BUFFER_SIZE);

  }
}

void * reelProcess (void * numberofReel)
{
  int reel = *(int *) numberofReel;
  //the movement continues until the program ends
  while(1)
  {
    moveReel(reels[reel],reelsSpeed[reel]);
  }
  free (numberofReel);
  return NULL;
}

//function to get the results and evaluate each line
void getResults(int lines,char * buffer)
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
      //printf("Won line 1\n\n");
      sprintf(buffer + strlen(buffer),"%s \n","Won line 1");
    }
    else
    {
      //printf("You lost\n\n");
      sprintf(buffer + strlen(buffer),"%s \n","You lost");
    }
  }
  //if two lines are played then only the first two lines are evaluated
  else if (lines == 2)
  {
    //the first line is evaluated using the state arrays
    if (reel1State[0] == reel2State[0] && reel1State[0] == reel3State[0])
    {
      //printf("Won line 1\n\n");
      sprintf(buffer + strlen(buffer),"%s \n","Won line 1");
      line1Win = 1;
    }

    //the second line is evaluated using the state arrays for each reel
    if (reel1State[1] == reel2State[1] && reel1State[1] == reel3State[1])
    {
      //printf("Won line 2\n\n");
      sprintf(buffer + strlen(buffer),"%s \n","Won line 2");
      line2Win = 1;
    }

    //if both lines loose then the player looses
    if (line1Win == 0 && line2Win == 0)
    {
      //printf("You lost\n\n");
      sprintf(buffer + strlen(buffer),"%s \n","You lost");
    }
  }
  //if three lines are played then all lines are evaluated using the state arrays for each reel
  else
  {
    if (reel1State[0] == reel2State[0] && reel1State[0] == reel3State[0])
    {
      //printf("Won line 1\n\n");
      sprintf(buffer + strlen(buffer),"%s \n","Won line 1");
      line1Win = 1;
    }

    if (reel1State[1] == reel2State[1] && reel1State[1] == reel3State[1])
    {
      //printf("Won line 2\n\n");
      sprintf(buffer + strlen(buffer),"%s \n","Won line 2");
      line2Win = 1;
    }

    if (reel1State[2] == reel2State[2] && reel1State[2] == reel3State[2])
    {
      //printf("Won line 3\n\n");
      sprintf(buffer + strlen(buffer),"%s \n","Won line 3");
      line3Win = 1;
    }

    //if the three lines loose then the player looses
    if (line1Win == 0 && line2Win == 0 && line3Win == 0)
    {
      //printf("You lost\n\n");
      sprintf(buffer + strlen(buffer),"%s \n","You lost");
    }
  }
}

//function to print the current state of the reel, in this case the result
void printReelStates(char * buffer)
{
  //the mutex is locked
  pthread_mutex_lock(&mutex);

  int i = 0;
  int j = 0;

  for (i = 0; i < NUMBER_OF_REELS;i++)
  {
    for (j = 0; j < NUMBER_OF_REELS; j++)
    {
      reelStates[i][j] = reels[i][j+4];
    }
  }

  i = 0;

  for (i = 0; i < NUMBER_OF_REELS;i++)
  {
    sprintf(buffer + strlen(buffer),"Line %i: ",i+1);
    for (j = 0; j < NUMBER_OF_REELS; j++)
    {
      //reelStates[i][j] = reels[i][j+4];
      sprintf(buffer + strlen(buffer),"%c ",reelStates[j][i]);
    }
    sprintf(buffer + strlen(buffer),"\n");
  }

  //printf("\n");
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
