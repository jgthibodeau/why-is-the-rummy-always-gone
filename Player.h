#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <list>
#include "Card.h"
#include <sstream>
#include <iostream>
#include <stdlib.h>
using namespace std;

class Player
{
    private:
        string name;  ///player name
        int score, turnPhase; ///player score, turn phase
        list<Card> Hand;
        bool active;  ///is it player's turn?
        bool ai;

    public:
        Player(){name="";score=0;turnPhase=0;active=false;ai=false;}
        Player(string name, int score, int turnPhase, bool activity);
        Player(string name, int score, int turnPhase, bool activity, bool ai);

    ///constants for turnPhase
    static const int draw = 0;
    static const int play = 1;
    static const int knock = 2;
    static const int not_knocker = 3;

    ///set and get score
    int getScore() {return score;}
    void setScore(int newScore) {score = newScore;}
    void setAI(bool a) {ai = a;}

    ///set and get turn information
    int getTurnPhase() {return turnPhase;}
    void setTurnPhase(int newPhase) {turnPhase = newPhase;}
    void setActivity(bool activity) {active = activity;}
    bool isActive() {return active;}
    bool isAI() {return ai;}

    ///set name
    void setName(string newName) {name = newName;}
    string getName() {return name;}
    ///add Card to hand
    void addCard(Card card);

    ///remove Card from hand at index
    Card removeCard(int index);

    ///returns Card from hand at index without deleting
    Card getCard(int index);

    ///calculates score for player
    int calculateScore();

    ///returns if a player can knock
    bool canKnock();

    ///returns the number of cards in the player's current hand
    int handSize();

    ///swaps two cards
    void swapCard(int index1, int index2);

    //set all vars to nothing!
    void initialize();

    //convert to string for saving in datebase
    string save();
    //load player from database
    void load(string serial);
};

#endif
