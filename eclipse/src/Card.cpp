#include "Card.h"  
using namespace std; 
#include <string>

 Card::Card(int cardSuit, int cardValue, int cardPoints){
 	suitVal = cardSuit;
 	valueVal = cardValue;
 	pointsVal = cardPoints;
 }
 Card::Card(int cardSuit, int cardValue, int cardPoints, Player p){ 
 	suitVal = cardSuit;
 	valueVal = cardValue;
 	pointsVal = cardPoints; 
 	owner = p;
 }

void Card::makeOwner(Player play){ 
owner = play;
} 

string Card::toString(){ 
	string out= ""; 
	switch(valueVal){ 
		case 13: 
			out= out +"[King of "; 
			break; 
		case 12: 
			out= out +"[Queen of "; 
			break; 
		case 11: 
			out= out +"[Jack of "; 
			break; 
		case 1: 
			out= out +"[Ace of "; 
			break; 
		default: 
			out= out + "[" +to_string(valueVal)+ " of "; 
			break;
	} 
	switch(suitVal){ 
		case 1: 
			out= out +"Spades]"; 
			break; 
		case 2: 
			out= out +"Hearts]"; 
			break; 
		case 3: 
			out= out +"Clubs]"; 
			break; 
		case 4: 
			out= out +"Diamonds]"; 
			break; 
		default: 
			break;
	}
}
