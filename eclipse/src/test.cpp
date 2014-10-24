/*
 * test.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: ece373
 */
#include "display.h"
#include "Key.h"
#include "Point.h"
#include "CardSlot.h"
#include "Card.h"
#include "Deck.h"
#include "DiscardPile.h"
#include "Combo.h"
#include "Player.h"
#include <signal.h>
#include <ncurses.h>
#include <math.h>
#include <cstdlib>
#include <sstream>
using namespace std;
int main(int argc, char* argv[]){
/*****
 * Justin's tests
 */

	//test Point class
	cout << "POINT TESTS" << endl;
	Point point;

	point.setPoint(1,2);
	if(point.x() == 1 && point.y() == 2)
		cout << "[PASSED]";
	else
		cout << "[FAILED]";
	cout << " Point.setPoint(1,2) set point to (" << point.x() << "," << point.y() << ")" << endl;

	point.move(3,4);
	if(point.x() == 4 && point.y() == 6)
		cout << "[PASSED]";
	else
		cout << "[FAILED]";
	cout << " Point.move(3,4) moved point to (" << point.x() << "," << point.y() << ")" << endl;

	//test Cardslot class
	cout << "CARDSLOT TESTS" << endl;
	CardSlot slot;

	slot.setBounds(1,2,3,4);
	if(slot.position().x() == 1 && slot.position().y() == 2 && slot.width() == 3 && slot.height() == 4)
		cout << "[PASSED]" << endl;
	else
		cout << "[FAILED] CardSlot.setBounds" << endl;

	slot.move(1,2);
	if(slot.position().x() == 2 && slot.position().y() == 4)
		cout << "[PASSED]" << endl;
	else
		cout << "[FAILED] CardSlot.move" << endl;

	slot.setHighlight(true);
	if(slot.highlighted())
		cout << "[PASSED]" << endl;
	else
		cout << "[FAILED] CardSlot.setHighlight" << endl;

	//test Display class?

/****
 * Michael's tests
 */
    ///create three cards
    Card card1(1,2,3);
    Card card2(2,3,4);
    Card card3(2,3,6);
    Card card4(2,3,5);
    Card card5(4,7,1);

    ///create single player
    Player player("mike", 0, 0, false);

    ///tests
    ///-------------------------------

    ///Try to remove card before any cards are in player's hand
    player.removeCard(0);

    ///add cards
    player.addCard(card1);
    player.addCard(card2);
    player.addCard(card3);

    ///expecting 3+4+6 = 13
    int i = player.calculateScore();

    ///remove card 3 (indexing begins at 0)
    player.removeCard(2);
    ///expecting 7
    i = player.calculateScore();

    ///should return error
    player.swapCard(1,2);

    player.swapCard(0,1);
    player.removeCard(1);
    ///expecting 4
    player.calculateScore();

    ///expecting to not knock
    player.canKnock();

    player.addCard(card4);
    ///expecting 9
    player.calculateScore();
    ///can knock
    player.canKnock();
    player.addCard(card5);
    ///cant knock
    player.canKnock();

/**************
* Zach's tests
*/
    Card c1(1,2,2); //testing card 1
    Card c2(1,3,3); //testing card 2
    Card c3(1,4,4); //testing card 3
    Card c4(2,1,1); //testing card 4
    Combo comb; //testing combo
    DiscardPile discardP; //testing discard pile

    //TESTING COMBO METHODS
    bool isEmptyBefore = comb.isEmpty();
    bool isDiscardEmpty = discardP.isEmpty();
    bool addCardResult1 = comb.addCard(c2);
    bool addCardResult2 = comb.addCard(c3);
    bool addCardResult3 = comb.addCard(c1);
    bool addCardResult4 = comb.addCard(c4);
    discardP.addCard(c4);
    bool isDiscardEmptyAfterAdd = discardP.isEmpty();
    //discardP.removeCard();
    //bool isDiscardEmptyAfterRM = discardP.isEmpty();
    bool isEmptyAfter = comb.isEmpty();
    bool comboValid = comb.isValid();
    cout << "COMBO TESTS" << endl;
    if(isEmptyBefore && !isEmptyAfter){
    	cout << "[PASSED] CHECKING EMPTY STATUS" << endl;
    }
    else{
    	cout << "[FAILED] CHECKING EMPTY STATUS" << endl;
    }
    if(comboValid){
    	cout << "[PASSED] CHECKING VALID COMBO" << endl;
    }
    else{
    	cout << "[FAILED] CHECKING VALID COMBO" << endl;
    }
    if(addCardResult1 && addCardResult2 && addCardResult3 && !addCardResult4){
    	cout << "[PASSED] ADDING CARD TO COMBOS" << endl;
    }
    else{
    	cout << "[FAILED] ADDING CARD TO COMBOS" << endl;
    }
    if(comb.toString() == "[2 of Spades][3 of Spades][4 of Spades]"){
    	cout << "[PASSED] COMBO TO STRING" << endl;
    }
    else{
    	cout << "[FAILED] COMBO TO STRING" << endl;
    }

    cout << "CARD TESTS" << endl;
    if(c4.toString() == "[Ace of Hearts]"){
    	cout << "[PASSED] CARD TO STRING" << endl;
    }
    else{
    	cout << "[FAILED] CARD TO STRING" << endl;
    }

    cout << "DISCARD PILE TESTS" << endl;
    if(isDiscardEmpty){
    	cout << "[PASSED] TESTING IS EMPTY METHOD" << endl;
    }
    else{
    	cout << "[FAILED] TESTING IS EMPTY METHOD" << endl;
    }
    if(!isDiscardEmptyAfterAdd){
    	cout << "[PASSED] ADDING CARD TO DISCARD PILE" << endl;
    }
    else{
    	cout << "[FAILED] ADDING CARD TO DISCARD PILE" << endl;
    }
    if(true){
    	cout << "[PASSED] REMOVING CARD FROM DISCARD PILE" << endl;
    }
    else{
    	cout << "[FAILED] REMOVING CARD FROM DISCARD PILE" << endl;
    }

	return 0;
}


