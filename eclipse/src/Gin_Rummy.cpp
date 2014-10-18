//============================================================================
// Name        : Gin_Rummy.cpp
// Author      : Justin Thibodeau
// Version     : 1.0
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "display.h"
#include "Key.h"
#include "Point.h"
#include "CardSlot.h"
#include <signal.h>
#include <ncurses.h>
#include <math.h>
#include <cstdlib>
#include <sstream>
using namespace std;

/* TODO Move vars and function declarations to a header file */

// client vars
char key;
//enumerable for game state
static const int OUT_GAME = 0;
static const int IN_GAME = 1;
static const int ENTER_NAME = 2;
int GAME_STATE = OUT_GAME;
string playerName="";

//game vars
//players
//whose turn
//deck
//discard pile

//display vars
display gameDisplay;
//input keys
Key startKey('n', "New Game");
Key knockKey('k', "Knock");
Key submitKey('s', "Submit");
Key cancelKey('c', "Cancel");
Key quitKey('q', "Quit");
//positions to display cards
const int NUMBERCARDSLOTS = 18;
CardSlot cardSlots[NUMBERCARDSLOTS];		//1 deck, 1 discard pile, 6 combo piles, 10 player cards
//banner messages to display controls and info
string startMessage = "Gin Rummy - "+startKey.toString()+"\t"+quitKey.toString();
string drawMessage = "Gin Rummy - Your Turn - Draw - "+quitKey.toString();
string discardMessage = "Gin Rummy - Your Turn - "+knockKey.toString()+"\t"+quitKey.toString();
string knockMessage = "Gin Rummy - Your Turn - Knock - "+submitKey.toString()+"\t"+cancelKey.toString()+"\t"+quitKey.toString();
string notTurnMessage = "Gin Rummy - Opponent's Turn - "+quitKey.toString();
string nameMessage = "Enter your name: ";

// Signal Subroutine for Window Resize
static void detectResize (int sig);
// stub artifact for what the game does when the screen resizes
void stub_PrintResize(void);
// sets the text of the displays topBanner
void setTopBanner(string s);
// sets the text of the displays bottomBanner
void setBottomBanner(string s);
//main game loop
void gameLoop();
//main draw methid
void draw();
//draw cards and other things
void drawCards();
//find a cardSlot at a given point
CardSlot *findCardSlot(int x, int y);
// moved set of sample commands
void sampleDisplay(char key);

/*
* This is the main function that starts the driver artifact.
*/
int main(int argc, char* argv[])
{
	int numCols = (gameDisplay.getCols()-2)/8;
	cardSlots[0] = CardSlot(2,2,0,0,CardSlot::deck);		//1 deck
	cardSlots[1] = CardSlot(10,2,0,0,CardSlot::discard);		//1 discard pile
	for(int i=0;i<10;i++){
		cardSlots[i+2] = CardSlot(2+8*(i%numCols),12+(10*(int)(i/numCols)), 6, 5, CardSlot::player);	//10 player cards
	}
	for(int i=0;i<6;i++){
		cardSlots[i+12] = CardSlot(2+8*(i%10),7+5*(i/10), 6, 5, CardSlot::combo);
	}

	// enable a interrupt triggered on a window resize
	signal(SIGWINCH, detectResize); // enable the window resize signal

	// infinite loop for the main game, press ctrl-c to quit
	for (;;) {
		gameLoop();
		draw();
	}

	return 0;
}

