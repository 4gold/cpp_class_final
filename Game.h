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
#endif