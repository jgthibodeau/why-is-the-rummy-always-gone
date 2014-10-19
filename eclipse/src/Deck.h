#ifndef DECK_H
#define DECK_H
#include "Card.h"
#include <vector>


class Deck {
public:
  Deck();
  void initialize();
  void shuffle();
  std::vector<Card> cardDeck;
};

#endif