#include "CardSlot.h"
CardSlot::CardSlot(double x, double y, int width, int height, int type) : positionPoint(x,y){
	widthVal = width;
	heightVal = height;
	typeVal = type;
	highlight = false;
	cardP = NULL;
}
CardSlot::CardSlot(double x, double y, int width, int height, int type, Card *c) : positionPoint(x,y){
	widthVal = width;
	heightVal = height;
	typeVal = type;
	highlight = false;
	cardP = c;
}
