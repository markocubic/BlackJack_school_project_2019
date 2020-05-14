#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <math.h>
#include <dos.h>
#include <thread>
#include <chrono>
#include "blackjack.h"

using namespace std;

Blackjack::Blackjack(){
    cout<<"Game starts!"<<endl;
    _playerpts = 1000;
    _play=1;
    _rounds=0;
    setShuffle();
}
Blackjack::~Blackjack(){
    cout<<"Game ends!"<<endl;
}
void Blackjack::playBlackjack(){
    while (_play){
        gameRound();
    }
}
void Blackjack::gameRound(){
    roundSetup();
    placeYourBid();
    firstDeal();
    checkBlackjack();
    while(_round_flag==1)
    {
        hitOrStand();
        if(_input==0)//if STAND
        {
            willHouseHit();//odlucuje hoce li house raditi HIT
            if(isBust(_house_total))
                houseBust();
            else
                roundWinner();
        }
        else if(_input==1)//if HIT
            playerHit();
    }
    printPlayerPoints();
    if (_playerpts>0)
        leaveStay();
    else
        _play=0;
}
void Blackjack::roundSetup(){
    _round_flag=1;
    _rounds++;//_roundCounter
    _player_total=0;
    _house_total=0;
    cout<<"________________________________________________"<<endl;
    cout<<"Round: "<<_rounds<<"\n"<<endl;
    if(_rounds%5==0){//Spil se misa svakih 5 rundi
        vector<string>().swap(_deck);
        setShuffle();
        cout<<" SHUFFLING DECK...\n"<<endl;
        this_thread::sleep_for (chrono::seconds(1));
    }
}
void Blackjack::setShuffle(){
    if(!_deck.empty())
        vector<string>().swap(_deck);

    unsigned int br=0;
    vector<string> niz = {" clubs", " diamonds", " hearts", " spades"};
    for(unsigned int i=0; i<52; i++)
    {
        unsigned int card = i+1-13*br;
        if(card==1)
            _deck.push_back('A' + niz.at(br));
        else if(card==11)
            _deck.push_back('J' + niz.at(br));
        else if(card==12)
            _deck.push_back('Q' + niz.at(br));
        else if(card==13){
            _deck.push_back('K' + niz.at(br));
            br++;
        }
        else
            _deck.push_back(to_string(card) + niz.at(br));
    }
    random_shuffle(_deck.begin(), _deck.end());
}
void Blackjack::printPlayerPoints() const{
    cout<<" You have "<<_playerpts<<" points."<<endl;
}
void Blackjack::placeYourBid(){
    printPlayerPoints();
    cout<<" Place your bid: ";
    cin>>_bid;
    checkBid();
}
void Blackjack::checkBid(){
    if (_playerpts-_bid<10 && _playerpts-_bid!=0){
        cout<<" Your bid is set to all in! Your bid is set to "<<_playerpts<<"!"<<endl;
        _bid = _playerpts;
    }
    while(_bid>_playerpts || _bid<10 || isInputValidated()==0)
    {
        cout<<" You have entered a wrong value."<<endl;
        cout<<" Please place your bid correctly: ";
        cin>>_bid;
    }
}
bool Blackjack::isInputValidated(){
    if(cin.fail())
    {
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        return 0;
    }
    return 1;
}
void Blackjack::firstDeal(){
    cout<<"\n            DEALING..."<<endl;
    this_thread::sleep_for (chrono::seconds(1));
    dealCards(_player);
    dealCards(_house);
    dealCards(_player);
    dealCards(_house);
    printHand();
}
void Blackjack::dealCards(vector<string> &hand){
    hand.push_back(_deck.at(_deck.size()-1));
    _deck.erase(_deck.end()-1);
    sortHand(hand);//stavlja A na kraj tako da znamo vrijedi li 1pt ili 11pts
    countHand(_player_total, _player);
    countHand(_house_total, _house);
}
void Blackjack::sortHand(vector<string> &hand){
    int br=1;
    for(unsigned int i=0; i<=hand.size()-br; i++)
    {
        string temp;
        if(hand.at(i)[0]=='A' && i < hand.size()-br){
            while(hand.at(hand.size()-br)[0] == 'A' && hand.size()-br != 0){
                cout<<hand.size()-br<<endl;
                br++;
            }
            if(i > hand.size()-br)
                break;
            temp=hand.at(i);
            hand.at(i)=hand.at(hand.size()-br);
            hand.at(hand.size()-br)=temp;
        }
        else
            break;
        br=1;
    }
}
void Blackjack::countHand(unsigned int &strength, vector<string> &hand){
    strength=0;
    for(unsigned int i=0; i<hand.size(); i++)
    {
        if (isdigit(hand.at(i)[0])){
            int a=hand.at(i)[0] - '0';
            if (a==1)
                strength=strength+10;
            else
                strength=strength+a;
        }
        else if(hand.at(i)[0]=='A'){
            if(strength+11 > 21)
                strength=strength+1;
            else if(strength+11 <= 21)
                strength=strength+11;
        }
        else if(hand.at(i)[0]=='J')
            strength=strength+10;
        else if(hand.at(i)[0]=='Q')
            strength=strength+10;
        else if(hand.at(i)[0]=='K')
            strength=strength+10;
    }
}
void Blackjack::printHand() const{
    cout<<"\n Your hand: ";
    for(unsigned int i=0; i<_player.size(); i++){
        cout<<"'"<<_player.at(i)<<"'"<<", ";
    }
    cout<<"\n House hand: "<<"'unknown card', ";
    for(unsigned int i=1; i<_house.size(); i++){
        cout<<"'"<<_house.at(i)<<"'"<<", ";
    }
    cout<<endl;
}
void Blackjack::checkBlackjack(){
    if(_player_total==21){
        _playerpts=_playerpts+_bid;
        cout<<"\n BLACKJACK!!!"<<endl;
        printHandOpen();
        cout<<"\n PLAYER WINS: "<<_bid<<" pts"<<endl;
        _round_flag=0;
        return;
    }
}
void Blackjack::printHandOpen() const{
    cout<<"\n Your hand: ";
    for(unsigned int i=0; i<_player.size(); i++){
        cout<<"'"<<_player.at(i)<<"'"<<", ";
    }
    cout<<"\n House hand: ";
    for(unsigned int i=0; i<_house.size(); i++){
        cout<<"'"<<_house.at(i)<<"'"<<", ";
    }
    cout<<endl;
}
void Blackjack::hitOrStand(){
    cout<<endl;
    cout<<" type 0 to STAND"<<endl;
    cout<<" type 1 to HIT"<<endl;
    cout<<"\n your choice: ";
    cin>>_input;
    checkInput();
    if(_input==0)
        cout<<"\n Player: STAND"<<endl;
    else
        cout<<"\n Player: HIT"<<endl;
}
void Blackjack::checkInput(){
    while ((_input != 0 && _input != 1) || isInputValidated() == 0){
        cout<<" You have entered a wrong number.\n Please enter 0 or 1:";
        cin>>_input;
    }
}
void Blackjack::willHouseHit(){
    printHandOpen();
    while(_house_total<17 || isSoftSeventeen()){
        if(_player_total<_house_total)
            return;
        dealCards(_house);
        this_thread::sleep_for (chrono::seconds(1));
        cout<<"\n House: HIT"<<endl;
        printHandOpen();
    }
}
bool Blackjack::isSoftSeventeen(){
    for(unsigned int i=0; i<_house.size(); i++)
    {
        if(_house.at(i)[0] == 'A' && _house_total==17)
            return 1;
    }
    return 0;
}
bool Blackjack::isBust(unsigned int &strength){
    if(strength>21){
        cout<<"\n BUST! "<<endl;
        _round_flag=0;
        return 1;
    }
    return 0;
}
void Blackjack::houseBust(){
    _playerpts=_playerpts+_bid;
    cout<<"\n PLAYER WINS: "<<_bid<<" pts"<<endl;
    _round_flag=0;
}
void Blackjack::roundWinner(){
    cout<<"\n House: STAND"<<endl;
    cout<<"\n            COUNTING..."<<endl;
    if(_player_total>_house_total){
        cout<<"\n PLAYER WINS: "<<_bid<<" pts"<<endl;
        _playerpts=_playerpts+_bid;
    }
    else if(_player_total<_house_total){
        cout<<"\n HOUSE WINS. PLAYER LOSES: "<<_bid<<" pts"<<endl;
        _playerpts=_playerpts-_bid;
    }
    else if(_player_total==_house_total)
        cout<<"\n PUSH. Players bid is returned!"<<endl;
    _round_flag=0;
}
void Blackjack::playerHit(){
    cout<<"\n            DEALING..."<<endl;
    this_thread::sleep_for (chrono::seconds(1));
    dealCards(_player);
    printHand();
    if(isBust(_player_total)){
        _playerpts=_playerpts-_bid;
        cout<<"\n HOUSE WINS. PLAYER LOSES: "<<_bid<<" pts"<<endl;
        _round_flag=0;
    }
}
void Blackjack::leaveStay(){
    cout<<"\n If you want to leave type 0."<<endl;
    cout<<" If you want to stay type 1."<<endl; //AKO OSTANEN MORAN BRISAT RUKE, A NAKON 5. RUNDE MORAN MIŠAT DEK
    cout<<"\n Stay/Leave: ";
    cin>>_input;
    checkInput();
    if (_input==1){
        vector<string>().swap(_player);
        vector<string>().swap(_house);
    }
    else
        _play=_input;
}
unsigned int Blackjack::playerPoints(){
    return _playerpts;
}
void Blackjack::printDeck() const{
    for(unsigned int i=0; i<_deck.size(); i++){
        cout<<_deck[i]<<endl;
    }
}
