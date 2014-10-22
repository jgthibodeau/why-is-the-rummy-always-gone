#include "Combo.h" 
#include "Card.h" 

Combo::Combo(){  
	isSet = false;
	initialize();
} 

void Combo::initialize(){ 
	comboSet.clear();
} 

bool Combo::addCard(Card c){  //returns true if a card is sucessefully added, returns false if card is not added
	if (comboSet.empty()){  				// if no card in combo, put the card in
		comboSet.push_back(c); 
		return true;
	} 
	else if (comboSet.size() == 1){  		// if one card check if the new card is making a set or run
		if (comboSet.front().value() == c.value()){ 
			isSet = true; 
			comboSet.push_back(c); 
		} 
		else{ //check runs
			if((c.value() == comboSet.front().value()+1) && (c.suit() == comboSet.front().suit())) { 
				isSet = false; 
				comboSet.push_back(c); 
				return true;
			} 
			else if ((c.value() == comboSet.front().value()-1) && (c.suit() == comboSet.front().suit())){ 
				isSet = false; 
				comboSet.push_front(c); 
				return true;
			} 
		}
	} 
	else{  //more than 1 card in combo, check if set or run
		if(isSet){ 
			if( c.value() == comboSet.front().value()){  //if set check if value is same as current set
				comboSet.push_front(c); 
				return true;
			}
		} 
		else{ //must be a run
			if((c.value() == comboSet.back().value()+1) && (c.suit() == comboSet.front().suit())) { //combo is a run check suit and values on front and back 
				comboSet.push_back(c); 
				return true; 
			} 
			else if((c.value() == comboSet.front().value()-1) && (c.suit() == comboSet.front().suit())){ 
				comboSet.push_front(c); 
				return true; 
			} 
			else return false; //card doesnt belong
		}
	}
}
	