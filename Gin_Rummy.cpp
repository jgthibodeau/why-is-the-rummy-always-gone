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

#include "Key.h"
#include "Point.h"
#include "CardSlot.h"
#include "Card.h"
#include "Deck.h"
#include "DiscardPile.h"
#include "Combo.h"
#include "Player.h"
#include <signal.h>
#include <math.h>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <vector>

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>


#include <cstring>
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

using namespace std;

/* TODO Move vars and function declarations to a header file */

//interaction vars
char key;
//enumberable for game status
static const int FULL = 0;
static const int WAITING = 1;
static const int EMPTY = 2;
static const int GAMEOVER = 3;
int SERVER_STATUS = EMPTY;

//game vars
Player player1;
Player player2;
Player* curPlayer;
Deck deck;
Card cardBack(0,0,0);
DiscardPile discardPile;
Combo combos[6];

//input keys
Key startKey('n', "New Game");
Key knockKey('k', "Knock");
Key submitKey('s', "Submit");
Key cancelKey('c', "Cancel");
Key quitKey('q', "Quit Game");
Key closeKey('x', "Save and Exit");
//positions to display cards
const int NUMBERCARDSLOTS = 19;
CardSlot cardSlots[NUMBERCARDSLOTS];	//1 deck, 1 discard pile, 6 combo piles, 10 player cards
CardSlot *selectedSlots[2];				//used for selecting specific cards
//banner messages to display controls and info
string dontComboMessage = "Those cards don't combo!";
string badDeadwoodMessage = "You have too much remaining deadwood!";
string bottomBanner;
string chatMessage;


//reset highlights and selected cards
void resetSelectedSlots();
void save(string name, string classType, string data);
void saveAll();
string load(string name);
void loadAll();
void initialize();
void emptyDatabase();
void endGame();

class setChat : public xmlrpc_c::method{
public:
	setChat(){}
	void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value* const retvalP){
		string message = paramList.getString(0);
		paramList.verifyEnd(1);
		//chatMessage = chatMessage +"\n"+ message;
		chatMessage = message;
		*retvalP = xmlrpc_c::value_boolean(true);
	}
};

class getChat : public xmlrpc_c::method{
public:
	getChat(){}
	void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value* const retvalP){
		paramList.verifyEnd(0);
		*retvalP = xmlrpc_c::value_string(chatMessage);
	}
};

class winnerStatus : public xmlrpc_c::method{
public:
	winnerStatus(){}
	void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value* const retvalP){
		//string playerName = paramList.getString(0);
		paramList.verifyEnd(0);

		vector<xmlrpc_c::value> returnData;
		returnData.push_back(xmlrpc_c::value_string(player1.getName()));
		returnData.push_back(xmlrpc_c::value_int(player1.getScore()));
		returnData.push_back(xmlrpc_c::value_string(player2.getName()));
		returnData.push_back(xmlrpc_c::value_int(player2.getScore()));
		//which player knocked first, whoever is not current player
		if(curPlayer == &player1)
			returnData.push_back(xmlrpc_c::value_int(2));
		else
			returnData.push_back(xmlrpc_c::value_int(1));

		// Make an XML-RPC array out of it
		xmlrpc_c::value_array ret(returnData);

		*retvalP = xmlrpc_c::value_array(ret);
	}
};

//returns the current game status (game going, waiting for p2, no game)
class gameStatus : public xmlrpc_c::method{
public:
	gameStatus(){}
	void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value* const retvalP){
		*retvalP = xmlrpc_c::value_int(SERVER_STATUS);
	}
};

class addPlayer : public xmlrpc_c::method{
public:
	addPlayer(){}
	void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value* const retvalP){
		//take in players name
		string playerName = paramList.getString(0);
		bool ai = paramList.getBoolean(1);
		paramList.verifyEnd(2);

		switch(SERVER_STATUS){
			case(EMPTY):
				//set player1 to this player
				player1.setName(playerName);
				player1.setAI(ai);
				SERVER_STATUS = WAITING;
				*retvalP = xmlrpc_c::value_boolean(true);
			break;
			case(WAITING):
				//if this players name already exists, return false
				if(player1.getName() == playerName)
					*retvalP = xmlrpc_c::value_boolean(false);
				else{
					//set player2 to this player and initialize game
					player2.setName(playerName);
					player2.setAI(ai);

					SERVER_STATUS = FULL;
					initialize();
					*retvalP = xmlrpc_c::value_boolean(true);
				}
			break;
			default:
				*retvalP = xmlrpc_c::value_boolean(false);
			break;
		}
	}
};

