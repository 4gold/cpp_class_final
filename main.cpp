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
#include "story.h"
#include "Game.h"
#include "Event.h"


using namespace std;

bool isValidObj(string obj, int Itemcnt, vector<Item> itemsInCurrentRoom)
{
    for (int i = 0; i < Itemcnt; i++)
    {
        if (itemsInCurrentRoom[i].getName() == obj and itemsInCurrentRoom[i].getDisable())
        {
            return true;
        }
    }
    return false;
}

int main()
{
    // 初始化
    int eventProgress;
    string cmd, obj;
    Player p = Player();
    Player *pPtr = &p;
    Game g = Game();
    Map m = Map();
    evenInitializer();
    Event *startEvent = EventManager::getEvent(0);
    startEvent->start(0);
    // g.setPlayer(pPtr);

    // 螢幕初始顯示

    while (true)
    {
        eventProgress = Event::progressing;
        int currentPlayerRoom = p.getRoom(); // 這裡是個例子，實際上可以是任何 0 到 9 的整數

        // 取得當前房間中的物品列表

        vector<Item> itemsInCurrentRoom = m.roomItems[static_cast<Map::Room>(currentPlayerRoom)];
        vector<PureItem> bag = p.bag;
        int Itemcnt = itemsInCurrentRoom.size();
        int numInBag = bag.size();

        // 玩家需輸入: 可執行的指令 + " " + 所在房間的Item名稱
        // 切換地圖時: switchMap + " " + 1~9的整數

        cin >> cmd >> obj;
        Item objItem;

        if (cmd == "switchMap")
        {
            int convertedNumber = stoi(obj);
            if (convertedNumber <= 9 and convertedNumber > 0)
            {
                g.switchMap(convertedNumber);

                // // 輸出當前房間中的物品名稱
                cout << "這個房間有 : ";
                for (int i = 0; i < Itemcnt; i++)
                {
                    cout << itemsInCurrentRoom[i].getName() << ", ";
                }
                cout << endl;
                continue;
            }
            else
            {
                cout << "不存在的房間";
                continue;
            }
        }

        // 確認obj是否為存在此房間且可視的item
        if (!isValidObj(obj, Itemcnt, itemsInCurrentRoom))
        {
            continue;
        }

        for (int i = 0; i < Itemcnt; i++)
        {
            if (itemsInCurrentRoom[i].getName() == obj and itemsInCurrentRoom[i].getDisable())
            {
                objItem = itemsInCurrentRoom[i];
            }
        }

        // 各個Event的結束條件，結束後更新progress
        if (eventProgress == 0)
        {
            if (cmd == "attack" and obj == "cabinet")
            {
                // 檢查玩家是否有長燭台
                for (int i = 0; i < numInBag; i++)
                {
                    if (bag[i].getName() == "long_candlestick")
                    {   
                        g.attack(objItem);
                        cout << "前進儲藏室的台詞";
                        startEvent->turnoff();
                        startEvent->end(1, p);
                        g.switchMap(1);


                    }
                    else
                    {
                        g.attack(objItem);
                        cout << "扣血台詞";
                        cout << "前進儲藏室的台詞";
                        startEvent->turnoff();
                        startEvent->end(1, p);
                        g.switchMap(1);
                    }
                }
            }
        }
        else if (eventProgress == 1)
        {
        }
        else if (eventProgress == 2)
        {
        }
        else if (eventProgress == 3)
        {
        }
        else if (eventProgress == 4)
        {
        }
        else if (eventProgress == 5)
        {
        }
        else if (eventProgress == 6)
        {
        }
        else if (eventProgress == 7)
        {
        }

        if (cmd == "attack")
        {
            g.attack(objItem);
        }
        else if (cmd == "talk")
        {
            g.talk(objItem);
        }
        else if (cmd == "interact")
        {
            g.interact(objItem);
        }
        else if (cmd == "observe")
        {
            g.observe(objItem);
        }
        else if (cmd == "help") // 輸入help me
        {
            g.help();
        }
        else
        {
            // 輸入不存在指令
        }
    }

    return 0;
}
