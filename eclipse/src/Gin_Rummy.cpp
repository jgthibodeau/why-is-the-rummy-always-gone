//============================================================================
// Name        : Gin_Rummy.cpp
// Author      : Justin Thibodeau
// Version     : 1.0
//============================================================================

#include "display.h"
#include "Key.h"
#include "Point.h"
#include "CardSlot.h"
#include "Card.h"
#include "Deck.h"
#include "DiscardPile.h"
#include "Player.h"
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
Player player1, player2;
Player* curPlayer;
Deck deck;
Card cardBack(0,0,0);
DiscardPile discardPile;

//display vars
display gameDisplay;
//input keys
Key startKey('n', "New Game");
Key knockKey('k', "Knock");
Key submitKey('s', "Submit");
Key cancelKey('c', "Cancel");
Key quitKey('q', "Quit");
//positions to display cards
const int NUMBERCARDSLOTS = 19;
CardSlot cardSlots[NUMBERCARDSLOTS];	//1 deck, 1 discard pile, 6 combo piles, 10 player cards
CardSlot *selectedSlots[2];				//used for selecting specific cards
//banner messages to display controls and info
string startMessage = "Gin Rummy - "+startKey.toString()+"\t"+quitKey.toString();
string drawMessage = "Gin Rummy - Your Turn - Draw - "+quitKey.toString();
string playMessage = "Gin Rummy - Your Turn - "+knockKey.toString()+"\t"+quitKey.toString();
string knockMessage = "Gin Rummy - Your Turn - Knock - "+submitKey.toString()+"\t"+cancelKey.toString()+"\t"+quitKey.toString();
string notTurnMessage = "Gin Rummy - Opponent's Turn - "+quitKey.toString();
string nameMessage = "Enter your name: ";
string dontComboMessage = "Those cards don't combo!";
string badDeadwoodMessage = "You have too much remaining deadwood!";
string topBanner,bottomBanner;

// Signal Subroutine for Window Resize
static void detectResize (int sig);
//main game loop
void gameLoop();
//main draw method
void draw();
//draw cards
void drawCards();
//draw the displays banners
void drawBanners();
//find a cardSlot at a given point
CardSlot *findCardSlot(int x, int y);
//reset highlights and selected cards
void resetSelectedSlots();
//reset cardSlot positions based on window size
void resizeCardSlots();

