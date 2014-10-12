#include "CardSlot.h"
CardSlot::CardSlot(double x, double y, int width, int height) : positionPoint(x,y){
	widthVal = width;
	heightVal = height;
}

void CardSlot::move(double a, double b){
	positionPoint.move(a,b);
}
