#include "Player.h"
#include <string>
using namespace std;

Player::Player(string name, int score, card[] cards, combo[] combos, int turnPhase, bool activity)
{
    name = name;
    score = score;
    cards = cards;
    combos = combos;
    turnPhase = turnPhase;
    activity = activity;
}

public int calculateScore(){
    int score = 0;
    int i;
    for (i=0; i<cards.length; i++){
        score += cards[i].value;
    }
    return score
}


public bool canKnock(){
    bool knock = false;
    int deadwood = calculateScore();
    if (deadwood < 10){
        knock = true;
    }
    return knock;
    }
}
