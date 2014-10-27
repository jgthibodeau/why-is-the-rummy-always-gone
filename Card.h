#ifndef CARD_H
#define CARD_H
#include <string>
using namespace std;
//#include "Player.h"
class Player;

class Card { 
private: 
	int suitVal;
	int valueVal;
	int pointsVal; 
	Player* owner;
public: 
	Card(){suitVal=0;valueVal=0;pointsVal=0;owner=NULL;}
	Card(int cardSuit, int cardValue, int cardPoints);
	string toString();
	Card(int cardSuit, int cardValue, int cardPoints, Player* p);
	void makeOwner(Player* play);
	int suit(){return suitVal;}
	int value(){return valueVal;}
	int points(){return pointsVal;} 
	Player* whoOwns(){return owner;}
	bool isValid(){return (suitVal!=0) && (valueVal!=0);}
};
#endif
