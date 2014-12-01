#include "Player.h"
#include <string>
#include <iostream>
using namespace std;

Player::Player(string name, int score, int turnPhase, bool activity)
{
    this->name = name;
    this->score = score;
    this->turnPhase = turnPhase;
    this->active = activity;
    this->ai = false;
}
Player::Player(string name, int score, int turnPhase, bool activity, bool ai)
{
    this->name = name;
    this->score = score;
    this->turnPhase = turnPhase;
    this->active = activity;
    this->ai = ai;
}

void Player::initialize(){
	score = 0;
	turnPhase = 0;
	active = false;
	Hand.clear();
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
    if (deadwood < 1000){
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
    Card card(0,0,0);
    if (Hand.empty()){
        return card;
    }
    if (index <0 || index > Hand.size()-1){
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
    if((index1 >= Hand.size() || index2 >= Hand.size()) || (index1<0 ||index2<0)){
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

string Player::save(){
    ostringstream s;
    s << "name=" << name << ";score=" << score << ";turnPhase=" << turnPhase << ";active=" << active << ";ai=" << ai << ";";
    //TODO add in cards from Hand
    for (list<Card>::iterator it = Hand.begin(); it != Hand.end(); ++it){
        s <<"card="<<(*it).save()<<";";
    }
    return s.str();
}

void Player::load(string serial){
    initialize();

    string delimeter = ";";
    string delimeter2 = "=";
    string full, name, var;
    int index1 = 0, index2 = 0;
    while ((index1 = serial.find(delimeter)) != -1) {
        full = serial.substr(0, index1);
        index2 = full.find(delimeter2);
        name = full.substr(0, index2);
        var = full.substr(index2+1,full.size());
        if (name == "name") {
            this->name = var;
        } else if (name == "score") {
            this->score = atoi(var.c_str());
        } else if (name == "turnPhase") {
            this->turnPhase = atoi(var.c_str());
        } else if (name == "active") {
            this->active = var.c_str()=="true"; //boolean?
        } else if (name == "ai") {
            this->ai = var.c_str()=="true";   //boolean?
        } else if (name == "card") {
            //make a card
            Card c = Card();
            //load this card and push it into the hand
            c.load(var);
            Hand.push_back(c);
        } else {
            cout << "error" << endl;
        }
        serial.erase(0, index1 + delimeter.length());
    }
}


