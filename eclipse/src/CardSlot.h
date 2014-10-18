#ifndef CARDSLOT_H
#define CARDSLOT_H
#include "Point.h"

class CardSlot {

	//types of slots
	static int deck;
	static int discard;
	static int player;
	static int combo;

private:
	Point positionPoint;
	int widthVal;
	int heightVal;
	bool highlight;
	int typeVal;
	//Card card;
public:
	CardSlot() : positionPoint(0,0){widthVal=0; heightVal=0; highlight=false; typeVal=0};
	CardSlot(double x, double y, int width, int height, int type);

	// Extractors
	Point position() { return positionPoint; }
	double width() { return widthVal; }
	double height() { return heightVal; }
	bool highlighted() { return highlight; }
	int type() { return type; }

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
