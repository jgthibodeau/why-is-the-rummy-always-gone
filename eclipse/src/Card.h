#ifndef CARD_H
#define CARD_H 

class Card { 
private: 
	int suitVal;
	int valueVal;
	int pointsVal;
public: 
	Card(int cardSuit, int cardValue, int cardPoints);
	int suit(){return suitVal;}
	int value(){return valueVal;}
	int points(){return pointsVal;}
};
#endif
