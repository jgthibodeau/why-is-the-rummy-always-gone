#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <list>
#include "Card.h"
using namespace std;

class Player
{
    private:
        string name;  ///player name
        int score, turnPhase; ///player score, turn phase
        list<Card> Hand;
        int turnphase;
        bool active;  ///is it player's turn?

    public:
        Player(string name, int score, int turnPhase, bool activity);

    ///constants for turnPhase
    static const int draw = 0;
    static const int play = 1;
    static const int knock = 2;
    static const int not_knocker = 3;

    ///set and get score
    int getScore() {return score;}
    void setScore(int newScore) {score = newScore;}

    ///set and get turn information
    int getTurnPhase() {return turnPhase;}
    void setTurnPhase(int newPhase) {turnPhase = newPhase;}
    void setActivity(bool activity) {active = activity;}

    ///card operations
    void addCard(Card card);
    void removeCard(int index);
    Card getCard(int index);
    int calculateScore();
    bool canKnock();


};

#endif
