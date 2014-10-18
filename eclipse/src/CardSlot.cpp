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

int CardSlot::deck = deck;
int CardSlot::discard = discard;
int CardSlot::player = player;
int CardSlot::combo = combo;
