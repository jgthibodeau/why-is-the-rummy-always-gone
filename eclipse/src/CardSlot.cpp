#include "CardSlot.h"
CardSlot::CardSlot(double x, double y, int width, int height, int type) : positionPoint(x,y){
	widthVal = width;
	heightVal = height;
	typeVal = type;
	highlight = false;
	indexVal = 0;
}
CardSlot::CardSlot(double x, double y, int width, int height, int type, int index) : positionPoint(x,y){
	widthVal = width;
	heightVal = height;
	typeVal = type;
	highlight = false;
	indexVal = index;
}
