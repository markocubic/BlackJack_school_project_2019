#ifndef BLACKJACK_H
#define BLACKJACK_H
#include <vector>
#include <string>
using namespace std;

class Blackjack
{
    public:
        Blackjack();
        ~Blackjack();
        void playBlackjack();
        unsigned int playerPoints();
    private:
        //METODE
        void gameRound();
        void roundSetup();
        void setShuffle();
        void printPlayerPoints() const;
        void placeYourBid();
        void checkBid();
        bool isInputValidated();
        void firstDeal();
        void dealCards(vector<string> &hand);
        void sortHand(vector<string> &hand);
        void countHand(unsigned int &strength, vector<string> &hand);
        void printHand() const;
        void checkBlackjack();
        void printHandOpen() const;
        void hitOrStand();
        void checkInput();
        void willHouseHit();
        bool isSoftSeventeen();
        bool isBust(unsigned int &strength);
        void houseBust();
        void roundWinner();
        void playerHit();
        void leaveStay();
        void printDeck() const;
        //VARIJABLE KLASE
        vector<string> _deck;
        vector<string> _player;
        vector<string> _house;
        bool _play;
        bool _round_flag;
        unsigned int _playerpts;
        unsigned int _rounds;
        unsigned int _player_total;
        unsigned int _house_total;
        unsigned int _bid;
        bool _input;
};

#endif // BLACKJACK_H
