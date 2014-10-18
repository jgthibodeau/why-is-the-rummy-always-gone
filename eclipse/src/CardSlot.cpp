#include "CardSlot.h"
CardSlot::CardSlot(double x, double y, int width, int height, int type) : positionPoint(x,y){
	widthVal = width;
	heightVal = height;
	typeVal = type;
	highlight = false;
}

void CardSlot::move(double a, double b){
	positionPoint.move(a,b);
}

int CardSlot::deck = 0;
int CardSlot::discard = 1;
int CardSlot::player = 2;
int CardSlot::combo = 3;
