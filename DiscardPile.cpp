#include "DiscardPile.h" 
#include "Card.h"  
#include <vector>

DiscardPile::DiscardPile(){ 
	initialize();
} 

void DiscardPile::initialize(){ 
	discardDeck.clear(); //clear the discard pile
} 

// The "top" of the discard pile is the back of the vector 
// this makes it so we dont have to push everthing back if we were inserting new cards to the front

void DiscardPile::addCard(Card c){ 
	discardDeck.push_back(c);
}

bool DiscardPile::isEmpty(){ 
	return discardDeck.empty();
}

Card DiscardPile::removeCard(){ 
	Card topCard = discardDeck.back(); //end returns a reference to the last card
	discardDeck.erase(discardDeck.end()); //begin returns the interator to the first card
	return topCard;
} 

Card DiscardPile::topCard(){ 
	Card top = discardDeck.back(); 
	return top;
}

string DiscardPile::save(){
    ostringstream s;
    for (vector<Card>::iterator it = discardDeck.begin(); it != discardDeck.end(); ++it){
        s <<"card="<<(*it).save()<<";";
    }
    return s.str();
}

void DiscardPile::load(string serial){
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
            discardDeck.push_back(c);
        } else {
            cout << "error" << endl;
        }
        serial.erase(0, index1 + delimeter.length());
    }
}