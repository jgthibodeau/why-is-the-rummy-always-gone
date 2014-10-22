#ifndef COMBO_H 
#define COMBO_H  
#include "Card.h"
#include <deque>

class Combo {   
private: 
	bool isSet;
public:
  Combo();
  void initialize();   
  Card removeCard(); 
  Card showCard();
  bool isEmpty();
  bool addCard(Card c); 
  string toString();
  std::deque<Card> comboSet; 
}; 

#endif