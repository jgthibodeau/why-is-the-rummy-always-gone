#ifndef CARD_H
#define CARD_H
#include <string>
#include <stdlib.h>
#include <sstream>
#include <iostream>

using namespace std;
//#include "Player.h"
class Player;

class Card { 
private: 
	int suitVal;
	int valueVal;
	int pointsVal; 
	string owner;
public: 
	Card(){suitVal=0;valueVal=0;pointsVal=0;owner="";}
	Card(int cardSuit, int cardValue, int cardPoints);
	string toString();
	Card(int cardSuit, int cardValue, int cardPoints, string p);
	void makeOwner(string play);
	int suit(){return suitVal;}
	int value(){return valueVal;}
	int points(){return pointsVal;} 
	string whoOwns(){return owner;}
	bool isValid(){return (suitVal!=0) && (valueVal!=0);}
	string save();		
	void load(string serial);
};
#endif