int main(int argc, char* argv[])
{
	//initialize cardSlots for display
	resizeCardSlots();

	// enable a interrupt triggered on a window resize
	signal(SIGWINCH, detectResize); // enable the window resize signal

	// infinite loop for the main game
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

	//regular game
	switch(GAME_STATE){
	case OUT_GAME:
		topBanner = startMessage;
		//if startKey pushed
		if(key == startKey.key()){
			//go to ENTER_NAME state
			GAME_STATE = ENTER_NAME;
		}
		break;
	case ENTER_NAME:
		//write startMessage
		topBanner = startMessage;
		//prompt for name
		bottomBanner = nameMessage+playerName;

		//if enter key pressed
		if(key == '\n'){
			GAME_STATE = IN_GAME;
			bottomBanner = "";
			//initialize stuff
			deck.initialize();
			deck.shuffle();
			discardPile.initialize();
			//Deal player cards
			for(int i=0;i<10;i++){
				//TODO player1.addCard(deck.drawCard());
			}
			for(int i=0;i<10;i++){
				//TODO player2.addCard(deck.drawCard());
			}
			discardPile.addCard(deck.drawCard());
		}

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
			CardSlot *temp = findCardSlot(gameDisplay.getMouseEventX(), gameDisplay.getMouseEventY());
			if(selectedSlots[0] == NULL)
				selectedSlots[0] = temp;
			else
				selectedSlots[1] = temp;
		}
		//if player turn
		//TODO if(curPlayer == player1){
			//allow player to swap cards around
			//if 1st selected is player card
			if(selectedSlots[0] != NULL && (*selectedSlots[0]).type() == CardSlot::player){
				//highlight it
				(*selectedSlots[0]).setHighlight(true);
				//if 2nd selected is player cards
				if(selectedSlots[1] != NULL && (*selectedSlots[1]).type() == CardSlot::player){
					//swap and unhighlight
					//TODO Card temp = curPlayer.cards[(*selectedSlots[0]).index()];
					//TODO curPlayer.setCard[(*selectedSlots[0]).index(), index] = curPlayer.cards[(*selectedSlots[1]).index()]
					//TODO curPlayer.cards[(*selectedSlots[1]).index()] = temp;
					resetSelectedSlots();
				}
			}
			//player phases
			//TODO switch(curPlayer.phase){
				//if in draw phase
				//TODO case(Player::draw):
					topBanner = drawMessage;

					//if player click deck
					if((selectedSlots[0] != NULL && (*selectedSlots[0]).type() == CardSlot::deck) ||
							(selectedSlots[1] != NULL && (*selectedSlots[1]).type() == CardSlot::deck)){
						//TODO give next deck card and go to discard phase
						resetSelectedSlots();

					}
					//if player clicks discarded card
					if((selectedSlots[0] != NULL && (*selectedSlots[0]).type() == CardSlot::discard) ||
							(selectedSlots[1] != NULL && (*selectedSlots[1]).type() == CardSlot::discard)){
						//TODO give that card and go to discard phase
						resetSelectedSlots();
					}

				//if in play phase
				//TODO case(Player::play):
					topBanner = playMessage;

					//if knockKey pressed
					if(key == knockKey.key()){
						//TODO curPlayer.phase = knock;
					}
					if(selectedSlots[0] != NULL){
						//if player card is 1st selected selected
						if((*selectedSlots[0]).type() == CardSlot::player){
							(*selectedSlots[0]).setHighlight(true);
							//if 2nd selected is discard
							if((*selectedSlots[0]).type() == CardSlot::discard){
								//discard selected and reset
								//TODO discardPile add selected
								resetSelectedSlots();
								//TODO go to next player
							}
						}
						//if not player card as 1st selected
						else
							//reset selected
							resetSelectedSlots();
					}

				//if knock phase
				//TODO case(Player::knock):
					topBanner = knockMessage;

					if(selectedSlots[0] != NULL){
						//if player card is 1st selected selected
						if((*selectedSlots[0]).type() == CardSlot::player){
							(*selectedSlots[0]).setHighlight(true);
							//if 2nd selected is combo
							if((*selectedSlots[0]).type() == CardSlot::combo){
								//TODO try to move this card into this combo
								//TODO if combo ok
									bottomBanner = "";
								//TODO else
									bottomBanner = dontComboMessage;
								resetSelectedSlots();
							}
						}
						//if not player card as 1st selected
						else
							//reset selected
							resetSelectedSlots();
					}
					//if submit key pressed
					if(key == submitKey.key()){
						//if deadwood ok
						//TODO check currentplayer deadwood
						//TODO if deadwood ok
							//TODO go to next player, set phase to last turn
							bottomBanner = "";
						//TODO else
							bottomBanner = badDeadwoodMessage;
					}
					//if cancelKey pressed
					if(key == cancelKey.key()){
						//TODO remove cards from any combos to curPlayers cards
						//TODO go to play phase
					}

				//if last turn phase
				//TODO case(Player::last):
					topBanner = knockMessage;

					if(selectedSlots[0] != NULL){
						//if player card is 1st selected selected
						if((*selectedSlots[0]).type() == CardSlot::player){
							(*selectedSlots[0]).setHighlight(true);
							//if 2nd selected is combo
							if((*selectedSlots[0]).type() == CardSlot::combo){
								//TODO try to move this card into this combo
								//TODO if combo ok
									bottomBanner = "";
								//TODO else
									bottomBanner = dontComboMessage;
								resetSelectedSlots();
							}
						}
						//if not player card as 1st selected
						else
							//reset selected
							resetSelectedSlots();
					}
					//if submit key pressed
					if(key == submitKey.key()){
						//TODO go to end game
					}
					//if cancelKey pressed
					if(key == cancelKey.key()){
						//TODO remove cards from any combos to curPlayers cards
					}
			//TODO end case

		//if ai turn
		//if(curPlayer == player2){
			topBanner = notTurnMessage;
			//execute ai code
		//}
		break;
	}

	//draw small box in a corner to get rid of the annoying cursor
	//gameDisplay.drawBox(0,1,1,1,0);
}

