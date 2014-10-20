#include "Deck.h"  
#include "Card.h" 
#include <algorithm> // std::random_shuffle 
#include <ctime>   	// std::time

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

void Deck::shuffle(){ 
	std::srand(unsigned(std::time(0))); 
	std::random_shuffle(cardDeck.begin(),cardDeck.end());
}