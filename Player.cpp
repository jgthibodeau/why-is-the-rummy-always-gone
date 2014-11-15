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
    ai = false;
}
Player::Player(string name, int score, int turnPhase, bool activity, bool ai)
{
    name = name;
    score = score;
    turnPhase = turnPhase;
    activity = activity;
    ai = ai;
}

void initialize(){
	score = 0;
	turnPhase = 0;
	activity = false;
	ai = false;
	Hand.clear();
}

string save(){
    string s = "name=" + name + ";score=" + score + ";turnPhase=" + turnPhase + ";activity=" + activity + ";ai=" + ai + ";";
    //TODO add in cards from Hand
    for (list<Card>::iterator it = Hand.begin(); it != Hand.end(); ++it){
        s += "card="+(*it).save()+";";
    }
    return s;
}

void load(){
	//this.initialize();

    string delimeter = ";";
    string delimeter2 = "=";
    string full, name, var;
    int index1 = 0, index2 = 0;
    while ((index1 = serial.find(delimeter)) != -1) {
        full = serial.substr(0, index1);
        index2 = full.find(delimiter2);
        name = full.substr(0, index2);
        var = full.substr(index2+1,full.size());

	    if (name == "name") {
	    	name = var.c_str();
	    } else if (name == "score") {
	    	score = atoi(var.c_str());
	    } else if (name == "turnPhase") {
	    	turnPhase = atoi(var.c_str());
	    } else if (name == "activity") {
	    	activity = var.c_str()=="true";	//boolean?
	    } else if (name == "ai") {
	    	ai = var.c_str()=="true";	//boolean?
	    } else if (name == "card") {
	    	//make a card
	    	Card c = Card();
	    	//load this card and push it into the hand
	    	Hand.push_back(c.load(var));
	    } else {
	    	cout << "error" << endl;
	    }
    	serial.erase(0, index1 + delimeter.length());
    }
}

///calculates player score
int Player::calculateScore(){
    int score = 0;
    for (list<Card>::iterator it = Hand.begin(); it != Hand.end(); ++it){
        score += (*it).points();
    }
    //cout << "score is " << score << endl;
    return score;
}

///can player knock?
bool Player::canKnock(){
    bool knock = false;
    int deadwood = calculateScore();
    if (deadwood < 10){
        //cout << "player can knock, deadwood is " << deadwood << endl;
        knock = true;
    }
    //cout << "player can't knock, deadwood is " << deadwood << endl;
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
        //cout << "can't remove, hand is empty" << endl;
        return card;
    }
    if (index <0 || index > Hand.size()-1){
        //cout << "can't remove, card at given index does not exist " << endl;
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
        //cout << "can't return, card at given index does not exist " << endl;
        return card;
    }
    list<Card>::iterator it = Hand.begin();
    advance(it,index);
    return *it;
}

///returns size of player's current hand
int Player::handSize(){
    //cout << "hand size is " << Hand.size() << endl;
    return Hand.size();
}

 ///swaps the cards located in two indices in player's hand
void Player::swapCard(int index1, int index2){
    if((index1 >= Hand.size() || index2 >= Hand.size()) || (index1<0 ||index2<0)){
        //cout << "one of the indices is either too large or negative" << endl;
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
