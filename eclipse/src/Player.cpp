#include "Player.h"
#include <string>
#include <iostream>
using namespace std;

Player::Player(string n, int s, int t, bool a)
{
    name = n;
    score = s;
    turnPhase = t;
    active = a;
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

void Player::removeCard(int index){
    if (index <1 || index > Hand.size()){
        cout << "sorry" << endl;
        return;
    }
    list<Card>::iterator it = Hand.begin();
    advance(it,index-1);
    it = Hand.erase(it);
}

 Card Player::getCard(int index){
    if (index <1 || index > Hand.size()){
        cout << "sorry" << endl;
    }
    list<Card>::iterator it = Hand.begin();
    advance(it,index-1);
    return *it;
}
