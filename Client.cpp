//============================================================================
// Name        : Gin_Rummy.cpp
// Author      : Justin Thibodeau
// Version     : 1.0
//============================================================================

/*
 * Stretch goals:
 * 	prevent discarding card that was drawn from discard pile
 *	better ai
 *	refactor code
 *	pull game logic code into player and etc.
 *	click and drag to insert player cards on ui
 */

#include <string>
#include <iostream>
#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>
#include <vector>

#include "display.h"
#include "Key.h"
#include "Point.h"
#include "CardSlot.h"
#include <signal.h>
#include <ncurses.h>
#include <math.h>
#include <cstdlib>
#include <sstream>
#include <algorithm>

//locally hosted server url
#define SERVERURL "http://localhost:8080/RPC2"

using namespace std;

/* TODO Move vars and function declarations to a header file */

xmlrpc_c::clientSimple client;

// client vars
char key;
//enumerable for game state
static const int OUT_GAME = 0;
static const int IN_GAME = 1;
static const int ENTER_NAME = 2;
static const int IRWIN = 3;
static const int STEVE = 4;
static const int LOBBY = 5;
static const int PRE_GAME = 6;
int GAME_STATE = OUT_GAME;
//enumberable for server state
static const int FULL = 0;
static const int WAITING = 1;
static const int EMPTY = 2;
string playerName="";
string answer="";
int turnPhase = -1;

//display vars
display gameDisplay;
//input keys
Key startKey('\n', "Login");
Key singleKey('1', "Start 1P Game");
Key multiKey('2', "Start 2P Game");
Key joinKey('j', "Join 2P Game");
Key loadKey('l', "Load Game");
Key knockKey('k', "Knock");
Key submitKey('s', "Submit");
Key cancelKey('c', "Cancel");
Key quitKey('q', "Quit");
//positions to display cards
const int NUMBERCARDSLOTS = 19;
CardSlot cardSlots[NUMBERCARDSLOTS];	//1 deck, 1 discard pile, 6 combo piles, 10 player cards
CardSlot *selectedSlots[2];				//used for selecting specific cards
//banner messages to display controls and info
//string startMessage = "Gin Rummy - "+startKey.toString()+"\t"+loadKey.toString()+"\t"+quitKey.toString();
string startMessage = "Gin Rummy - Login: Enter \t"+quitKey.toString();
string nameMessage = "Enter your name: ";
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
int findCardSlot(int x, int y);
//reset highlights and selected cards
void resetSelectedSlots();
//reset cardSlot positions based on window size
void resizeCardSlots();
//decipher the returned xml from polling the server
void decipherCards(xmlrpc_c::value cards);

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
	case IRWIN:
	{
		topBanner = "Professor Irwin? Prove it! What does an empty cement mixer truck weigh, measured in units of US dollar bills?";
		bottomBanner = "Answer: "+answer;
		if(key == '\n'){
			//answer 11.3 million
			int guess = atoi(answer.c_str());
			if(guess <= 100000000 && guess >= 1000000){
				cout << " Welcome home David... We've been expecting you..." << endl;
				system("python -mwebbrowser Secret");
			}
			else{
				bottomBanner = "You are very clearly not David Irwin";
				GAME_STATE = OUT_GAME;
			}
		}

		//if delete key pressed
		else if((key == 7 || key == 74) && playerName.size() > 0)
			answer.erase (answer.end() - 1);
		//use key as next name letter
		else if(isdigit(key))//key > 31 && key < 127)
			answer = answer + key;

		break;
	}
	case OUT_GAME:
	{
		topBanner = startMessage;
		playerName = "";
		answer = "";
		//if startKey pushed
		if(key == startKey.key()){
			//go to ENTER_NAME state
			GAME_STATE = ENTER_NAME;
		}
		break;
	}
	case ENTER_NAME:
	{
		//write startMessage
		topBanner = startMessage;
		//prompt for name
		bottomBanner = nameMessage+playerName;

		//if enter key pressed
		if(key == '\n'){
			string low = playerName;
			std::transform(low.begin(), low.end(), low.begin(), ::tolower);
			if(low == "david" || low == "irwin" || low == "davidirwin" || low == "dirwin"
					|| low == "profirwin" || low == "professorirwin"){
				GAME_STATE = IRWIN;
			}
			else if(low == "steveirwin"){
				system("python -mwebbrowser Secret2");
				GAME_STATE = OUT_GAME;
			}
			else{
				GAME_STATE = LOBBY;
				bottomBanner = "";
			}
		}

		//if delete key pressed
		else if((key == 7 || key == 74) && playerName.size() > 0)
			playerName.erase (playerName.end() - 1);
		//use key as next name letter
		else if(isalnum(key))//key > 31 && key < 127)
			playerName = playerName + key;
		break;
	}
	case LOBBY:
	{
		//TODO
		xmlrpc_c::value status;
		client.call(SERVERURL, "server.gameStatus", "s", &status, playerName.c_str());
		switch(xmlrpc_c::value_int(status)){
			//server is doing a game right now
			case FULL:
			{
				topBanner = "Game server is busy, please wait or try again later.";
				break;
			}
			//server is doing a 2player game and is waiting for player2
			case WAITING:
			{
				topBanner = joinKey.toString();
				break;
			}
			//server is doing nothing
			case EMPTY:
			{
				topBanner = singleKey.toString()+"\t"+multiKey.toString();
				break;
			}

		}

		//set top banner to:
		//		'Start 1P, Start 2P'
		//		'Join 2P'
		//		'Game already in session, please wait'
		//if singleKey pushed
			//send players name to the server
			//send a name for the cpu player
		//if multiKey pushed
			//send players name to the server
			//if there are 2 players, initialize the game
			//if there is 1 player, wait for second player
		//if loadKey pushed
			//load from the server with players name

		xmlrpc_c::value result;
		client.call(SERVERURL, "server.initialize", "s", &result, playerName.c_str());
		xmlrpc_c::value initialCards;
		client.call(SERVERURL, "server.respondToInput", "iis", &initialCards,' ',-1, playerName.c_str());
		decipherCards(initialCards);
		GAME_STATE = IN_GAME;
		break;
	}
	case PRE_GAME:
	{
		//TODO
		//wait here when starting 2P game until second player has been added
	}
	case IN_GAME:
	{
		//it not our turn, poll for info
		if(turnPhase == -1){
			xmlrpc_c::value cards;
			client.call(SERVERURL, "server.respondToInput", "iis", &cards,' ',-1, playerName.c_str());
			decipherCards(cards);
		}
		//if mouseClick, 1 and 4 are clicked and released (removes issues from holding then releasing) or keyboard key
		else if(key != 0 || (key == -1 && (gameDisplay.getMouseEventButton() == 1 || gameDisplay.getMouseEventButton() == 4))){
			//get card clicked if any
			int slotNum = -1;
			if(key == -1)
				slotNum = findCardSlot(gameDisplay.getMouseEventX(), gameDisplay.getMouseEventY());
			//TODO
			xmlrpc_c::value cards;
			client.call(SERVERURL, "server.respondToInput", "iis", &cards,key,slotNum, playerName.c_str());
			decipherCards(cards);
		}
		break;
	}
	}
}

