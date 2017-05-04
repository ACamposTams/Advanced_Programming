//Alfredo Campos Tams A01020939
#include <unistd.h>
#include "cardHandler.h"

#include <netdb.h>

#define SERVICE_PORT 8642
#define BUFFER_SIZE 1024

void usage(char * program);
void connectToServer(char * address, char * port);
void startGame(int connection_fd);

int main(int argc, char * argv[])
{
    printf("\n=== Blackjack Player ===\n");

    if (argc != 3)
        usage(argv[0]);

    connectToServer(argv[1], argv[2]);

    return 0;
}

// Show the user how to run this program
void usage(char * program)
{
    printf("Usage:\n%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}

// Establish a connection with the server indicated by the parameters
void connectToServer(char * address, char * port)
{
    struct addrinfo hints;
    struct addrinfo * server_info;
    int connection_fd;

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
    if ( getaddrinfo(address, port, &hints, &server_info) == -1 )
    {
        perror("ERROR: getaddrinfo");
        exit(EXIT_FAILURE);
    }

    ///// SOCKET
    // Use the information obtained by getaddrinfo
    connection_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if ( connection_fd == -1 )
    {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }

    ///// CONNECT
    // Connect with the desired port
    if ( connect(connection_fd, server_info->ai_addr, server_info->ai_addrlen) == -1 )
    {
        perror("ERROR: connect");
        exit(EXIT_FAILURE);
    }

    // Release the memory of the list of addrinfo
    freeaddrinfo(server_info);

    // Establish the communication
    startGame(connection_fd);

    // Close the socket
    close(connection_fd);
}

//function that handles the players decisions in the game
void startGame(int connection_fd)
{
    char buffer[BUFFER_SIZE]; //communication buffer
    struct card firstCard; //player's first card
    struct card secondCard; //player's second hand
    struct card dealersOpenCard; //dealer's open card
    int handValue = 0; //hand value

    while (1)
    {
        //the playe rsend the first message to start the game
        printf("\n\nWould you like to start the game? Place your bet (press enter if you want to finish, or place your bet if you want to play): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Finish the connection with a string containing only the '\n'
        if (strlen(buffer) == 1)
        {
            printf("Finishing the connection\n");
            break;
        }

        //the answer is sent to the server
        if ( send(connection_fd, buffer, strlen(buffer)+1, 0) == -1 )
        {
            perror("ERROR: send");
            exit(EXIT_FAILURE);
        }

        // Clear the buffer
        bzero(buffer, BUFFER_SIZE);

        // the player gets it's first two cards
        if ( recv(connection_fd, buffer, BUFFER_SIZE, 0) == -1 )
        {
            perror("ERROR: recv");
            exit(EXIT_FAILURE);
        }

        //the player gets it's two cards and the dealer's open card
        sscanf(buffer,"%d %d %d %d %d %d %d\n",&firstCard.type,&firstCard.value,&secondCard.type,&secondCard.value,&dealersOpenCard.type,&dealersOpenCard.value,&handValue);

        //the terminal show the player it's two first cards
        printf("\n\nThese are your cards:\n");
        printCardValues(firstCard,buffer);
        printCardValues(secondCard,buffer);

        //buffer is cleaned
        bzero(buffer, BUFFER_SIZE);

        //the hand value is calculated so that the player can decid to hit or stand, the user is asekd in case of an
        //Ace which value to use
        handValue = getPlayerScore(firstCard.value,secondCard.value);
        //the current score is shown to the player
        printf("Your current score is: %d\n",handValue);

        //the dealers open card is shown on the terminal to the client
        printf("\n\nThese is the dealer's open card: \n");
        printCardValues(dealersOpenCard,buffer);

        //the player is promted to decide what to do
        printf("\n\nWhat will you do? Type hit if you want another card or stand if you're satisfied with your cards: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        //variables used to validate that the user wrote the correct message either hit or stand
        int rc = strncmp(buffer,"hit",3);
        int valid = 0;

        //while the message is stil incorrect the player will be asked to write the message again
        while (valid == 0)
        {
          //if the player decides to hit then a new function is called
          if (rc == 0)
          {
            //the new player's hand after hitting is returned
            handValue = playerHits(handValue);
            valid = 1;
          }
          else
          {
            //if the player decides different from hitting the message is evaluated again
            rc = strncmp(buffer,"stand",5);
            //if the player decided to stand the two cards are sent to the server with the hand value
            if (rc == 0)
            {
              valid = 1;
            }
            //if the message is wrongly written the user is prompted to write it again wether it be hit or stand
            else
            {
              printf("\n\nYou wrote the message wrong, write it again\n");
              fgets(buffer, BUFFER_SIZE, stdin);
              rc = strncmp(buffer,"hit",3);
            }
          }
        }

        //the players hand after hitting or standing is sent back to the server awaiting results
        sprintf(buffer,"%d",handValue);
        send(connection_fd, buffer, strlen(buffer)+1, 0);

        //buffer is cleaned
        bzero(buffer, BUFFER_SIZE);

        //the client receives the results and prints them in the terminal
        recv(connection_fd, buffer, BUFFER_SIZE, 0);
        printf("\n\n%s\n",buffer);
    }
}
