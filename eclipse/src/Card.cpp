#include "Card.h"  
 Card::Card(int cardSuit, int cardValue, int cardPoints){
 	suitVal = cardSuit;
 	valueVal = cardValue;
 	pointsVal = cardPoints;
 }
 Card::Card(int cardSuit, int cardValue, int cardPoints, Player* p){
 	suitVal = cardSuit;
 	valueVal = cardValue;
 	pointsVal = cardPoints; 
 	owner = p;
 }

void Card::makeOwner(Player* play){
owner = play;
}
