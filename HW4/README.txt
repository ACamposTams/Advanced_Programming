To use the program one must open at least one terminal for the server and another one for the client.
If there are more clients there needs to be one terminal for each.

To start the server program one must first compile using the following line:
gcc -o server server.c cardHandler.c

Then to run the executable use the following line:
./server 8642

Once the server is running clients can connect to it.
To start the client one must first compile using the following line:
 gcc -o client client.c cardHandler.c

 Then to run the executable use the following line:
 ./client [server ip address] 8642
 For example:
 ./client 192.168.1.79 8642

 If you are using the same computer for server and client then your computer ip addres will work, if the server is in another you must use that computer's address.

 Once the client is running you will be prompted by a message that asks for your bet, insert a number and press enter.
 The program will show you your two first cards, your score and the first card the dealer has.
 In case one of your cards is an ace you will be asked to answer with a 1 or an 11 to determine its value.

 If you don't have an ace then the program will ask you if you want to hit or stand. Type either one of those words to follow that action.
 If you write someting different then you will be asked to write it again.

 If you hit then you will be shown your next card and your new score.
 In case it is an ace you will be asked to answer with a 1 or an 11 to determine its value.
 If you don't go over 21 then you'll be asked again if you want to hit or stand.
 If you want to hit type anything, but if you want to stand type stand.
 If you hit again you'll be shown the same message and follow the above steps again.

 If you decide to stand at any point, or you've gone over 21 then its time for the server/dealer to play.
 The terminal will show you the dealer's cards and score. Then the terminal will show the winner and how much money you win or loose in that case.

 The first message of the program (asking for a bet) will be shown again to start a new game, if you don't want to play anymore then press start to end the client.

 In the server side there is no input but the terminal shows information about each game, the information is preceeded by the number of connection this client has with the server.
