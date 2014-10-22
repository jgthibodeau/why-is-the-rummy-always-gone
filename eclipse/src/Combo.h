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
  bool addCard(Card c); 
  string toString();
  std::deque<Card> comboSet; 
}; 

#endif