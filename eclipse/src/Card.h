#ifndef CARD_H
#define CARD_H  
#include "Player.h"

class Card { 
private: 
	int suitVal;
	int valueVal;
	int pointsVal; 
	Player owner;
public: 
	Card(){suitVal=0;valueVal=0;pointsVal=0;}
	Card(int cardSuit, int cardValue, int cardPoints);
	Card(int cardSuit, int cardValue, int cardPoints, Player p);  
	string toString();
	void makeOwner(Player play);
	int suit(){return suitVal;}
	int value(){return valueVal;}
	int points(){return pointsVal;} 
	Player whoOwns(){return owner;}
};
#endif
