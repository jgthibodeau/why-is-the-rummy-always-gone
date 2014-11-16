#include "Deck.h"  
#include "Card.h" 
#include <algorithm> // std::random_shuffle 
#include <ctime>   	// std::time 
#include <vector>

Deck::Deck(){ 
	initialize(); 
} 

void Deck::initialize(){ 
	cardDeck.clear(); //make sure we are starting with an empty deck
	int suit, value;  
	for (suit = 1; suit < 5; suit++){
		for (value = 1; value <14; value++){  
			if (value <= 10){
			cardDeck.push_back(Card(suit,value,value)); 
			} 
			else{ 
			cardDeck.push_back(Card(suit,value,10));
			}
		}
	}
} 

void Deck::shuffle(){ 
	std::srand(unsigned(std::time(0))); 
	std::random_shuffle(cardDeck.begin(),cardDeck.end());
} 

bool Deck::isEmpty(){ 
	return cardDeck.empty();
}

Card Deck::drawCard(){ 
	Card topCard = cardDeck.front(); //front returns a reference to the first card
	cardDeck.erase(cardDeck.begin()); //begin returns the interator to the first card
	return topCard;
}

string Deck::save(){
    ostringstream s;
    for (vector<Card>::iterator it = cardDeck.begin(); it != cardDeck.end(); ++it){
        s <<"card="<<(*it).save()<<";";
    }
    return s.str();
}

void Deck::load(string serial){
    cardDeck.clear();

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
            cardDeck.push_back(c);
        } else {
            cout << "error" << endl;
        }
        serial.erase(0, index1 + delimeter.length());
    }
}