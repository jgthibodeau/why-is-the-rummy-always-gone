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
	//test Point class
	cout << "     POINT TESTS" << endl;
	Point a;

	a.setPoint(1,2);
	if(a.x() == 1 && a.y() == 2)
		cout << "[PASSED]" << endl;
	else
		cout << "[FAILED] Point.setPoint" << endl;

	a.move(3,4);
	if(a.x() == 4 && a.y() == 6)
		cout << "[PASSED]" << endl;
	else
		cout << "[FAILED] Point.move" << endl;

	//test Cardslot class
	cout << "     CARDSLOT TESTS" << endl;
	CardSlot c;

	c.setBounds(1,2,3,4);
	if(c.position().x() == 1 && c.position().y() == 2 && c.width() == 3 && c.height() == 4)
		cout << "[PASSED]" << endl;
	else
		cout << "[FAILED] CardSlot.setBounds" << endl;

	c.move(1,2);
	if(c.position().x() == 2 && c.position().y() == 4)
		cout << "[PASSED]" << endl;
	else
		cout << "[FAILED] CardSlot.move" << endl;

	c.setHighlight(true);
	if(c.highlighted())
		cout << "[PASSED]" << endl;
	else
		cout << "[FAILED] CardSlot.setHighlight" << endl;

	//test Display class?

	return 0;
}


