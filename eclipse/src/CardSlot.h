#ifndef CARDSLOT_H
#define CARDSLOT_H
#include "Point.h"

class CardSlot {
private:
	Point positionPoint;
	int widthVal;
	int heightVal;
	bool highlight;
	//Card card;
public:
	CardSlot() : positionPoint(0,0){widthVal=0; heightVal=0; highlight=false;};
	CardSlot(double x, double y, int width, int height);

	// Extractors
	Point position() { return positionPoint; }
	double width() { return widthVal; }
	double height() { return heightVal; }
	bool highlighted() {return highlight; }

	// set bounds
	void setBounds(int x, int y, int width, int height){ positionPoint.setPoint(x,y); widthVal=width;heightVal=height; }

	// Move the existing slot
	void move(double a, double b);

	// Set the highlight
	void setHighlight(bool b){ highlight=b; }

	//set card
	//remove card
};
#endif
