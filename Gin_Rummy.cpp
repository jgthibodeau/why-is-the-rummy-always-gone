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

using namespace std;

/* TODO Move vars and function declarations to a header file */

//client vars
char key;

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
Key quitKey('q', "Quit");
//positions to display cards
const int NUMBERCARDSLOTS = 19;
CardSlot cardSlots[NUMBERCARDSLOTS];	//1 deck, 1 discard pile, 6 combo piles, 10 player cards
CardSlot *selectedSlots[2];				//used for selecting specific cards
//banner messages to display controls and info
string startMessage = "Gin Rummy - "+startKey.toString()+"\t"+quitKey.toString();
string drawMessage = "Gin Rummy - Click Deck or Discard Pile to Draw - "+quitKey.toString();
string playMessage = "Gin Rummy - Click a Card then Discard Pile to Discard - "+knockKey.toString()+" - "+quitKey.toString();
string knockMessage = "Gin Rummy - Click a Card then a Combo Slot to Play it - Click a Card then Discard Pile to Knock - "+cancelKey.toString()+" - "+quitKey.toString();
string notTurnMessage = "Gin Rummy - Opponent's Turn - "+quitKey.toString();
string nameMessage = "Enter your name: ";
string dontComboMessage = "Those cards don't combo!";
string badDeadwoodMessage = "You have too much remaining deadwood!";
string topBanner,bottomBanner;


//reset highlights and selected cards
void resetSelectedSlots();

//TODO eventually!
class save : public xmlrpc_c::method{
public:
	save(){}
	void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value* const retvalP){
		//TODO save all the things
		// datebase variable
		// player1.save();
		// player2.save();
		// deck.save();
		// discardPile.save();
		// for(int i=0;i<6;i++)
		// 	combos[i].save();
	}
};

class load : public xmlrpc_c::method{
public:
	load(){}
	void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value* const retvalP){
		//take in players name

		//TODO load all the things
		//save all the things
		// datebase variable
		// player1.load();
		// player2.load();
		// deck.load();
		// discardPile.load();
		// for(int i=0;i<6;i++)
		// 	combos[i].load();

		//return if players name doesn't match one of the players that was saved
	}
};

class addPlayer : public xmlrpc_c::method{
public:
	addPlayer(){}
	void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value* const retvalP){
		//TODO
		//take in players name

		//if no players yet, set player1 to this player
		//if player1, set player2 to this player

		//return -1 if already 2 players
		//return number of players
	}
};

class quit : public xmlrpc_c::method{
public:
	quit(){}
	void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value* const retvalP){
		//TODO quit/clear stuff
	}
};

//set us up the gamez
class initialize : public xmlrpc_c::method{
public:
	initialize(){}
	void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value* const retvalP){
		string name = paramList.getString(0);
		paramList.verifyEnd(1);

		//TODO initialize all the things
		// player1.initialize();
		// player2.initialize();
		// discardPile.initialize();
		// for(int i=0;i<6;i++)
		// 	combos[i].initialize();

		deck.initialize();
		deck.shuffle();
		discardPile.initialize();
		//TODO decide first player
		curPlayer = &player1;
		//Deal player cards
		for(int i=0;i<10;i++){
			player1.addCard(deck.drawCard());
			player1.setTurnPhase(Player::draw);
			player1.setName(name);
		}
		for(int i=0;i<10;i++){
			player2.addCard(deck.drawCard());
			player2.setTurnPhase(Player::draw);
		}
		discardPile.addCard(deck.drawCard());

		//initialize cardslots
		cardSlots[0] = CardSlot(0,0,0,0,CardSlot::deck);		//1 deck
		cardSlots[1] = CardSlot(0,0,0,0,CardSlot::discard);	//1 discard pile
		for(int i=0;i<11;i++){									//11 player cards
			cardSlots[2+i] = CardSlot(0,0,0,0, CardSlot::player, i);
		}
		for(int i=0;i<6;i++){									//6 combo cards
			cardSlots[13+i] = CardSlot(0,0,0,0, CardSlot::combo, i);
		}

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
			std::exit(0);

		//TODO convert to checkEndDeck() method
		if(deck.isEmpty()){
			bottomBanner = "The deck has run out of cards, oh noes!";
		}
		//if this player's turn
		if(playerName == (*curPlayer).getName()){
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
					topBanner = drawMessage;

					//if player click deck
					if((selectedSlots[0] != NULL && (*selectedSlots[0]).type() == CardSlot::deck) ||
							(selectedSlots[1] != NULL && (*selectedSlots[1]).type() == CardSlot::deck)){
						//give player a deck card and go to play phase
						(*curPlayer).addCard(deck.drawCard());
						topBanner = playMessage;
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
							topBanner = playMessage;
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
						topBanner = knockMessage;
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
									topBanner = notTurnMessage;
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
								else
									(*curPlayer).addCard(c);
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
										//go to next player
										//curPlayer = &player2;
										//TODO player1.setActivity(false);
										bottomBanner = "A winner is you!";
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
						topBanner = playMessage;
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
						//if player card is 1st selected selected
						if((*selectedSlots[0]).type() == CardSlot::player){
							(*selectedSlots[0]).setHighlight(true);
							//if 2nd selected is combo
							if((*selectedSlots[1]).type() == CardSlot::combo){
								//TODO try to move this card into this combo
								//TODO if combo ok
									//TODO bottomBanner = combos[(*selectedSlots[1]).index()].toString();
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
				break;
			}
		}

		else{
			sleep(1);
			//execute ai code
			player2.addCard(deck.drawCard());
			Card c = player2.removeCard(0);
			discardPile.addCard(c);
			curPlayer = &player1;
			player1.setTurnPhase(Player::draw);
			topBanner = drawMessage;
		}

		//convert cards/player info/banner into a big ol' array to return
		vector<xmlrpc_c::value> returnData;
		returnData.push_back(xmlrpc_c::value_string(topBanner));
		returnData.push_back(xmlrpc_c::value_string(bottomBanner));
		if((*curPlayer).getName() == playerName)
			returnData.push_back(xmlrpc_c::value_int((*curPlayer).getTurnPhase()));
		else
			returnData.push_back(xmlrpc_c::value_int(-1));
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

int main(int const argc, const char** const argv){
	xmlrpc_c::registry myRegistry;
	xmlrpc_c::methodPtr const respondToInputP(new respondToInput);
	myRegistry.addMethod("server.respondToInput", respondToInputP);
	xmlrpc_c::methodPtr const initializeP(new initialize);
	myRegistry.addMethod("server.initialize", initializeP);
	xmlrpc_c::methodPtr const saveP(new save);
	myRegistry.addMethod("server.save", saveP);
	xmlrpc_c::methodPtr const loadP(new load);
	myRegistry.addMethod("server.load", loadP);
	xmlrpc_c::methodPtr const addPlayerP(new addPlayer);
	myRegistry.addMethod("server.load", addPlayerP);
	xmlrpc_c::methodPtr const quitP(new quit);
	myRegistry.addMethod("server.load", quitP);

	xmlrpc_c::serverAbyss welcomeToTheAbyss(
		myRegistry,
		8080,
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