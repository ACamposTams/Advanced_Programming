//Alfredo Campos Tams A01020939
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    while (1)
    {
        //the playe rsend the first message to start the game
        // printf("\n\nWould you like to start the game? Place your bet (press enter if you want to finish, or place your bet if you want to play): ");
        // fgets(buffer, BUFFER_SIZE, stdin);
        int i = 4;
        int choice = 1;

        while(choice != 0)
        {
          //the user is prompted to decide how many lines to play
          printf("Press 1 to play one line, 2 to play two lines, 3 to play three lines or 0 to finish the program\n");
          fgets(buffer, BUFFER_SIZE, stdin);
          choice = atoi(buffer);
          //if the command is correct then the results are printed
          if (choice == 1 || choice == 2 || choice == 3)
          {
            if ( send(connection_fd, buffer, strlen(buffer)+1, 0) == -1 )
            {
                perror("ERROR: send");
                exit(EXIT_FAILURE);
            }
            recv(connection_fd, buffer, BUFFER_SIZE, 0);

            printf("%s",buffer);

          }
          //if the command is 0 the program ends
          else if (choice == 0)
          {
            printf("See ya\n");
            break;
          }
          //if the number isn't recognized then the user is asked again for a choice
          else
          {
            printf("Unrecognized command\n");
          }
        }

        // // Finish the connection with a string containing only the '\n'
        // if (strlen(buffer) == 1)
        // {
        //     printf("Finishing the connection\n");
        //     break;
        // }
        //
        // //the answer is sent to the server
        // if ( send(connection_fd, buffer, strlen(buffer)+1, 0) == -1 )
        // {
        //     perror("ERROR: send");
        //     exit(EXIT_FAILURE);
        // }
        //
        // // Clear the buffer
        // bzero(buffer, BUFFER_SIZE);
        //
        // // the player gets it's first two cards
        // if ( recv(connection_fd, buffer, BUFFER_SIZE, 0) == -1 )
        // {
        //     perror("ERROR: recv");
        //     exit(EXIT_FAILURE);
        // }
    }
}
