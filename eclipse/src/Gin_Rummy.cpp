//============================================================================
// Name        : Gin_Rummy.cpp
// Author      : Justin Thibodeau
// Version     : 1.0
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "display.h"
#include <signal.h>
#include <ncurses.h>
#include <math.h>
#include <cstdlib>
#include <sstream>

using namespace std;

/* TODO Move function declarations to a header file */

// Signal Subroutine for Window Resize
static void detectResize (int sig);
// stub artifact for what the game does when the screen resizes
void stub_PrintResize(void);
// sets the text of the displays topBanner
void setTopBanner(string s);
// sets the text of the displays bottomBanner
void setBottomBanner(string s);
// moved set of sample commands
void sampleDisplay(char key);

// The gameDisplay object is global, because the static signal handler object
// needs to access the dynamic object.
display gameDisplay;

/* Point class */
class Point {
private:
	double xval, yval;
public:
	// Constructor uses default arguments to allow calling with zero, one,
	// or two values.
	Point(double x = 0.0, double y = 0.0) {
		xval = x;
		yval = y;
	}

	// Extractors.
	double x() { return xval; }
	double y() { return yval; }

	// Distance to another point.  Pythagorean thm.
	double dist(Point other) {
		double xd = xval - other.xval;
		double yd = yval - other.yval;
		return sqrt(xd*xd + yd*yd);
	}

	// Add or subtract two points.
	Point add(Point b){
		return Point(xval + b.xval, yval + b.yval);
	}
	Point sub(Point b){
		return Point(xval - b.xval, yval - b.yval);
	}

	// Move the existing point.
	void move(double a, double b){
		xval += a;
		yval += b;
	}
};

/* CardSlot class */
class CardSlot {
private:
	Point positionPoint;
	int widthVal;
	int heightVal;
	//Card card;
public:
	// Constructor uses default arguments to allow calling with zero, one,
	// or two values.
	CardSlot(double x = 0.0, double y = 0.0, int width = 0, int height = 0) {
		positionPoint.move(x,y);
		widthVal = width;
		heightVal = height;
	}

	// Extractors.
	Point position() { return positionPoint; }
	double width() { return widthVal; }
	double height() { return heightVal; }

	// Move the existing button.
	void move(double a, double b){
		positionPoint.move(a,b);
	}

	//set card
	//remove card
};

/* Key class */
class Key {
private:
	char keyVal;
	string textVal;
public:
	// Constructor uses default arguments to allow calling with zero, one,
	// or two values.
	Key(char k = ' ', string text = "") {
		keyVal = k;
		textVal = text;
	}

	// Extractors.
	char key() { return keyVal; }
	string text() { return textVal; }
	string toString() { return textVal+": "+keyVal; }

	// Setters
	void setKey(char c){
		keyVal = c;
	}
	void setText(string s){
		textVal = s;
	}
};

/*
* This is the main function that starts the driver artifact.
*/
int main(int argc, char* argv[])
{
	// client vars
	char key;
	//enumerable for game state
	int OUT_GAME = 0;
	int IN_GAME = 1;
	int ENTER_NAME = 2;
	int GAME_STATE = OUT_GAME;
	string playerName="";

	//game vars
	//players
	//whose turn
	//deck
	//discard pile

	//display vars
	Key startKey('n', "New Game");
	Key knockKey('k', "Knock");
	Key submitKey('s', "Submit");
	Key cancelKey('c', "Cancel");
	Key quitKey('q', "Quit");
	string startMessage = startKey.toString()+"\t"+quitKey.toString();
	string drawMessage = quitKey.toString();
	string discardMessage = knockKey.toString()+"\t"+quitKey.toString();
	string knockMessage = submitKey.toString()+"\t"+cancelKey.toString()+"\t"+quitKey.toString();
	string notTurnMessage = quitKey.toString();
	string nameMessage = "Enter your name: ";
	CardSlot deckSlot(0,0,0,0);
	//discard slots
	//player card slots
	//combo slots

	// enable a interrupt triggered on a window resize
	signal(SIGWINCH, detectResize); // enable the window resize signal

	//initialize display
		//draw start game button

	// infinite loop for the main game, press ctrl-c to quit
	for (;;) {
		// calls the game display to get input
    	key = gameDisplay.captureInput();

    	//erase the canvas
		gameDisplay.eraseBox(0,0,100000,1000000);

		//x, y, width, height, formatting
    	//gameDisplay.drawBox(60, 10, 20, 20, 0);
		//x, y, suit, num, formatting
		//gameDisplay.displayCard(60, 10, 1, 1, 0);
		int x = rand()%100;
		int y = rand()%20;
		int suit = rand()%5;
		int number = rand()%15;
		gameDisplay.displayCard(x, y, suit, number, A_BOLD);

    	//outside of game
		if(GAME_STATE == OUT_GAME){
    		//write startMessage
    		setTopBanner(startMessage);

    		//if startKey pushed
    		if(key == startKey.key()){
    			//prompt for name
    			setBottomBanner(nameMessage);
    			//go to ENTER_NAME state
    			GAME_STATE = ENTER_NAME;
    		}


		}
		else if(GAME_STATE == ENTER_NAME){
			//if enter key pressed
			if(key == '\n')
				//go to in-game
				GAME_STATE = IN_GAME;
			//use key as next name letter
			else if(key > 0){
				playerName = playerName + key;
			}

			//prompt for name
			setBottomBanner(nameMessage+playerName);
		}
		//inside of game
		else if(GAME_STATE == IN_GAME){
    		//draw deck
    		//draw discard cards
    		//draw player cards

    		//if player turn
    			//if click 1st player card
    				//highlight it
    			//if click 2nd player card
    				//swap with highlighted card

    			//if in draw phase
    				//write drawMessage
    				//if player clicks deck
    					//give next deck card and go to discard phase
    				//if player clicks discarded card
    					//give that card and go to discard phase

    			//if in discard phase
					//write discardMessage
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
    				//write knockMessage

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
    			//write notTurnMessage
    			//execute ai code
		}
    	//draw small box in a corner to get rid of the annoying cursor
		gameDisplay.drawBox(0,1,1,1,0);
	}

	return 0;
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
