#ifndef DISCARDPILE_H
#define DISCARDPILE_H 
#include "Card.h" 
#include <vector> 

class DiscardPile {
public:
  DiscardPile();
  void initialize(); 
  void addCard(Card c);
  Card removeCard(); 
  Card topCard();
  std::vector<Card> discardDeck;
};

#endif