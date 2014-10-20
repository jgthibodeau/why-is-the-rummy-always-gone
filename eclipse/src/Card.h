#ifndef CARD_H
#define CARD_H 

class Card { 
private: 
	int suit; 
	int value; 
	int points; 
public: 
	Card(int cardSuit, int cardValue, int cardPoints);
	int getsuit(){return suit;} 
	int getvalue(){return value;} 
	int getpoints(){return points;}
};
#endif