void decipherCards(xmlrpc_c::value cards){

	vector<xmlrpc_c::value> v = xmlrpc_c::value_array(cards).vectorValueValue();
	std::vector<xmlrpc_c::value>::iterator it = v.begin();
	topBanner = xmlrpc_c::value_string(*it);
	it++;
	bottomBanner = xmlrpc_c::value_string(*it);
	it++;
	turnPhase = xmlrpc_c::value_int(*it);
	it++;
	//iterate to get cards
	int i=0;
	while(it!= v.end()){
		cardSlots[i].setSuit(xmlrpc_c::value_int(*it));
		it++;
		cardSlots[i].setValue(xmlrpc_c::value_int(*it));
		it++;
		cardSlots[i].setHighlight(xmlrpc_c::value_boolean(*it));
		it++;
		i++;
	}
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

		if(slot.suit() > -1 && slot.value() > -1)
			gameDisplay.displayCard(slot.position().x(),slot.position().y(),slot.suit(),slot.value(),0);
		else
			gameDisplay.drawBox(slot.position().x(),slot.position().y(),6,5,0);
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

int findCardSlot(int x, int y){
	//iterate over card slots
	int minX,minY,maxX,maxY;
	for(int i=0;i<NUMBERCARDSLOTS;i++){
		minX=cardSlots[i].position().x();
		maxX=minX+cardSlots[i].width();
		minY=cardSlots[i].position().y();
		maxY=minY+cardSlots[i].height();
		//return CardSlot if found
		if(x>=minX && x<=maxX && y>=minY && y<=maxY){
			return i;
		}
	}
	return -1;
}

void resetSelectedSlots(){
	if(selectedSlots[0] != NULL)
		(*selectedSlots[0]).setHighlight(false);
	if(selectedSlots[1] != NULL)
		(*selectedSlots[1]).setHighlight(false);
	selectedSlots[0] = NULL;
	selectedSlots[1] = NULL;
}

void resizeCardSlots(){
	int numCols = (gameDisplay.getCols()-2)/8;
	cardSlots[0] = CardSlot(2,2,6,5,CardSlot::deck);		//1 deck
	cardSlots[1] = CardSlot(10,2,6,5,CardSlot::discard);	//1 discard pile
	for(int i=0;i<11;i++){									//11 player cards
		cardSlots[2+i] = CardSlot(2+8*i,12, 6, 5, CardSlot::player, i);
	}
	for(int i=0;i<6;i++){									//6 combo cards
		cardSlots[13+i] = CardSlot(2+8*i,7, 6, 5, CardSlot::combo, i);
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
