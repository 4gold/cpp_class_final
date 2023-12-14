#include <iostream>
#include <string>
#include <cctype>
#include <vector>

#include "Item.h"
#include "Dialog.h"
#include "InteractType.h"
#include "HumanItem.h"
#include "PureItem.h"
#include "map.h"
#include "Player.h"

using namespace std;

#ifndef GAME_H
#define GAME_H
class Game
{
private:
    Player *pPtr;
public:
    Game();
    ~Game();
    void setPlayer(Player *pPtr);
    void switchMap(int room);
    void attack(Item i);
    void talk(Item i);
    void interact(Item i);
    void observe(Item i);
    void help();
};


Game::Game()  
{      
    
}


Game::~Game()
{
}
void Game::setPlayer(Player *pPtr)
{
    this->pPtr = pPtr;
}

void Game::attack(Item i)
{
    i.useItem(ATTACK);
}

void Game::talk(Item i)
{
    i.useItem(TALK);
}

void Game::interact(Item i)
{
    i.useItem(INTERACT);
}

void Game::observe(Item i)
{
    i.useItem(OBSERVE);
}

void Game::switchMap(int newRoom)
{
    // 只要isvalidmove就直接移動
    int currentRoomIndex = pPtr->getRoom();
    Map m = pPtr->getMap();
    bool canMove = m.isValidMove(static_cast<Map::Room>(currentRoomIndex), static_cast<Map::Room>(newRoom));

    if (canMove)
    {
        pPtr->setRoom(newRoom);
        cout << "移動至 : " << static_cast<Map::Room>(newRoom) << endl;
    }
    cout << "無效的移動";
}

void Game::help()
{
    cout << "以下是可使用指令" << endl;
    cout << "attack, talk, interact, observe" << endl;
    cout << "可作用對象為房間內的人/物" << endl;
    cout << "使用格式： 可執行的指令 + " " + 所在房間的可作用對象" << endl;
    cout << "例如輸入: observe bed" << endl
         << "移動的指令為 switchMap" << endl
         << "使用格式： switchMap + " " + 1~9的整數" << endl
         << "1~9代表著不同的的房間, 只能移動到相通的房間" << endl
         << "目前所有已去過的房間 :";
}
#endif