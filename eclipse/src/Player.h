#ifndef PLAYER_H
#define PLAYER_H

#include "Card.h"
#include <string>
#include <list>
using namespace std;
//class Card;

class Player
{
    private:
        string name;  ///player name
        int score, turnPhase; ///player score, turn phase
        bool active;  ///is it player's turn?
        list<Card> Hand;

    public:
        Player(){name="";score=0;turnPhase=0;active=false;}
        Player(string name, int score, int turnPhase, bool active);

    ///constants for turnPhase
    static const int draw = 0;
    static const int play = 1;
    static const int knock = 2;
    static const int not_knocker = 3;

    ///set and get score
    int getScore() {return score;}
    void setScore(int newScore) {score = newScore;}

    ///set and get turn
    int getTurnPhase() {return turnPhase;}
    void setTurnPhase(int newPhase) {turnPhase = newPhase;}
    void setActivity(bool activity) {active = activity;}


    void addCard(Card card);
    void removeCard(int index);
    int calculateScore();
    bool canKnock();


};

#endif
