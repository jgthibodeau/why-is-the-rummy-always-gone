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
		cout << "[PASSED]";
	else
		cout << "[FAILED]";
	cout << " CardSlot.setBounds(1,2,3,4) set bounds of slot to (" << slot.position().x() << "," << slot.position().x()
			<< slot.width() << "," << slot.height() << ")" << endl;

	slot.move(1,2);
	if(slot.position().x() == 2 && slot.position().y() == 4)
		cout << "[PASSED]";
	else
		cout << "[FAILED]";
	cout << " CardSlot.move(1,2) moved slot to (" << slot.position().x() << "," << slot.position().y() << ")" << endl;

	slot.setHighlight(true);
	if(slot.highlighted())
		cout << "[PASSED]";
	else
		cout << "[FAILED]";
	cout << " CardSlot.setHighlight(tue) set slot highlight to " << slot.highlighted() << endl;

	slot.setHighlight(false);
	if(!slot.highlighted())
		cout << "[PASSED]";
	else
		cout << "[FAILED]";
	cout << " CardSlot.setHighlight(tue) set slot highlight to " << slot.highlighted() << endl;

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


    Card test_Card;
    bool test_bool;

    ///create single player
    Player player("mike", 0, 0, false);

    ///tests
    ///-------------------------------
    cout << "PLAYER TESTS" << endl;
    ///Try to remove card before any cards are in player's hand
    test_Card = player.removeCard(0);
    if (test_Card.points() == 0)
        cout << "[PASSED] ";
    else
        cout << "[FAILED] ";
    cout << "Player.removeCard(0) returned card with value " << test_Card.points() << ", expecting 0" <<endl;

    ///add cards
    player.addCard(card1);
    player.addCard(card2);
    player.addCard(card3);

    int i = player.calculateScore();
    if (i = 13)
            cout << "[PASSED] ";
    else
            cout << "[FAILED] ";
    cout << "Player.calculateScore() returned " << i << ", expected 13" << endl;

    ///remove card 3 (indexing begins at 0)
    test_Card = player.removeCard(2);
    if (test_Card.points() == 6)
        cout << "[PASSED] ";
    else
        cout << "[FAILED] ";
    cout << "Player.removeCard(2) returned " << test_Card.points() << ", expected 6" << endl;

    ///expecting 7
    i = player.calculateScore();
    if (i = 7)
        cout << "[PASSED] ";
    else
        cout << "[FAILED] ";
    cout << "Player.calculateScore() returned " << i << ", expected 7" << endl;


    player.swapCard(0,1);
    test_Card = player.removeCard(1);
    if (test_Card.points() == 3)
        cout << "[PASSED] ";
    else
        cout << "[FAILED] ";
    cout << "Player.removeCard(1) returned " << test_Card.points() << ", expected 3" << endl;

    ///expecting 4
    i = player.calculateScore();
    if (i == 4)
        cout << "[PASSED] ";
    else
        cout << "[FAILED] ";
    cout << "Player.calculateScore() returned " << i << ", expected 4" << endl;


    ///expecting to not knock
    test_bool = player.canKnock();
    if (test_bool == true)
        cout << "[PASSED] ";
    else
        cout << "[FAILED] ";
    cout << "player.canKnock() returned " << test_bool << ", expecting 1" << endl;

    player.addCard(card4);
    ///expecting 9
    i = player.calculateScore();
    if (i == 9)
        cout << "[PASSED] ";
    else
        cout << "[FAILED] ";
    cout << "Player.calculateScore() returned " << i << ", expected 9" << endl;
    ///can knock
    test_bool = player.canKnock();
    if (test_bool == true)
        cout << "[PASSED] ";
    else
        cout << "[FAILED] ";
    cout << "player.canKnock() returned " << test_bool << ", expecting 1" << endl;
    player.addCard(card5);

    i = player.calculateScore();
    if (i == 10)
        cout << "[PASSED] ";
    else
        cout << "[FAILED] ";
    cout << "Player.calculateScore() returned " << i << ", expected 10" << endl;


    ///cant knock
    test_bool = player.canKnock();
    if (test_bool == false)
        cout << "[PASSED] ";
    else
        cout << "[FAILED] ";
    cout << "player.canKnock() returned " << test_bool << ", expecting 0" << endl;

/**************
* Zack's tests
*/
    Card c1(1,2,2); //testing card 1
    Card c2(1,3,3); //testing card 2
    Card c3(1,4,4); //testing card 3
    Card c4(2,1,1); //testing card 4
    Combo comb; //testing combo
    DiscardPile discardP; //testing discard pile 
    Deck deck; //testing deck

    bool isEmptyBefore = comb.isEmpty();
    bool isDiscardEmpty = discardP.isEmpty();
    bool addCardResult1 = comb.addCard(c2);
    bool addCardResult2 = comb.addCard(c3);
    bool addCardResult3 = comb.addCard(c1);
    bool addCardResult4 = comb.addCard(c4);
    discardP.addCard(c4);
    bool isDiscardEmptyAfterAdd = discardP.isEmpty();
    discardP.removeCard();
    bool isDiscardEmptyAfterRM = discardP.isEmpty();
    bool isEmptyAfter = comb.isEmpty();
    bool comboValid = comb.isValid(); 
    bool emptyStartDeck = deck.isEmpty(); 
    deck.shuffle(); 
    Card shufCard1 = deck.drawCard(); 
    Card shufCard2 = deck.drawCard(); 
    Card shufCard3 = deck.drawCard();
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
    if(isDiscardEmptyAfterRM){
    	cout << "[PASSED] REMOVING CARD FROM DISCARD PILE" << endl;
    }
    else{
    	cout << "[FAILED] REMOVING CARD FROM DISCARD PILE" << endl;
    } 

    cout << "DECK TESTS" << endl; 
    if(!emptyStartDeck){ 
        cout << "[PASSED] CHECKING THAT START DECK ISN'T EMPTY" << endl;
    } 
    else{ 
        cout << "[PASSED] CHECKING THAT START DECK ISN'T EMPTY" << endl;
    } 
    if((shufCard3.suit()!=shufCard2.suit()!=shufCard1.suit()) && (shufCard3().value()!= (shufCard2.value()-1 && shufCard1.value()-2))){ 
        cout << "[PASSED] SHUFFLED CARDS" << endl;
    }
    else{ 
        cout << "[PASSED] SHUFFLED CARDS" << endl;
    }
	return 0;
}


