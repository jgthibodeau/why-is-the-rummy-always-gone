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
  //convert to string for saving in datebase
  string save();
  //load player from database
  void load(string serial);
  Card removeCard();  
  Card showCard(); 
  bool isValid();
  bool isEmpty();
  void returnCardsToPlayer(Player* player);
  bool addCard(Card c); 
  string toString();
  std::deque<Card> comboSet; 
}; 

#endif