class checkPlayer : public xmlrpc_c::method{
public:
	checkPlayer(){}
	void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value* const retvalP){
		//take in players name
		string playerName = paramList.getString(0);
		paramList.verifyEnd(1);

		//return if player is player1 or player2
		*retvalP = xmlrpc_c::value_boolean(SERVER_STATUS != EMPTY && (playerName == player1.getName() || playerName == player2.getName()));
	}
};

class quit : public xmlrpc_c::method{
public:
	quit(){}
	void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value* const retvalP){
		//quit/clear stuff
		SERVER_STATUS = EMPTY;
		saveAll();
		//emptyDatabase();
		*retvalP = xmlrpc_c::value_boolean(true);
	}
};

//receive input from client and deal wit it, returns state of the garmez
class respondToInput : public xmlrpc_c::method{
public:
	respondToInput(){}
	void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value* const retvalP){

		//set the input vars to useable vars
		int key = paramList.getInt(0);
		int cardSlotIndex = paramList.getInt(1);
		string playerName = paramList.getString(2);
		paramList.verifyEnd(3);

		//if quitKey pressed
		if(key == quitKey.key())
			endGame();

		//TODO convert to checkEndDeck() method
		else if(deck.isEmpty()){
			bottomBanner = "The deck has run out of cards, oh noes!";
			endGame();
		}
		//if this player's turn
		else if(playerName == (*curPlayer).getName()){
			//temporarily hold on to current player and current turn phase
			Player* tempPlayer = curPlayer;
			int tempPhase = (*curPlayer).getTurnPhase();

			//set up selected slots
			if(cardSlotIndex >= 0){
				//get card clicked if any
				CardSlot* temp = &cardSlots[cardSlotIndex];

				if(selectedSlots[0] == NULL)
					selectedSlots[0] = temp;
				else
					selectedSlots[1] = temp;
			}

			//allow player to swap cards around
			//if 1st selected is player card
			if(selectedSlots[0] != NULL && (*selectedSlots[0]).type() == CardSlot::player){
				//highlight it
				(*selectedSlots[0]).setHighlight(true);
				//if 2nd selected is player cards
				if(selectedSlots[1] != NULL && (*selectedSlots[1]).type() == CardSlot::player){
					//swap and unhighlight
					(*curPlayer).swapCard((*selectedSlots[0]).index(), (*selectedSlots[1]).index());
					resetSelectedSlots();
				}
			}
			//player phases
			switch((*curPlayer).getTurnPhase()){
				//if in draw phase
				case(Player::draw):

					//if player click deck
					if((selectedSlots[0] != NULL && (*selectedSlots[0]).type() == CardSlot::deck) ||
							(selectedSlots[1] != NULL && (*selectedSlots[1]).type() == CardSlot::deck)){
						//give player a deck card and go to play phase
						(*curPlayer).addCard(deck.drawCard());
						(*curPlayer).setTurnPhase(Player::play);
						resetSelectedSlots();
					}

					//if player clicks discarded card
					if((selectedSlots[0] != NULL && (*selectedSlots[0]).type() == CardSlot::discard) ||
							(selectedSlots[1] != NULL && (*selectedSlots[1]).type() == CardSlot::discard)){
						if(!discardPile.isEmpty()){
							//give player a discard card and go to play phase
							(*curPlayer).addCard(discardPile.removeCard());
							(*curPlayer).setTurnPhase(Player::play);
							resetSelectedSlots();
						}
						else
							bottomBanner = "You can't draw from an empty discard pile, you silly!";
					}
				break;

				//if in play phase
				case(Player::play):
					//if knockKey pressed
					if(key == knockKey.key()){
						(*curPlayer).setTurnPhase(Player::knock);
					}

					else if(selectedSlots[0] != NULL){
						//if player card is 1st selected
						if((*selectedSlots[0]).type() == CardSlot::player){
							(*selectedSlots[0]).setHighlight(true);
							//if 2nd selected is discard
							if((selectedSlots[1] != NULL) && ((*selectedSlots[1]).type() == CardSlot::discard)){
								//discard selected and reset
								Card c = (*curPlayer).removeCard((*selectedSlots[0]).index());
								//if card was actually removed
								if(c.isValid()){
									discardPile.addCard(c);
									if(curPlayer == &player1)
										curPlayer = &player2;
									else
										curPlayer = &player1;
									(*curPlayer).setTurnPhase(Player::draw);
									bottomBanner = "";
								}
								resetSelectedSlots();
							}
						}
						//if not player card as 1st selected
						else
							//reset selected
							resetSelectedSlots();
					}
				break;

				//if knock phase
				case(Player::knock):

					if(selectedSlots[0] != NULL){
						//if click combo, print combo cards on bottomBanner
						if((*selectedSlots[0]).type() == CardSlot::combo){
							bottomBanner = combos[(*selectedSlots[0]).index()].toString();
							resetSelectedSlots();
						}

						//if player card is 1st selected selected
						else if((*selectedSlots[0]).type() == CardSlot::player){
							(*selectedSlots[0]).setHighlight(true);
							//if 2nd selected is combo
							if((selectedSlots[1] != NULL) && ((*selectedSlots[1]).type() == CardSlot::combo)){
								Card c = (*curPlayer).removeCard((*selectedSlots[0]).index());
								if(c.isValid()){
									//try to move this card into this combo
									//Combo com = combos[(*selectedSlots[1]).index()];
									bool success = combos[(*selectedSlots[1]).index()].addCard(c);
									if(success)
										//if combo ok
										bottomBanner = combos[(*selectedSlots[1]).index()].toString();
									else{
										(*curPlayer).addCard(c);
										bottomBanner = dontComboMessage+" "+combos[(*selectedSlots[1]).index()].toString();
									}
								}
								//else
								//	(*curPlayer).addCard(c);
								resetSelectedSlots();
							}
							//if 2nd selected is discard
							if((selectedSlots[1] != NULL) && ((*selectedSlots[1]).type() == CardSlot::discard)){

								//if combos are good
								int failedCombo = 0;
								for(int i=0;i<6;i++){
									if(!combos[i].isValid()){
										failedCombo = i+1;
										break;
									}
								}
								if(failedCombo == 0){
									//if deadwood ok
									Card c = (*curPlayer).removeCard((*selectedSlots[0]).index());
									if((*curPlayer).canKnock()){
										//TODO SUCCESSFUL KNOCK
										discardPile.addCard(c);
										//set current player score and go to next player
										(*curPlayer).setScore((*curPlayer).calculateScore());
										int score = (*curPlayer).getScore();

										if(curPlayer == &player1)
											curPlayer = &player2;
										else
											curPlayer = &player1;

										//if this players score is 0, end the game
										if(score == 0)
											SERVER_STATUS = GAMEOVER;
										else
											(*curPlayer).setTurnPhase(Player::not_knocker);
									}
									else{
										(*curPlayer).addCard(c);
										bottomBanner = badDeadwoodMessage;
									}
								}
								else{
									stringstream ss;
									ss << "Combo " << failedCombo << " is bad!" << endl;
									bottomBanner = ss.str();
								}
								resetSelectedSlots();
							}
						}
						//if not player card as 1st selected
						else
							//reset selected
							resetSelectedSlots();
					}
					//if cancelKey pressed
					else if(key == cancelKey.key()){
						//remove cards from combo
						for(int i=0;i<6;i++){
							while(!combos[i].isEmpty()){
								Card c = combos[i].removeCard();
								(*curPlayer).addCard(c);
							}
						}
						//go to play phase
						(*curPlayer).setTurnPhase(Player::play);
					}
				break;

				/*
				 * TODO
				 * Not important when only vs AI
				 *
				 */
				//if last turn phase
				case(Player::not_knocker):
					if(selectedSlots[0] != NULL){
						//if click combo, print combo cards on bottomBanner
						if((*selectedSlots[0]).type() == CardSlot::combo){
							bottomBanner = combos[(*selectedSlots[0]).index()].toString();
							resetSelectedSlots();
						}

						//if player card is 1st selected selected
						else if((*selectedSlots[0]).type() == CardSlot::player){
							(*selectedSlots[0]).setHighlight(true);
							//if 2nd selected is combo
							if((selectedSlots[1] != NULL) && ((*selectedSlots[1]).type() == CardSlot::combo)){
								Card c = (*curPlayer).removeCard((*selectedSlots[0]).index());
								c.makeOwner((*curPlayer).getName());
								if(c.isValid()){
									//try to move this card into this combo
									//Combo com = combos[(*selectedSlots[1]).index()];
									bool success = combos[(*selectedSlots[1]).index()].addCard(c);
									if(success)
										//if combo ok
										bottomBanner = combos[(*selectedSlots[1]).index()].toString();
									else{
										(*curPlayer).addCard(c);
										bottomBanner = dontComboMessage+" "+combos[(*selectedSlots[1]).index()].toString();
									}
								}
								//else
								//	(*curPlayer).addCard(c);
								resetSelectedSlots();
							}
						}
						//if not player card as 1st selected
						else
							//reset selected
							resetSelectedSlots();
					}
					//if donekey pressed
					if(key == submitKey.key()){
						//if combos are good
						int failedCombo = 0;
						for(int i=0;i<6;i++){
							if(!combos[i].isValid()){
								failedCombo = i+1;
								break;
							}
						}
						cout << failedCombo << endl;
						if(failedCombo == 0){
							(*curPlayer).setScore((*curPlayer).calculateScore());
							SERVER_STATUS = GAMEOVER;
						}
						else{
							stringstream ss;
							ss << "Combo " << failedCombo << " is bad!" << endl;
							bottomBanner = ss.str();
						}
						resetSelectedSlots();
					}
					//if cancelKey pressed
					else if(key == cancelKey.key()){
						//remove cards from combo
						for(int i=0;i<6;i++){
							if(!combos[i].isEmpty()){
								cout << "combo" << i << endl;
								combos[i].returnCardsToPlayer(curPlayer);
							}
						}
					}
				break;
			}

			//if turnphase changed or player changed, save the game
			if(tempPlayer != curPlayer || tempPhase != (*curPlayer).getTurnPhase())
				saveAll();
		}
		//do ai code if there is an aiplayer
		else if((*curPlayer).isAI()){
			//cout << "doing ai" << endl;
			//execute ai code
			//if human player knocked, just give up
			if((*curPlayer).getTurnPhase() == Player::not_knocker){
				(*curPlayer).setScore((*curPlayer).calculateScore());
				SERVER_STATUS = GAMEOVER;
			}
			//otherwise, do normal, boring ai routine
			else{
				(*curPlayer).addCard(deck.drawCard());
				Card c = (*curPlayer).removeCard(0);
				discardPile.addCard(c);
				//TODO (*curPlayer).doTurn(deck, discardPile, combos);

				if(curPlayer == &player2)
					curPlayer = &player1;
				else
					curPlayer = &player2;
				(*curPlayer).setTurnPhase(Player::draw);
				saveAll();
			}
		}
		
		//cout << "current users turn \"" << (*curPlayer).getName() << "\"" << endl;

		//convert cards/player info/banner into a big ol' array to return
		vector<xmlrpc_c::value> returnData;
		if((*curPlayer).getName() == playerName){
			returnData.push_back(xmlrpc_c::value_string(bottomBanner));
			returnData.push_back(xmlrpc_c::value_int((*curPlayer).getTurnPhase()));
		}
		else{
			returnData.push_back(xmlrpc_c::value_string(""));
			returnData.push_back(xmlrpc_c::value_int(-1));
		}
		//get the player that called here
		Player* thisPlayer = &player1;
		if(player2.getName() == playerName)
			thisPlayer = &player2;
		//send data for all cards
		int i;
		for(i=0;i<NUMBERCARDSLOTS;i++){
			CardSlot slot = cardSlots[i];
			Card* card = NULL;
			Card c;
			switch (slot.type()){
			case (CardSlot::deck):
				card = &cardBack;
				break;
			case (CardSlot::discard):
				if(!discardPile.isEmpty()){
					c = discardPile.topCard();
					card = &c;
				}
				break;
			case (CardSlot::player):
				if(slot.index() < (*thisPlayer).handSize()){
					c = (*thisPlayer).getCard(slot.index());
					card = &c;
				}
				break;
			case (CardSlot::combo):
				if(!combos[slot.index()].isEmpty()){
					c = combos[slot.index()].showCard();
					card = &c;
				}
				break;
			}
			if(card != NULL){
				returnData.push_back(xmlrpc_c::value_int((*card).suit()));
				returnData.push_back(xmlrpc_c::value_int((*card).value()));
				returnData.push_back(xmlrpc_c::value_boolean(slot.highlighted()));
			}
			else{
				returnData.push_back(xmlrpc_c::value_int(-1));
				returnData.push_back(xmlrpc_c::value_int(-1));
				returnData.push_back(xmlrpc_c::value_boolean(false));
			}
		}

		// Make an XML-RPC array out of it
		xmlrpc_c::value_array ret(returnData);

		*retvalP = xmlrpc_c::value_array(ret);
	}
};

