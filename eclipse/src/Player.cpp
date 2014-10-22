#include "Player.h"
#include <string>
#include <iostream>
using namespace std;

Player::Player(string name, int score, int turnPhase, bool activity)
{
    name = name;
    score = score;
    turnPhase = turnPhase;
    activity = activity;
}


///calculates player score
int Player::calculateScore(){
    int score = 0;
    for (list<Card>::iterator it = Hand.begin(); it != Hand.end(); ++it){
        score += (*it).points();
    }

    return score;
}

///can player knock?
bool Player::canKnock(){
    bool knock = false;
    int deadwood = calculateScore();
    if (deadwood < 10){
        knock = true;
    }
    return knock;
}

///card to player
void Player::addCard(Card card){
    Hand.push_back(card);
}

///remove card from hand at index
Card Player::removeCard(int index){
    if (index <0 || index > Hand.size()-1){
        Card card(0,0,0);
        return card;
    }
    list<Card>::iterator it = Hand.begin();
    advance(it,index);
    Card temp = *it;
    it = Hand.erase(it);
    return temp;
}

///returns card from hand at index without deleting
 Card Player::getCard(int index){
    if (index <0 || index > Hand.size()-1){
        Card card(0,0,0);
        return card;
    }
    list<Card>::iterator it = Hand.begin();
    advance(it,index);
    return *it;
}

///returns size of player's current hand
int Player::handSize(){
    return Hand.size();
}

 ///swaps the cards located in two indices in player's hand
void Player::swapCard(int index1, int index2){
    if(index1 >= Hand.size() || index2 >= Hand.size())){
        return;
    }
    list<Card>::iterator card1 = Hand.begin();
    list<Card>::iterator card2 = Hand.begin();
    advance(card1,index1);
    advance(card2,index2);
    Card temp = *card1;
    *card1 = *card2;
    *card2 = temp;
}
