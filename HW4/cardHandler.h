//Alfredo Campos Tams A01020939
//separate file used to handle the game functionality and leave the server and client working on its own functions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//card structure that saves the type of card and its value
struct card
{
  int type;
  int value;
};

//function to create the deck, it receives an array of cards and fills it in with the 52 cards in order
void createDeck(struct card * deck);

//function to print the card showing its type and its value
void printCardValues(struct card aCard,char * buffer);

//function that draws the dealers hand
int dealerDraws(struct card * deck, int dealersHandValue, char * buffer,int clientId,struct card dealersOpenCard);

//function that gets the player's score
int getPlayerScore (int,int);

//function that handles when the player wants another card
int playerHits(int handValue);

//function that gets the winner according to the game results
void getWinner (int playerHandValue, int dealersHandValue,char * buffer,int playerBet);
