//Alfredo Campos Tams A01020939
//separate file used to handle the game functionality and leave the server and client working on its own functions
#include "cardHandler.h"

//function to create the deck, it receives an array of cards and fills it in with the 52 cards in order
void createDeck(struct card * deck)
{
  //initialize the counters
  int k = 0;
  int i = 0;
  int j = 0;

  //for structure to fill the deck with the 52 cards with its 4 types and corresponding 13 values from Ace to King
  for (i=0; i<4; i++)
  {
    for (j=1; j<=13; j++)
    {
      struct card newCard;
      newCard.type = i;
      newCard.value = j;
      deck[k] = newCard;
      k++;
    }
  }
}

//function to print the card showing its type and its value
void printCardValues(struct card aCard, char * buffer)
{
  //buffer used as a secondary buffer to paste next to the current buffer
  //this second buffer is filles with the card type and value;
  char helpingBuffer[20];

  //the card type and value is reviewed and printed in screen showing its type and value
  //it shows if the card is a Jack, Queen, Ace or King also
  if (aCard.type == 0)
  {
    printf("Hearts");
    strcat(buffer,"Hearts");
  }
  else if (aCard.type == 1)
  {
    printf("Diamonds");
    strcat(buffer,"Diamonds");
  }
  else if (aCard.type == 2)
  {
    printf("Spikes");
    strcat(buffer,"Spikes");
  }
  else if (aCard.type == 3)
  {
    printf("Clubs");
    strcat(buffer,"Clubs");
  }

  if (aCard.value == 1)
  {
    printf(" Ace\n");
    strcat(buffer," Ace\n");
  }
  else if (aCard.value == 11)
  {
    printf(" Jack\n");
    strcat(buffer," Jack\n");
  }
  else if (aCard.value == 12)
  {
    printf(" Queen\n");
    strcat(buffer," Queen\n");
  }
  else if (aCard.value == 13)
  {
    printf(" King\n");
    strcat(buffer," King\n");
  }
  else
  {
    //if the card is a numeric value only a number is displayed algon the type of card
    printf(" %d\n",aCard.value);
    sprintf(helpingBuffer," %d\n",aCard.value);
    strcat(buffer,helpingBuffer);
  }
}

//function that draws the dealers hand
int dealerDraws(struct card * deck, int dealersHandValue, char * buffer,int clientId, struct card dealersOpenCard)
{
  //variables for the random function are set
  time_t t;
  int randomCard;
  //card structures used for the hand
  struct card firstCard = dealersOpenCard;
  struct card secondCard;
  struct card otherCard;
  //current hand value
  int currentPlay = 0;

  //a random number is created to get a random card from the deck
  srand((unsigned) time(&t));
  randomCard = rand() % 51;

  //the second card is obtained from the deck
  secondCard = deck[randomCard];

  //the two drawn cards are shown on the terminal
  printf("\n\nFirst two hands on the server for game against client %d: \n",clientId);
  printCardValues(firstCard,buffer);
  printCardValues(secondCard,buffer);

  //if the card is a figure (Jack,Queen or King) its value is set to 10
  if (firstCard.value >= 10)
  {
    firstCard.value = 10;
  }

  //if the card is a figure (Jack,Queen or King) its value is set to 10
  if (secondCard.value >= 10)
  {
    secondCard.value = 10;
  }

  //if the card is an Ace its value is set automatically to eleven due to Blackjack rules regarding the dealer
  if (firstCard.value == 1)
  {
    firstCard.value = 11;
  }

  //if the card is an Ace its value is set automatically to eleven due to Blackjack rules regarding the dealer
  if (secondCard.value == 1)
  {
    secondCard.value = 11;
  }

  //the hand value is calculate by adding the two card values
  currentPlay = firstCard.value + secondCard.value;

  //accordign to the rules of blackjack the dealer must draw until it's handl value is greater or equal to 17
  while(currentPlay <= 17)
  {
    //a random number is created to get a random card from the deck
    randomCard = rand() % 51;
    //a new card is taken from the deck
    otherCard = deck[randomCard];
    //the new card is printed on the terminal
    printCardValues(otherCard,buffer);
    //if the card is a figure (Jack,Queen or King) its value is set to 10
    if (otherCard.value >= 10)
    {
      otherCard.value = 10;
    }
    //if the card is an Ace its value is set automatically to eleven due to Blackjack rules regarding the dealer
    if (otherCard.value == 1)
    {
      otherCard.value = 11;
    }
    //the new card is added tu the current value of the hand
    currentPlay += otherCard.value;
  }

  //the dealers hand is returned to the server with its value and number of cards
  dealersHandValue = currentPlay;
  return dealersHandValue;
}

