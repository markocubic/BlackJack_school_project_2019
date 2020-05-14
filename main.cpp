#include <iostream>
#include "blackjack.h"
#include <random>
#include <time.h>

using namespace std;

int main()
{
    srand(time(0));
    Blackjack game;
    game.playBlackjack();

    cout<<"\nPlayer is leaving the table with: "<<game.playerPoints()<<" pts"<<endl;
}
