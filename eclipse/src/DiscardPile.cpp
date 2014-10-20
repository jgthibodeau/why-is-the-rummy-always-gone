#include "DiscardPile.h" 
#include "Card.h" 

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

Card DiscardPile::removeCard(){ 
	Card topCard = discardDeck.back(); //end returns a reference to the last card
  discardDeck.erase(discardDeck.end()); //begin returns the interator to the first card
  return topCard;
} 

Card DiscardPile::topCard(){ 
	Card top = discardDeck.back(); 
	return top;
}