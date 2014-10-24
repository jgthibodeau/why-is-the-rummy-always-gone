#include <iostream>
#include "Player.h"

using namespace std;

int main()
{

    ///create three cards
    Card card1(1,2,3);
    Card card2(2,3,4);
    Card card3(2,3,6);
    Card card4(2,3,5);
    Card card5(4,7,1);

    ///create single player
    Player player("mike", 0, 0, false);

    ///tests
    ///-------------------------------

    ///Try to remove card before any cards are in player's hand
    player.removeCard(0);

    ///add cards
    player.addCard(card1);
    player.addCard(card2);
    player.addCard(card3);

    ///expecting 3+4+6 = 13
    int i = player.calculateScore();

    ///remove card 3 (indexing begins at 0)
    player.removeCard(2);
    ///expecting 7
    i = player.calculateScore();

    ///should return error
    player.swapCard(1,2);

    player.swapCard(0,1);
    player.removeCard(1);
    ///expecting 4
    player.calculateScore();

    ///expecting to not knock
    player.canKnock();

    player.addCard(card4);
    ///expecting 9
    player.calculateScore();
    ///can knock
    player.canKnock();
    player.addCard(card5);
    ///cant knock
    player.canKnock();
}