void gameLoop(){
	// calls the game display to get input
	char key = gameDisplay.captureInput();

	//if quitKey pressed
	if(key == quitKey.key())
		std::exit(0);

	switch(GAME_STATE){
	case OUT_GAME:
		//if startKey pushed
		if(key == startKey.key()){
			//go to ENTER_NAME state
			GAME_STATE = ENTER_NAME;
		}
		break;
	case ENTER_NAME:
		//if enter key pressed
		if(key == '\n')
			GAME_STATE = IN_GAME;
		//if delete key pressed
		else if((key == 7 || key == 74) && playerName.size() > 0)
			playerName.erase (playerName.end() - 1);
		//use key as next name letter
		else if(isalnum(key))//key > 31 && key < 127)
			playerName = playerName + key;
		break;
	case IN_GAME:
		//if mouseClick
		if(key == -1){
			//get card clicked if any
			findCardSlot(gameDisplay.getMouseEventX(), gameDisplay.getMouseEventY());
		}
		//if player turn
			//if click 1st player card
				//highlight it
			//if click 2nd player card
				//swap with highlighted card

			//if in draw phase
				//if player clicks deck
					//give next deck card and go to discard phase
				//if player clicks discarded card
					//give that card and go to discard phase

			//if in discard phase
				//if knockKey pressed
					//go to knock phase
				//if click 1st player card
					//highlight it
				//if click discard card
					//discard highlighted card and go to next player

			//if knock phase
				//draw player cards
				//draw 3 slots for combos
				//draw other players played combos

				//if click 1st player card
					//highlight it
				//if click combo box
					//if highlighted card can be combo'd with this box
						//put it in this combo and remove from player cards
					//else
						//write "cards don't combo"
				//if doneKey pressed
					//if deadwood ok
						//go to next player in knock turn
					//else
						//write "too much deadwood"
				//if cancelKey pressed
					//move cards from any combos to players cards
					//go to discard phase

		//if ai turn
			//execute ai code
		break;
	}

	//draw small box in a corner to get rid of the annoying cursor
	//gameDisplay.drawBox(0,1,1,1,0);
}

void draw(){
	//erase the canvas
	gameDisplay.eraseBox(0,0,100000,1000000);
	gameDisplay.fillBackground();

	//draw based on game state
	switch(GAME_STATE){
	case OUT_GAME:
		//write startMessage
		setTopBanner(startMessage);
		break;
	case ENTER_NAME:
		//write startMessage
		setTopBanner(startMessage);
		//prompt for name
		setBottomBanner(nameMessage+playerName);
		break;
	case IN_GAME:
		//if player turn
		//switch(curPlayer.phase){
			//if in draw phase
			//case(Player::draw):
				//write drawMessage
				setTopBanner(drawMessage);

			//if in discard phase
			//case(Player::discard):
				//write discardMessage
				setTopBanner(discardMessage);

			//if knock phase
			//case(Player::knock):
				//write knockMessage
				setTopBanner(knockMessage);
				//if click combo box
					//if highlighted card can't be combo'd with this box
						//write "cards don't combo"
				//if doneKey pressed
					//if deadwood not ok
						//write "too much deadwood"

		//if ai turn
			//write notTurnMessage
			setTopBanner(notTurnMessage);

		drawCards();
		break;
	}
}

void drawCards(){
	//draw all cards/slots
	for(int i=0;i<NUMBERCARDSLOTS;i++){
		//draw highlight
		if(cardSlots[i].highlighted())
			gameDisplay.drawBox(cardSlots[i].position().x()-1, cardSlots[i].position().y()-1,
					cardSlots[i].width()+2,cardSlots[i].height()+2,0);

		//draw cards
		switch (cardSlots[i].type()){
		case (CardSlot::deck):
				gameDisplay.displayCard(cardSlots[i].position().x(),cardSlots[i].position().y(),0,0,0);
				break;
		case (CardSlot::discard):
				//if not knock phase
				gameDisplay.displayCard(cardSlots[i].position().x(),cardSlots[i].position().y(),1,1,0);
				break;
		case (CardSlot::player):
				gameDisplay.displayCard(cardSlots[i].position().x(),cardSlots[i].position().y(),2,2,0);
				break;
		case (CardSlot::combo):
				//if knock phase
				gameDisplay.displayCard(cardSlots[i].position().x(),cardSlots[i].position().y(),3,3,0);
				break;
		}
	}
}

