#ifndef CARD_H
#define CARD_H 

class Card { 
private: 
	int suit; 
	int value; 
	int points; 
public: 
	Card(int cardSuit, int cardValue, int cardPoints);
	int suit(){return suit;} 
	int value(){return value;} 
	int points(){return points;}
};
#endif