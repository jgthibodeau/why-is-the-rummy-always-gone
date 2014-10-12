#ifndef CARDSLOT_H
#define CARDSLOT_H
#include "Point.h"

class CardSlot {
private:
	Point positionPoint;
	int widthVal;
	int heightVal;
	//Card card;
public:
	CardSlot() : positionPoint(0,0){widthVal=0; heightVal=0;};
	CardSlot(double x, double y, int width, int height);

	// Extractors.
	Point position() { return positionPoint; }
	double width() { return widthVal; }
	double height() { return heightVal; }

	// Move the existing button.
	void move(double a, double b);

	//set card
	//remove card
};
#endif
