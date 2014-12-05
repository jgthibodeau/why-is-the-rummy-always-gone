Gin Rummy

Authors:
Michael Chapman - Player class
Justin Thibodeau - Display classes (Display, CardSlot, Point, Key) and integration
Zachary Windoloski - Card classes (Card, Deck, Discard Pile, Combo)

*********************
How to Start/Compile:
	Run './compile' to compile the server and client executables
	Run './server' to start the server
	Run './client' to start the client

************
How to Play:
	For full game rules, please visit http://en.wikipedia.org/wiki/Gin_rummy

	Contextual game controls are shown in the top banner as the game progresses.
	At any point, press 'x' to quit the client, but keep the current game alive on the server.
	Press 'q' to quit the client, and quit the current running game.

	To login: press Enter and type your username, this will show up in the bottom banner as you type.
	Press enter to submit your name, this will put you into the lobby.
	From the lobby, if the server is not currently running a game, press 1 to start a 1 Player game or 2 to start a 2 Player game.
	If a 2 Player game has been started, you can join by clicking 'j'.
	If the server is in the middle of a 1 or 2 Player game, you must wait until that game is done, or try again later.

	While in a game:
		The top banner will alert you when it is your turn, and what contextual controls are available
		The deck is the card that says 'Draw' in the top left corner
		The discard pile is the card directly to the right of the deck (This will be an empty box if the discard pile is empty)
		There are 11 slots on the bottom of the screen where your cards will appear

		To chat:
			Press the "t" key at any point to open the chat box, and type a message. Press Enter to send the message, and Esc to cancel the message. The most recent chat message and author are displayed in the bottom left above the bottom banner. 
		To draw:
			Click on either the deck or the discard pile (you can only draw from the discard pile if there are cards in it)
		To organize cards in your hand:
			Click on 2 cards in your hand and they will swap places
		To discard:
			Click on 1 card in your hand and then the discard pile
		To knock:
			Press the 'k' key during your turn, after you have drawn
		When knocking:
			6 slots will appear above your cards where you can put combos
			A combo must consist of at least 3 cards of the same number, or 3 cards of the same suit in a row
			To play a single combo:
				Click on one of your cards for the combo, and then on the combo slot you want to place it in
				If this card cannot be placed in this combo, you will keep the card and a message will appear on the bottom banner
				Add the next card for this combo to the same slot, and repeat until the combo is complete
			When you have played all of your combos:
				Drag one card to the discard pile
				If all of your combos are valid, and your remaining cards sum to less than 10 you have won
				Otherwise, you will be told about any combos that are invalid or if you have too much remaining 'deadwood'
			If you would like to cancel knocking, press the 'c' key
		If your opponent knocks first:
			Your opponent's combos will show up in the combo slots. You can add a card to their combos by clicking on the card you'd like to add, and then clicking on the combo slot. You can also start a new combo in an empty slot as you would if you were the knocker (see "When knocking").
			If you change your mind about a combo, you can press "c" to cancel and your cards will be returned. 
			Once you are satisfied, you can press "s" to submit. After you submit or your opponent, the game will end

	When a game has ended:
		Your score and your opponent's score will be displayed, as well as the winner of the game. 



*******************
Saving and Loading:
	The server will continuously save the current status of a game, allowing the game to resume if either the client or server experiences a crash.
	When in a game:
		If you 'Save and Exit' the client will close, but the server will keep the current game running.
		If you 'Quit' the client will close, and the server will end the current game allowing a new one to be started.

************
How to Test:
	Run './testAll'
	This command will compile and run all of our tests located in the test.cpp file, each of which tests class methods, and run the resulting executable

*****
Note:
	If you experience strange behavior, try using a different username