void endGame(){
	emptyDatabase();
	SERVER_STATUS = EMPTY;
}

// Function called by sql query
// Stores data in *data pointer
static int callback(void *data, int argc, char **argv, char **azColName) {
  strcpy((char*)data, argv[0]);	  
  return 0;
 }


int main(int const argc, const char** const argv){
	//initialize cardslots
	cardSlots[0] = CardSlot(0,0,0,0,CardSlot::deck);		//1 deck
	cardSlots[1] = CardSlot(0,0,0,0,CardSlot::discard);		//1 discard pile
	for(int i=0;i<11;i++){									//11 player cards
		cardSlots[2+i] = CardSlot(0,0,0,0, CardSlot::player, i);
	}
	for(int i=0;i<6;i++){									//6 combo cards
		cardSlots[13+i] = CardSlot(0,0,0,0, CardSlot::combo, i);
	}

	SERVER_STATUS = EMPTY;


	//try to load everything if it exists
	loadAll();

	xmlrpc_c::registry myRegistry;
	xmlrpc_c::methodPtr const respondToInputP(new respondToInput);
	myRegistry.addMethod("server.respondToInput", respondToInputP);
	xmlrpc_c::methodPtr const addPlayerP(new addPlayer);
	myRegistry.addMethod("server.addPlayer", addPlayerP);
	xmlrpc_c::methodPtr const checkPlayerP(new checkPlayer);
	myRegistry.addMethod("server.checkPlayer", checkPlayerP);
	xmlrpc_c::methodPtr const quitP(new quit);
	myRegistry.addMethod("server.quit", quitP);
	xmlrpc_c::methodPtr const gameStatusP(new gameStatus);
	myRegistry.addMethod("server.gameStatus", gameStatusP);
	xmlrpc_c::methodPtr const getChatP(new getChat);
	myRegistry.addMethod("server.getChat", getChatP);
	xmlrpc_c::methodPtr const setChatP(new setChat);
	myRegistry.addMethod("server.setChat", setChatP);

	xmlrpc_c::serverAbyss welcomeToTheAbyss(
		myRegistry,
		10503,
		"/tmp/xmlrpc_log");

	welcomeToTheAbyss.run();
	assert(false);
	return 0;
}