void draw(){
	//erase the canvas
	gameDisplay.eraseBox(0,0,100000,1000000);
	gameDisplay.fillBackground();
	drawBanners();
	if(GAME_STATE == IN_GAME)
		drawCards();
}

void drawCards(){
	//draw all cards/slots
	for(int i=0;i<NUMBERCARDSLOTS;i++){
		//draw highlight
		if(cardSlots[i].highlighted())
			gameDisplay.drawBox(cardSlots[i].position().x()-1, cardSlots[i].position().y()-1, cardSlots[i].width()+2,cardSlots[i].height()+2,0);

		//draw cards
		CardSlot slot = cardSlots[i];
		Card* card = NULL;
		Card c;
		bool display = true;
		switch (slot.type()){
		case (CardSlot::deck):
			card = &cardBack;
			break;
		case (CardSlot::discard):
			c = discardPile.topCard();
			card = &c;
			break;
		case (CardSlot::player):
			//TODO card = player1.cards[slot.index()];
			break;
		case (CardSlot::combo):
			//TODO if not knock phase or last turn phase
				display = false;
			//TODO card = combos[slot.index()].last;
			break;
		}
		if(display){
			if(card == NULL)
				gameDisplay.drawBox(cardSlots[i].position().x(),cardSlots[i].position().y(),6,5,0);
			else
				gameDisplay.displayCard(cardSlots[i].position().x(),cardSlots[i].position().y(),(*card).suit(),(*card).value(),0);
		}
	}
}

void drawBanners(){
	stringstream messageString;
	if(topBanner != ""){
		messageString.str("");
		messageString << topBanner;
		gameDisplay.bannerTop(messageString.str());
	}

	if(bottomBanner != ""){
		messageString.str("");
		messageString << bottomBanner;
		gameDisplay.bannerBottom(messageString.str());
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
		//return CardSlot if found
		if(x>=minX && x<=maxX && y>=minY && y<=maxY){
			return &cardSlots[i];
		}
	}
	return NULL;
}

void resetSelectedSlots(){
	(*selectedSlots[0]).setHighlight(false);
	(*selectedSlots[1]).setHighlight(false);
	selectedSlots[0] = NULL;
	selectedSlots[1] = NULL;
}

void resizeCardSlots(){
	int numCols = (gameDisplay.getCols()-2)/8;
	cardSlots[0] = CardSlot(2,2,0,0,CardSlot::deck);		//1 deck
	cardSlots[1] = CardSlot(10,2,0,0,CardSlot::discard);	//1 discard pile
	for(int i=0;i<11;i++){									//11 player cards
		cardSlots[i+2] = CardSlot(2+8*(i%numCols),12+(5*(int)(i/numCols)), 6, 5, CardSlot::player, i);
	}
	for(int i=0;i<6;i++){									//6 combo cards
		cardSlots[i+13] = CardSlot(2+8*(i%10),7+5*(i/10), 6, 5, CardSlot::combo, i);
	}
}

void detectResize(int sig) {
	// update the display class information with the new window size
    gameDisplay.handleResize(sig);
	// re-enable the interrupt for a window resize
    signal(SIGWINCH, detectResize);

    //resize positions for cardSlots
    resizeCardSlots();
}
