#ifndef CARDSLOT_H
#define CARDSLOT_H
#include "Point.h"
#include "Card.h"
#include <cstdlib>

class CardSlot {

private:
	Point positionPoint;
	int widthVal;
	int heightVal;
	bool highlight;
	int typeVal;
	Card *cardP;

public:
	//types of slots
	static const int deck = 0;
	static const int discard = 1;
	static const int player = 2;
	static const int combo = 3;

	CardSlot() : positionPoint(0,0){widthVal=0; heightVal=0; highlight=false; typeVal=0; cardP=NULL;};
	CardSlot(double x, double y, int width, int height, int type);
	CardSlot(double x, double y, int width, int height, int type, Card *c);

	// Extractors
	Point position() { return positionPoint; }
	double width() { return widthVal; }
	double height() { return heightVal; }
	bool highlighted() { return highlight; }
	int type() { return typeVal; }
	Card *getCard() { return cardP; }

	// Setters
	void setBounds(int x, int y, int width, int height){ positionPoint.setPoint(x,y); widthVal=width;heightVal=height; }
	void move(double a, double b){ positionPoint.move(a,b); }
	void setHighlight(bool b){ highlight=b; }
	void setCard(Card *c){ cardP = c; }
};
#endif