void resetSelectedSlots(){
	if(selectedSlots[0] != NULL)
		(*selectedSlots[0]).setHighlight(false);
	if(selectedSlots[1] != NULL)
		(*selectedSlots[1]).setHighlight(false);
	selectedSlots[0] = NULL;
	selectedSlots[1] = NULL;
}

void emptyDatabase(){
	sqlite3 *db;
	char *dErrMsg = 0;
	int rc;
	const char* sql;
	char data[2056]; 

	rc = sqlite3_open("datebase.db", &db);
	if( rc ){
	  fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	  exit(0);
	}else{
	  fprintf(stderr, "Opened database successfully\n");
	}

	string sql_string = "DROP TABLE table_name 'GAME'";
	sql = sql_string.c_str();

	rc = sqlite3_exec(db, sql, callback, (void*)data, &dErrMsg);
	if( rc != SQLITE_OK ){
	  fprintf(stderr, "SQL error: %s\n", dErrMsg);
	  sqlite3_free(dErrMsg);
	}else{
	  cout << "Dropped successfully\n" << endl;
	}

	sqlite3_close(db);
}

void save(string name, string classType, string data){
	sqlite3 *db;
	char *dErrMsg = 0;
	int rc;
	const char* sql;  

	rc = sqlite3_open("datebase.db", &db);
	if( rc ){
	  fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	  exit(0);
	}else{
	  fprintf(stderr, "Opened database successfully\n");
	}

	sql = "CREATE TABLE GAME("  \
	     "OBJECT_NAME TEXT PRIMARY KEY NOT NULL," \
	 "OBJECT_TYPE TEXT NOT NULL," \
	 "OBJECT_VALUE TEXT NOT NULL);";

	rc = sqlite3_exec(db, sql, NULL, 0, &dErrMsg);
	if( rc != SQLITE_OK ){
	fprintf(stderr, "SQL error: %s\n", dErrMsg);
	  sqlite3_free(dErrMsg);
	}else{
	  fprintf(stdout, "Table created successfully\n");
	}

	// Insert serialized version of player1 object to database
	string str = ("INSERT OR REPLACE INTO GAME VALUES ('" + name + "', '" + classType + "', '" + data + "');");
	sql = str.c_str();
	rc = sqlite3_exec(db, sql, callback, 0, &dErrMsg);
	if( rc != SQLITE_OK ){
	  fprintf(stderr, "SQL error: %s\n", dErrMsg);
	  sqlite3_free(dErrMsg);
	}else{
	 //cout << name << " Records created successfully\n" << endl;
	}


	sqlite3_close(db);
}

