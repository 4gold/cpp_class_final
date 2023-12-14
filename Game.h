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
    void attack(Item &i);
    void talk(Item &i);
    void interact(Item &i);
    void observe(Item &i);
    void help();
};


#endif