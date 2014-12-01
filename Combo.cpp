#include "Combo.h" 
#include "Card.h"  
#include "Player.h"
using namespace std;

Combo::Combo(){  
	isSet = false;
	initialize();
} 

void Combo::initialize(){ 
	comboSet.clear();
}   

bool Combo::isEmpty(){ 
	return comboSet.empty();
}   

bool Combo::isValid(){ 
	return (comboSet.size() >= 3 || comboSet.size() == 0);
} 

void Combo::returnCardsToPlayer(Player p){ 
	cout << "starting to return cards" << endl;
	for(deque<Card>::iterator it = comboSet.begin(); it != comboSet.end();){

		cout << "next iteration" << endl;
		if ((*it).whoOwns().compare(p.getName()) == 0){ //if card is owned by the person requesting cards back

			cout << "this card is owned" << endl;
			Card returnedCard; 
			returnedCard = (*it); //get card from deque
			cout << "temporarily have it" << endl;
			comboSet.erase(it); //erase card from deque
			cout << "erased ok" << endl;
			p.addCard(returnedCard); //place removed card in players hand
			cout << "gave to player" << endl;
		}
		else
			++it;
	}
}

Card Combo::showCard(){ 
	return comboSet.back();
}

Card Combo::removeCard(){ 
	Card topCard = comboSet.back(); //end returns a reference to the last card
	comboSet.erase(comboSet.end()); //begin returns the interator to the first card
	return topCard;
}

string Combo::toString(){ 
	string output = ""; 
	for (deque<Card>::iterator it = comboSet.begin(); it != comboSet.end(); ++it){
		output = output + (*it).toString();
	} 
	return output;
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
	
string Combo::save(){
    ostringstream s;
    for (deque<Card>::iterator it = comboSet.begin(); it != comboSet.end(); ++it){
        s <<"card="<<(*it).save()<<";";
    }
    return s.str();
}

void Combo::load(string serial){
    initialize();

    string delimeter = ";";
    string delimeter2 = "=";
    string full, name, var;
    int index1 = 0, index2 = 0;
    while ((index1 = serial.find(delimeter)) != -1) {
        full = serial.substr(0, index1);
        index2 = full.find(delimeter2);
        name = full.substr(0, index2);
        var = full.substr(index2+1,full.size());
		if (name == "card") {
            //make a card
            Card c = Card();
            //load this card and push it into the hand
            c.load(var);
            comboSet.push_back(c);
        } else {
            cout << "error" << endl;
        }
        serial.erase(0, index1 + delimeter.length());
    }
}