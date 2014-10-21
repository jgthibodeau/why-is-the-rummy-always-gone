#ifndef PLAYER_H
#define PLAYER_H
#include <string>
using namespace std;

class Player
{
    private:
        string name;  ///player name
        int score, turnPhase; ///player score, turn phase
        card[] cards; ///player's cards
        combo[] combos; ///tracks players combos
        int turnphase
        bool active;  ///is it player's turn?

    public:
        Player(string name, int score, card[] cards, combo[] combos, int turnPhase, bool activity);

    static const int draw = 0;
    static const int play = 1;
    static const int knock = 2;
    static const int not_knocker = 3;

    ///set and get score
    int getScore() {return score;}
    void setScore(int newScore) {score = newScore;}

    ///
    int getTurnPhase() {return turnPhase;}
    void setTurnPhase(int newPhase) {turnPhase = newPhase;}

    void setActivity(bool activity) {active = activity;}

};