void saveAll(){
	save("player1", "Player", player1.save());
	save("player2", "Player", player2.save());
	save("deck", "Deck", deck.save());
	save("discardPile", "DiscardPile", discardPile.save());
	for(int i=0;i<6;i++){
		stringstream comboName;
		comboName << "combo" << i;
	 	save(comboName.str(), "Combo", combos[i].save());
	}
	stringstream state;
	state << SERVER_STATUS;
	save("SERVER_STATUS", "SERVER", state.str());
	save("curPlayer", "SERVER", (*curPlayer).getName());
}

string load(string name){
	sqlite3 *db;
	char *dErrMsg = 0;
	int rc;
	const char* sql;
	char data[2056]; 

	rc = sqlite3_open("datebase.db", &db);
	if( rc ){
	  fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	  exit(0);
	}else{
	  fprintf(stderr, "Opened database successfully\n");
	}

	  // Read serialied version of player1 object from database
	string str = "SELECT OBJECT_VALUE from GAME WHERE OBJECT_NAME='" + name + "';";
	sql = str.c_str();
	rc = sqlite3_exec(db, sql, callback, (void*)data, &dErrMsg);
	if( rc != SQLITE_OK ){
	  fprintf(stderr, "SQL error: %s\n", dErrMsg);
	  sqlite3_free(dErrMsg);
	}else{
	  //cout << name << " Operation done successfully\n" << endl;
	}

	sqlite3_close(db);

	return data;
}