//function that gets the player's score
int getPlayerScore(int firstCard, int secondCard)
{
  //variables set used to handle the current value of the hand and the player's decision regarding the Ace
  int currentPlay = 0;
  int choice = 0;
  char buffer[256];

  //if the card is a figure (Jack,Queen or King) its value is set to 10
  if (firstCard >= 10)
  {
    firstCard = 10;
  }

  //if the card is an Ace the player chooses ig it acts as an 11 or a 1
  if (firstCard == 1)
  {
    //while structure that prevents the player from writing someting wrong
    while (choice != 1 && choice != 11)
    {
      printf("Would you like to use your Ace as a 1 or as an 11?");
      fgets(buffer,256,stdin);
      sscanf(buffer,"%d",&choice);
    }
    firstCard = choice;
  }

  choice = 0;

  //if the card is a figure (Jack,Queen or King) its value is set to 10
  if (secondCard >= 10)
  {
    secondCard = 10;
  }

  //if the card is an Ace the player chooses ig it acts as an 11 or a 1
  if (secondCard == 1)
  {
    //while structure that prevents the player from writing someting wrong
    while (choice != 1 && choice != 11)
    {
      printf("Would you like to use your Ace as a 1 or as an 11?");
      fgets(buffer,256,stdin);
      sscanf(buffer,"%d",&choice);
    }
    secondCard = choice;
  }

  //the players hand value is calculated from the first two drawn hands
  currentPlay = firstCard + secondCard;

  //the function returns the current hand value to the client
  return currentPlay;
}

//function that handles when the player wants another card
int playerHits(int handValue)
{
  //variable set to handle the buffer, the hand value, the new cards the player will get, and the random variables
  //to get a new card from the deck
  char buffer[256];
  int currentPlay = handValue;
  struct card otherCard;
  time_t t;
  int randomCard;
  int rc = 1; //variable used to validate that the user write the correct message
  int choice = 0;

  //a deck is created allocating memory for 52 cards
  struct card * deck = (struct card *) malloc(sizeof(struct card *)*52);

  //the deck is filled in this function
  createDeck(deck);
  srand((unsigned) time(&t));

  //the player continues to hit until he reaches 21,goes over 21, or decides to stand
  while(currentPlay <= 21 && rc != 0)
  {
    //random number is created to get a new card
    randomCard = rand() % 51;
    //new card is taken from the deck
    otherCard = deck[randomCard];
    //the new card is shown on terminal
    printf("You draw this card: \n");
    printCardValues(otherCard,buffer);

    //if the card is a figure (Jack,Queen or King) its value is set to 10
    if (otherCard.value >= 10)
    {
      otherCard.value = 10;
    }
    //if the card is an Ace the player chooses ig it acts as an 11 or a 1
    if (otherCard.value == 1)
    {
      //while structure that prevents the player from writing someting wrong
      while (choice != 1 && choice != 11)
      {
        printf("\n\nWould you like to use your Ace as a 1 or as an 11?: ");
        fgets(buffer,256,stdin);
        sscanf(buffer,"%d",&choice);
      }
      otherCard.value = choice;
      choice = 0;
    }

    //the new card value is added to the value of the hand
    currentPlay += otherCard.value;

    //if the player goes over 21 he has lost
    if (currentPlay > 21)
    {
      printf("\n\nSorry you've gone over 21, your score is: %d\n",currentPlay);
    }
    //if the player reaches 21 then its the dealers turn
    else if (currentPlay == 21)
    {
      printf("\n\nYour current score is now: %d\n",currentPlay);
      rc = 0;
    }
    //the player can hit again if he wishes
    else
    {
      printf("\n\nYour current score is now: %d\n",currentPlay);
      printf("\n\nWould you like to hit it again? (type stand if not, type hit on the contrary)\n");
      fgets(buffer, 256, stdin);
      //strncmp validates the user wrote stand if he wrote it then rc = 0 and the whiles ends, if he didnt then the
      //process of hitting cards continues one more time
      rc = strncmp(buffer,"stand",5);
    }
  }

  //the created deck memory is freed
  free (deck);

  //the players hand value is returned to the server
  return currentPlay;
}

//function that gets the winner according to the game results
void getWinner (int playerHandValue, int dealerHandValue,char * buffer,int playerBet)
{
  //the function appends the result to the buffer which includes the dealers finished hand so the
  //server cand display the results and send them to the client for him to see them

  //buffer used as a secondary buffer to paste next to the current buffer
  char helpingBuffer[200];

  if (playerHandValue == 21 && dealerHandValue != 21)
  {
    sprintf(helpingBuffer,"Result: Player got 21 and wins. You win $%f.",(float)playerBet + (float)playerBet*1.5);
    strcat(buffer,helpingBuffer);
  }
  else if ((playerHandValue != 21 && dealerHandValue == 21))
  {
    sprintf(helpingBuffer,"Result: Dealer got 21. You loose $%d.",playerBet);
    strcat(buffer,helpingBuffer);
  }
  else if (playerHandValue > 21)
  {
    sprintf(helpingBuffer,"Result: Player went over, dealer wins. You loose $%d.",playerBet);
    strcat(buffer,helpingBuffer);
  }
  else if (dealerHandValue > 21)
  {
    sprintf(helpingBuffer,"Result: Dealer went over, player wins. You win $%d",playerBet*2);
    strcat(buffer,helpingBuffer);
  }
  else if (playerHandValue > dealerHandValue)
  {
    sprintf(helpingBuffer,"Result: Player wins by higher score. You win $%d",playerBet*2);
    strcat(buffer,helpingBuffer);
  }
  else if (playerHandValue < dealerHandValue)
  {
    sprintf(helpingBuffer,"Result: Dealer wins by higher score. You loose $%d.",playerBet);
    strcat(buffer,helpingBuffer);
  }
  else if (playerHandValue == dealerHandValue)
  {
    strcat(buffer,"Result: Tie. You keep your bet.");
  }
}
