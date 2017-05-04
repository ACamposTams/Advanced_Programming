//Alfredo Campos Tams A01020939
// Include libraries for sockets
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "cardHandler.h"

#define SERVICE_PORT 8642
#define MAX_QUEUE 5
#define BUFFER_SIZE 1024

void usage(char * program);
void startServer(char * port);
void waitForConnections(int server_fd);
void startGame(int connection_fd,struct card * deck,int clientId);

int main(int argc, char * argv[])
{
    printf("\n=== Blackjack Server ===\n");

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
    //the deck of cards memory is allocated to hold the 52 cards
    struct card * deck = (struct card *) malloc(sizeof(struct card *)*52);
    //the process id variable is declared
    pid_t pid;
    //the deck is filled with the correspoding 52 cards
    createDeck(deck);

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
          startGame(connection_fd,deck,client_address.sin_port); //the game is executed until the connection with the client ends
          close(connection_fd); //the file descriptor connected to the client is closed
          exit(0);
        }
        //if it is the parent
        else
        {
          close(connection_fd); //the parent isn't communicating so it doesn't need the connection's file descriptor
        }
    }

    //the deck memory is freed after the game ends
    free(deck);
}

//function that handles the game functionality
void startGame(int connection_fd, struct card * deck,int clientId)
{
    char buffer[BUFFER_SIZE]; //buffer for the communication
    char helpBuffer[BUFFER_SIZE]; //auxiliary buffer to add to the main communication buffer to avoid loss of text
    int chars_read;//validation that the message is received
    time_t t; // time variable to get a random number
    int randomCard; //variable that holds the random card numbet out of 52 cards
    struct card firstCard; //structure for the first drawn hand
    struct card secondCard; //structure for the second drawn hand
    struct card dealersOpenCard; //structure for the first dealers card which is and open card that the player sees
    int playerHandValue = 0;
    int dealerHandValue = 0;
    int playerBet = 0;

    while (1)
    {
        // Clear the buffer
        bzero(buffer, BUFFER_SIZE);

        ///// RECV
        // Read the request from the client
        chars_read = recv(connection_fd, buffer, BUFFER_SIZE, 0);
        // Error when reading
        if ( chars_read == -1 )
        {
            perror("ERROR: recv");
            exit(EXIT_FAILURE);
        }
        // Connection finished
        if ( chars_read == 0 )
        {
            printf("Client %d disconnected\n",clientId);
            break;
        }

        sscanf(buffer,"%d",&playerBet);

        //random number is generated to get a random card
        srand((unsigned) time(&t));
        randomCard = rand() % 51;
        //random card is drawn from deck
        firstCard = deck[randomCard];

        //random number is generated to get a random card
        randomCard = rand() % 51;
        //random card is drawn from deck
        secondCard = deck[randomCard];

        //random number is generated to get a random card
        randomCard = rand() % 51;
        //random card is drawn from deck
        dealersOpenCard = deck[randomCard];

        //buffer is freed
        bzero(buffer, BUFFER_SIZE);

        //the buffer is filled to send the two cards drawn for the player and the dealer's open card
        sprintf(buffer,"%d %d %d %d %d %d %d",firstCard.type,firstCard.value,secondCard.type,secondCard.value,dealersOpenCard.type,dealersOpenCard.value,0);

        // Send a reply to the client with the two drawn cards
        if ( send(connection_fd, buffer, strlen(buffer)+1, 0) == -1 )
        {
            perror("ERROR: send");
            exit(EXIT_FAILURE);
        }

        //buffer is freed
        bzero(buffer, BUFFER_SIZE);

        //server receives the player's final hand
        chars_read = recv(connection_fd, buffer, BUFFER_SIZE, 0);
        // Error when reading
        if ( chars_read == -1 )
        {
            perror("ERROR: recv");
            exit(EXIT_FAILURE);
        }
        // Connection finished
        if ( chars_read == 0 )
        {
            printf("\n\nClient %d disconnected\n",clientId);
            break;
        }

        //the server receives the player's number of cards and hand value
        sscanf(buffer,"%d \n", &playerHandValue);
        //the server displays the players final hand values
        printf("\n\nPlayer's %d score is: %d \n",clientId,playerHandValue);

        //the server draws the dealers hand
        sprintf(buffer,"Time to take my cards: \n");

        //the dealers hand is drawn
        dealerHandValue = dealerDraws(deck,dealerHandValue,buffer,clientId,dealersOpenCard);

        //the helping buffer is filled with the dealers hand status
        sprintf(helpBuffer,"My score is: %d \n",dealerHandValue);
        //the helping buffer is added to the buffer with the dealaers hand
        strcat(buffer,helpBuffer);

        //the winner is decided with both player's hands
        getWinner(playerHandValue,dealerHandValue,buffer,playerBet);
        //the result is printed in the server's terminal
        printf("\n\nClient %d results: \n",clientId);
        printf("%s\n",buffer);

        //the results are send to the client for him to see who won
        if ( send(connection_fd, buffer, strlen(buffer)+1, 0) == -1 )
        {
            perror("ERROR: send");
            exit(EXIT_FAILURE);
        }
    }
}
