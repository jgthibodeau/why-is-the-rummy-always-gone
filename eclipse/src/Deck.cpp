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

Card Deck::drawCard(){ 
	Card topCard = cardDeck.front(); //front returns a reference to the first card
	cardDeck.erase(cardDeck.begin()); //begin returns the interator to the first card
	return topCard;
}
