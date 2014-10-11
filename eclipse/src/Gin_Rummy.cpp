//============================================================================
// Name        : Gin_Rummy.cpp
// Author      : Justin Thibodeau
// Version     : 1.0
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "display.h"
#include <signal.h>
#include <ncurses.h>
#include <cstdlib>
#include <sstream>

using namespace std;

/* TODO Move function declarations to a header file */

// Signal Subroutine for Window Resize
static void detectResize (int sig);
// stub artifact for what the game does when the screen resizes
void stub_PrintResize(void);
// moved set of sample commands
void sampleDisplayActions(char key);

// The gameDisplay object is global, because the static signal handler object
// needs to access the dynamic object.
display gameDisplay;

/*
* This is the main function that starts the driver artifact.
*/
int main(int argc, char* argv[])
{
	// vars
	char key;
	stringstream messageString;
	//enumerable for game state
	//players
	//whose turn
	//deck
	//discard pile

	// enable a interrupt triggered on a window resize
	signal(SIGWINCH, detectResize); // enable the window resize signal

	//initialize display
		//draw start game button

	// infinite loop for the main game, press ctrl-c to quit
	for (;;) {
		// calls the game display to get input
    	key = gameDisplay.captureInput();

    	//outside of game
    		//draw "start game" button

    		//if "start game" button pushed
    			//prompt for name

    		//if name entered
    			//go to in-game

		//inside of game
    		//draw deck
    		//draw discard cards
    		//draw player cards

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
					//draw knock button
    				//if click knock button
    					//go to knock phase
    				//if click 1st player card
    					//highlight it
    				//if click discard card
    					//discard highlighted card and go to next player

    			//if knock phase
    				//draw player cards
    				//draw 3 slots for combos
    				//draw other players played combos
    				//draw "done" button
    				//draw "cancel" button

    				//if click 1st player card
    					//highlight it
    				//if click combo box
    					//if highlighted card can be combo'd with this box
    						//put it in this combo and remove from player cards
    					//else
    						//write "cards don't combo"
    				//if click "done" button
    					//if deadwood ok
    						//go to next player in knock turn
    					//else
    						//write "too much deadwood"
    				//if click "cancel" button
    					//move cards from any combos to players cards
    					//go to discard phase

    		//if ai turn
    			//execute ai code
	}

	return 0;
}

/*
 * This function demonstrates some of the abilities of the Display class
 */
void sampleDisplayActions(char key){
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
	stub_PrintResize();
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
