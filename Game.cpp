
#include "Game.h"

using namespace std;

Game::Game()
{
}

// return [health, sanity, erosion, map的case]
Game::~Game()
{
}
void Game::setPlayer(Player *pPtr)
{
    this->pPtr = pPtr;
}

void Game::attack(Item* i)
{
    int* result = i->useItem(ATTACK);
    int h = result[0];
    int s = result[1];
    int e = result[2];
    int mapCase = result[3];

    int oldh = pPtr->getHealthPoint();
    int olds = pPtr->getSanityPoint();
    int olde = pPtr->getErosionPoint();

    pPtr->updateErosionPoint(olde + e);
    pPtr->updateHealthPoint(oldh + h);
    pPtr->updateSanityPoint(olds + s);

    if (mapCase != -1)
    {
        Map presentMap = pPtr->getMap();
        presentMap.updateConnection(mapCase);
        pPtr->updateMap(presentMap);
    }
}

void Game::talk(Item* i)
{
    int* result = i->useItem(TALK);
    int h = result[0];
    int s = result[1];
    int e = result[2];
    int mapCase = result[3];

    int oldh = pPtr->getHealthPoint();
    int olds = pPtr->getSanityPoint();
    int olde = pPtr->getErosionPoint();

    pPtr->updateErosionPoint(olde + e);
    pPtr->updateHealthPoint(oldh + h);
    pPtr->updateSanityPoint(olds + s);

    if (mapCase != -1)
    {
        Map presentMap = pPtr->getMap();
        presentMap.updateConnection(mapCase);
        pPtr->updateMap(presentMap);
    }
}

void Game::interact(Item* i)
{   
    int* result = i->useItem(INTERACT);
    int h = result[0];
    int s = result[1];
    int e = result[2];
    int mapCase = result[3];

    int oldh = pPtr->getHealthPoint();
    int olds = pPtr->getSanityPoint();
    int olde = pPtr->getErosionPoint();

    pPtr->updateErosionPoint(olde + e);
    pPtr->updateHealthPoint(oldh + h);
    pPtr->updateSanityPoint(olds + s);

    if (mapCase != -1)
    {
        Map presentMap = pPtr->getMap();
        presentMap.updateConnection(mapCase);
        pPtr->updateMap(presentMap);
    }
    // 如果為可取得的物品，交互完畢直接放進背包
    if (i->getTakable()) {
        // 複製物品進入背包
        vector<PureItem*> pBag(pPtr->bag);
        PureItem* pi = new PureItem(dynamic_cast<PureItem*>(i));
        pBag.push_back(pi); 
        pPtr->updateBag(pBag);

        // 地圖上的道具設為不可見
        i->updateDisable();
    }
}

void Game::observe(Item* i)
{
    int* result = i->useItem(OBSERVE);
    int h = result[0];
    int s = result[1];
    int e = result[2];
    int mapCase = result[3];

    int oldh = pPtr->getHealthPoint();
    int olds = pPtr->getSanityPoint();
    int olde = pPtr->getErosionPoint();

    pPtr->updateErosionPoint(olde + e);
    pPtr->updateHealthPoint(oldh + h);
    pPtr->updateSanityPoint(olds + s);

    if (mapCase != -1)
    {
        Map presentMap = pPtr->getMap();
        presentMap.updateConnection(mapCase);
        pPtr->updateMap(presentMap);
    }
}

bool Game::switchMap(int newRoom)
{
    // 只要isvalidmove就直接移動
    int currentRoomIndex = pPtr->getRoom();
    Map m = pPtr->getMap();
    bool canMove = m.isValidMove(static_cast<Map::Room>(currentRoomIndex), static_cast<Map::Room>(newRoom));

    if (canMove)
    {
        pPtr->setRoom(newRoom);
        cout << "移動至 : " << static_cast<Map::Room>(newRoom) << endl;
    } else {
        cout << "無效的移動" << endl;
    }
    return canMove;
}

void Game::help()
{   
    int hp = pPtr->getHealthPoint();
    cout << "你當前的血量 : " << hp << endl;
    cout << "---------------------------" << endl;
    cout << "～～以下是可使用指令～～" << endl;
    cout << "1. 與人/物互動的指令有如下四項：" << endl;
    cout << "attack, talk, interact, observe" << endl;
    cout << "可作用對象為房間內的人/物" << endl;
    cout << "使用格式： 可執行的指令 +\" \" +所在房間的可作用對象id" << endl;
    cout << "範例：若要與- 水盆(id:basin) - 互動，則輸入以下指令" << endl;
    cout << "interact basin" << endl;
    cout << "若要觀察- 床(id:bed)，則輸入以下指令" << endl;
    cout << "observe bed" << endl << endl;

    cout << "2. 若要觀察房內有什麼物品，請輸入 showRoom" << endl << endl;

    cout << "3. 若要觀看背包內物品，請輸入 showBag" << endl << endl;

    cout << "4. 移動的指令為 switchMap" << endl
         << "使用格式： switchMap + \" \" + 1~9的整數"  << endl
         
         << "1~9代表著不同的的房間, 只能移動到相通的房間" << endl
         << "目前所有已去過的房間 :" << endl;
}
