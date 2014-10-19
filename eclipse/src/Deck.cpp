#include "Deck.h"  
#include "Card.h"

Deck::Deck(){ 
	initialize(); 
} 

void Deck::initialize(){ 
	cardDeck.clear(); //make sure we are starting with an empty deck
	int suit, value;  
	for (suit = 1; suit < 5; suit++){ //check these suit numbers
		for (value = 1; value <14; value++){ 
			cardDeck.push_back(Card(suit,value,value));
		}
	}
}