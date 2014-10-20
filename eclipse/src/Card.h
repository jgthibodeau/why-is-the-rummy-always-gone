#ifndef CARD_H
#define CARD_H 

class Card { 
private: 
	int suitVal;
	int valueVal;
	int pointsVal;
public: 
	Card(int cardSuit, int cardValue, int cardPoints);
	int getSuit(){return suitVal;}
	int getValue(){return valueVal;}
	int getPoints(){return pointsVal;}
};
#endif
