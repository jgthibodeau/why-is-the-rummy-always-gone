Gin Rummayyy

Authors:
Michael Chapman
Justin Thibodeau
Zachary Windoloski

*********************
How to Start/Compile:
	Run './Compile_Play' to compile the 'Play' executable and run it.
	To rerun, you only need to run './Play'

************
How to Play:
	For full game rules, please visit http://en.wikipedia.org/wiki/Gin_rummy

	Contextual game controls are shown in the top banner as the game progresses.
	At any point, press 'q' to quit

	Press 'n' for a new game and enter your username, this will show up in the bottom banner as you type
	Press enter to submit your name and begin a game

	While in a game:
		The top banner will alert you when it is your turn, and what contextual controls are available
		The deck is the card that says 'Draw' in the top left corner
		The discard pile is the card directly to the right of the deck (This will be an empty box if the discard pile is empty)
		There are 11 slots on the bottom of the screen where your cards will appear

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
				Repeat this for each card you want to add
			When you have played all of your combos:
				Drag one card to the discard pile
				If all of your combos are valid, and your remaining cards sum to less than 10 you have won
				Otherwise, you will be told about any combos that are invalid or if you have too much remaining 'deadwood'
			If you would like to cancel knocking, press the 'c' key

************
How to Test:
	Run './testAll'
	This command will compile and run all of our tests located in the test.cpp file, each of which tests class methods, and run the resulting executable