void loadAll(){
	//check if game table exists
	sqlite3 *db;
	char *dErrMsg = 0;
	int rc;
	const char* sql;
	char data[2056]; 

	rc = sqlite3_open("datebase.db", &db);
	if( rc ){
	  fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	  exit(0);
	}else{
	  fprintf(stderr, "Opened database successfully\n");
	}

	string sql_string = "select count(type) from sqlite_master where type='table' and name='GAME'";
	sql = sql_string.c_str();

	rc = sqlite3_exec(db, sql, callback, (void*)data, &dErrMsg);
	if( rc != SQLITE_OK ){
	  fprintf(stderr, "SQL error: %s\n", dErrMsg);
	  sqlite3_free(dErrMsg);
	}

	sqlite3_close(db);

	//cout << data << endl;
	//if table exists, load errythang
	if(atoi(data) > 0){
		player1.load(load("player1"));
		player2.load(load("player2"));
		deck.load(load("deck"));
		discardPile.load(load("discardPile"));

		for(int i=0;i<6;i++){
			stringstream comboName;
			comboName << "combo" << i;
			combos[i].load(load(comboName.str()));
		}
		SERVER_STATUS = atoi(load("SERVER_STATUS").c_str());
		string p = load("curPlayer");
		if(p == player1.getName())
			curPlayer = &player1;
		else
			curPlayer = &player2;
		//cout << "LoadAll done successfully\n" << endl;
	}
	else{
		//cout << "LoadAll skipped successfully\n" << endl;
	}
}

//set us up the gamez
void initialize(){
	bottomBanner = "";
	chatMessage="";

	player1.initialize();
	player2.initialize();
	for(int i=0;i<6;i++)
	 	combos[i].initialize();

	deck.initialize();
	deck.shuffle();
	discardPile.initialize();
	//decide first player
	srand (time(NULL));
	if(rand()%1 == 0)
		curPlayer = &player1;
	else
		curPlayer = &player2;

	//Deal player cards
	for(int i=0;i<10;i++){
		player1.addCard(deck.drawCard());
		player1.setTurnPhase(Player::draw);
	}
	for(int i=0;i<10;i++){
		player2.addCard(deck.drawCard());
		player2.setTurnPhase(Player::draw);
	}
	discardPile.addCard(deck.drawCard());

	saveAll();
}