CardSlot *findCardSlot(int x, int y){
	//iterate over card slots
	int minX,minY,maxX,maxY;
	for(int i=0;i<18;i++){
		minX=cardSlots[i].position().x();
		maxX=minX+cardSlots[i].width();
		minY=cardSlots[i].position().y();
		maxY=minY+cardSlots[i].height();
		//set highlighted
		if(x>=minX && x<=maxX && y>=minY && y<=maxY){
			cardSlots[i].setHighlight(!cardSlots[i].highlighted());
		}
	}
	return NULL;
}

void setTopBanner(string s){
	stringstream messageString;
	messageString.str("");
	messageString << s;
	gameDisplay.bannerTop(messageString.str());
}
void setBottomBanner(string s){
	stringstream messageString;
	messageString.str("");
	messageString << s;
	gameDisplay.bannerBottom(messageString.str());
}

/*
 * This is the interrupt service routine called when the resize screen
 * signal is captured.
 */
void detectResize(int sig) {
	// update the display class information with the new window size
    gameDisplay.handleResize(sig);
	// re-enable the interrupt for a window resize
    signal(SIGWINCH, detectResize);
	/*INSERT YOUR OWN SCREEN UPDATE CODE instead of stub_PrintResize*/

}

/*
 * This is a simple stub that should be replaced with what the game does
 * when the screen resizes.
 */
void stub_PrintResize(void) {
	// gets the new screen size
	int cols = gameDisplay.getCols();
	int lines = gameDisplay.getLines();
	// setups a message stream
	stringstream messageString;
	messageString << "Terminal is " << cols << "x" << lines;
	// prints out the information of the new screen size in a top banner
	gameDisplay.bannerTop(messageString.str());
}
/*
 * This function demonstrates some of the abilities of the Display class
 */
void sampleDisplay(char key){
	// using a stringstream rather than a string to make making the banner easier
	stringstream messageString;

	int cardX = 0;
	int cardY = 0;
	int suit = 0;
	int number = 0;

	int dragX = 0;
	int dragY = 0;

	// if a mouse event occurred
	if (key == -1) {
		// make a banner message
		messageString.str("");
		messageString << "A mouse event occurred x=" \
			<< gameDisplay.getMouseEventX() << ", y=" \
			<< gameDisplay.getMouseEventY() << ", bstate=" \
			<< gameDisplay.getMouseEventButton();
		// display a banner message
		gameDisplay.bannerTop(messageString.str());
		// record the location of the mouse event
		cardX = gameDisplay.getMouseEventX();
		cardY = gameDisplay.getMouseEventY();
		// Some of the mouse click values are defined in display.h
		// check if it was a left click
		if (gameDisplay.getMouseEventButton()&LEFT_CLICK) {
			// draw a random card at the click location
			suit = rand()%5;
			number = rand()%15;
			gameDisplay.displayCard(cardX,cardY,suit,number, A_BOLD);
		// check if it was a right click
		} else if (gameDisplay.getMouseEventButton()&RIGHT_CLICK) {
			// erase a portion of the screen in the shape of a card
			gameDisplay.eraseBox(cardX,cardY,6,5);
		// check for the start of a drag click
		} else if (gameDisplay.getMouseEventButton()&LEFT_DOWN) {
			// record start of the drag
			dragX = cardX;
			dragY = cardY;
		// when the mouse is released
		} else if (gameDisplay.getMouseEventButton()&LEFT_UP) {
			// calculate size of the drag
			int sizeX = abs(dragX-cardX);
			int sizeY = abs(dragY-cardY);
			// get to the top left corner of the drag area
			if (dragX > cardX)
				dragX = cardX;
			if (dragY > cardY)
				dragY = cardY;
			// draw a box around the drag area
			gameDisplay.drawBox(dragX, dragY, sizeX, sizeY, 0);
		}
	// if a key was pressed
	} else if(key > 0) {
		// make bottom a banner message saying that a key was pressed
		messageString.str("");
		messageString << "Key " << key << " pressed";
		gameDisplay.bannerBottom(messageString.str());
	}
}
