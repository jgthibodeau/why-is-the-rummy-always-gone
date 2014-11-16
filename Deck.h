#ifndef DECK_H
#define DECK_H
#include "Card.h"
#include <vector>

class Deck {
public:
  Deck();
  void initialize();
  void shuffle(); 
  bool isEmpty();
  Card drawCard(); 
  //convert to string for saving in datebase
  string save();
  //load player from database
  void load(string serial);
  std::vector<Card> cardDeck;
};

#endif
