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


#include <cassert>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

using namespace std;

/* TODO Move vars and function declarations to a header file */

// client vars

//initialize();
//save();
//load();

class doSomething : public xmlrpc_c::method{
public:
	doSomething(){}
	void execute(xmlrpc_c::paramList cost& paramList, xmlrpc_c::value* const retvalP){
		int first = paramList.getInt(0);
		int second = paramList.getInt(1);

		paramList.verifyEnd(2);

		*retvalP = xmlrpc_c::value_int(first+second);
	}
};

int main(int const argc, const char** const argv){
	xmlrpc_c::registry myRegistry;
	xmlrpc_c::methodPtr const doSomethingP(new doSomething);
	myRegistry.addMethod("server.do", doSomethingP);

	xmlrpc_c::serverAbyss welcomeToTheAbbys(
		myRegistry,
		8080,
		"/tmp/xmlrpc_log");

	welcomeToTheAbbys.run();
	assert(false);
	return 0;
}



// void respondToInput(char key, int cardSlotIndex, int playerID){

// 	//if quitKey pressed
// 	if(key == quitKey.key())
// 		std::exit(0);

// 	//TODO convert to checkEndDeck() method
// 	if(deck.isEmpty()){
// 		bottomBanner = "The deck has run out of cards, oh noes!";
// 	}
// 	//if player turn
// 	if(curPlayer == &player1){
// 		//if mouseClick, 1 and 4 are click and release (removes issues from holding then releasing)
// 		if(cardSlotIndex >= 0){
// 			//get card clicked if any
// 			CardSlot* temp = &cardSlots[cardSlotIndex];

// 			if(selectedSlots[0] == NULL)
// 				selectedSlots[0] = temp;
// 			else
// 				selectedSlots[1] = temp;
// 		}

// 		//allow player to swap cards around
// 		//if 1st selected is player card
// 		if(selectedSlots[0] != NULL && (*selectedSlots[0]).type() == CardSlot::player){
// 			//highlight it
// 			(*selectedSlots[0]).setHighlight(true);
// 			//if 2nd selected is player cards
// 			if(selectedSlots[1] != NULL && (*selectedSlots[1]).type() == CardSlot::player){
// 				//swap and unhighlight
// 				player1.swapCard((*selectedSlots[0]).index(), (*selectedSlots[1]).index());
// 				resetSelectedSlots();
// 			}
// 		}
// 		//player phases
// 		switch(player1.getTurnPhase()){
// 			//if in draw phase
// 			case(Player::draw):
// 				topBanner = drawMessage;

// 				//if player click deck
// 				if((selectedSlots[0] != NULL && (*selectedSlots[0]).type() == CardSlot::deck) ||
// 						(selectedSlots[1] != NULL && (*selectedSlots[1]).type() == CardSlot::deck)){
// 					//give player a deck card and go to play phase
// 					player1.addCard(deck.drawCard());
// 					player1.setTurnPhase(Player::play);
// 					resetSelectedSlots();
// 				}

// 				//if player clicks discarded card
// 				if((selectedSlots[0] != NULL && (*selectedSlots[0]).type() == CardSlot::discard) ||
// 						(selectedSlots[1] != NULL && (*selectedSlots[1]).type() == CardSlot::discard)){
// 					if(!discardPile.isEmpty()){
// 						//give player a discard card and go to play phase
// 						player1.addCard(discardPile.removeCard());
// 						player1.setTurnPhase(Player::play);
// 						resetSelectedSlots();
// 					}
// 					else
// 						bottomBanner = "You can't draw from an empty discard pile, you silly!";
// 				}
// 			break;

// 			//if in play phase
// 			case(Player::play):
// 				topBanner = playMessage;

// 				//if knockKey pressed
// 				if(key == knockKey.key()){
// 					player1.setTurnPhase(Player::knock);
// 				}

// 				else if(selectedSlots[0] != NULL){
// 					//if player card is 1st selected selected
// 					if((*selectedSlots[0]).type() == CardSlot::player){
// 						(*selectedSlots[0]).setHighlight(true);
// 						//if 2nd selected is discard
// 						if((selectedSlots[1] != NULL) && ((*selectedSlots[1]).type() == CardSlot::discard)){
// 							//discard selected and reset
// 							Card c = player1.removeCard((*selectedSlots[0]).index());
// 							//if card was actually removed
// 							if(c.isValid()){
// 								discardPile.addCard(c);
// 								curPlayer = &player2;
// 							}
// 							resetSelectedSlots();
// 						}
// 					}
// 					//if not player card as 1st selected
// 					else
// 						//reset selected
// 						resetSelectedSlots();
// 				}
// 			break;

// 			//if knock phase
// 			case(Player::knock):
// 				topBanner = knockMessage;

