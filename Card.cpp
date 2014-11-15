#include "Card.h"
#include <string>
#include <sstream>
using namespace std; 

 Card::Card(int cardSuit, int cardValue, int cardPoints){
 	suitVal = cardSuit;
 	valueVal = cardValue;
 	pointsVal = cardPoints;
 	owner = "";
 }
 Card::Card(int cardSuit, int cardValue, int cardPoints, string p){
 	suitVal = cardSuit;
 	valueVal = cardValue;
 	pointsVal = cardPoints; 
 	owner = p;
 }

void Card::makeOwner(string play){
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
			std::stringstream ss;
			ss << valueVal;
			out= out + "[" +ss.str()+ " of ";
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
	return out;
}

string Card::save(){
	ostringstream s;
	s << "suit=" << suitVal << ",value=" << valueVal << ",points=" << pointsVal << ",owner=" << owner + ",";
	return s.str();
}
	
void Card::load(string serial){
    string delimeter = ",";
    string delimeter2 = "=";
    string full, name, var;
    int index1 = 0, index2 = 0;
    while ((index1 = serial.find(delimeter)) != -1) {
        full = serial.substr(0, index1);
        index2 = full.find(delimeter2);
        name = full.substr(0, index2);
        var = full.substr(index2+1,full.size());

	    if (name == "suit") {
	    	suitVal = atoi(var.c_str());
	    } else if (name == "value") {
	    	valueVal = atoi(var.c_str());
	    } else if (name == "points") {
	    	pointsVal = atoi(var.c_str());
	    } else if (name == "onwer") {
	    	owner= var.c_str();
	    } else {
	    	cout << "error" << endl;
	    }
    	serial.erase(0, index1 + delimeter.length());
    }
}