// 				if(selectedSlots[0] != NULL){
// 					//if click combo, print combo cards on bottomBanner
// 					if((*selectedSlots[0]).type() == CardSlot::combo){
// 						bottomBanner = combos[(*selectedSlots[0]).index()].toString();
// 						resetSelectedSlots();
// 					}

// 					//if player card is 1st selected selected
// 					else if((*selectedSlots[0]).type() == CardSlot::player){
// 						(*selectedSlots[0]).setHighlight(true);
// 						//if 2nd selected is combo
// 						if((selectedSlots[1] != NULL) && ((*selectedSlots[1]).type() == CardSlot::combo)){
// 							Card c = player1.removeCard((*selectedSlots[0]).index());
// 							if(c.isValid()){
// 								//try to move this card into this combo
// 								//Combo com = combos[(*selectedSlots[1]).index()];
// 								bool success = combos[(*selectedSlots[1]).index()].addCard(c);
// 								if(success)
// 									//if combo ok
// 									bottomBanner = combos[(*selectedSlots[1]).index()].toString();
// 								else{
// 									player1.addCard(c);
// 									bottomBanner = dontComboMessage+" "+combos[(*selectedSlots[1]).index()].toString();
// 								}
// 							}
// 							else
// 								player1.addCard(c);
// 							resetSelectedSlots();
// 						}
// 						//if 2nd selected is discard
// 						if((selectedSlots[1] != NULL) && ((*selectedSlots[1]).type() == CardSlot::discard)){

// 							//if combos are good
// 							int failedCombo = 0;
// 							for(int i=0;i<6;i++){
// 								if(!combos[i].isValid()){
// 									failedCombo = i+1;
// 									break;
// 								}
// 							}
// 							if(failedCombo == 0){
// 								//if deadwood ok
// 								Card c = player1.removeCard((*selectedSlots[0]).index());
// 								if(player1.canKnock()){
// 									//TODO SUCCESSFUL KNOCK
// 									discardPile.addCard(c);
// 									//go to next player
// 									//curPlayer = &player2;
// 									//TODO player1.setActivity(false);
// 									bottomBanner = "A winner is you!";
// 								}
// 								else{
// 									player1.addCard(c);
// 									bottomBanner = badDeadwoodMessage;
// 								}
// 							}
// 							else{
// 								stringstream ss;
// 								ss << "Combo " << failedCombo << " is bad!" << endl;
// 								bottomBanner = ss.str();
// 							}
// 							resetSelectedSlots();
// 						}
// 					}
// 					//if not player card as 1st selected
// 					else
// 						//reset selected
// 						resetSelectedSlots();
// 				}
// 				//if cancelKey pressed
// 				else if(key == cancelKey.key()){
// 					//remove cards from combo
// 					for(int i=0;i<6;i++){
// 						while(!combos[i].isEmpty()){
// 							Card c = combos[i].removeCard();
// 							player1.addCard(c);
// 						}
// 					}
// 					//go to play phase
// 					player1.setTurnPhase(Player::play);
// 				}
// 			break;

// 			/*
// 			 * TODO
// 			 * Not important when only vs AI
// 			 *
// 			 */
// 			//if last turn phase
// 			case(Player::not_knocker):
// 				topBanner = knockMessage;

// 				if(selectedSlots[0] != NULL){
// 					//if player card is 1st selected selected
// 					if((*selectedSlots[0]).type() == CardSlot::player){
// 						(*selectedSlots[0]).setHighlight(true);
// 						//if 2nd selected is combo
// 						if((*selectedSlots[1]).type() == CardSlot::combo){
// 							//TODO try to move this card into this combo
// 							//TODO if combo ok
// 								//TODO bottomBanner = combos[(*selectedSlots[1]).index()].toString();
// 							//TODO else
// 								bottomBanner = dontComboMessage;
// 							resetSelectedSlots();
// 						}
// 					}
// 					//if not player card as 1st selected
// 					else
// 						//reset selected
// 						resetSelectedSlots();
// 				}
// 				//if submit key pressed
// 				if(key == submitKey.key()){
// 					//TODO go to end game
// 				}
// 				//if cancelKey pressed
// 				if(key == cancelKey.key()){
// 					//TODO remove cards from any combos to curPlayers cards
// 				}
// 			break;
// 		}

// 		//if ai turn
// 		else if(curPlayer == &player2){
// 			topBanner = notTurnMessage;
// 			//execute ai code
// 			player2.addCard(deck.drawCard());
// 			Card c = player2.removeCard(0);
// 			discardPile.addCard(c);
// 			curPlayer = &player1;
// 			player1.setTurnPhase(Player::draw);
// 		}
// 	break;
// 	}
// }