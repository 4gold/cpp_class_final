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
bool isStringInteger(const std::string &s)
{
    for (char c : s)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }
    return true;
}
bool isValidRoom(const string &obj)
{
    if (isStringInteger(obj))
    {
        int convertedNumber = stoi(obj);
        if (convertedNumber <= 9 and convertedNumber > 0)
        {
            return true;
        }
    }

    return false;
}

/** 待補:
 * 新手教學
 * 各項東西的初始化(Player, Map)
 * 螢幕初始顯示
 * 圖片置放
 * 遊戲內的細部台詞
 * 結束之後會發生的事
 * 理智值事件(?)
 * 血量提醒(?)
 * 儲存可前往的room以供help()查詢
 * 
 * 修改血量 理智值的function()
 *  */
int main()
{
    // 初始化
    int eventProgress;
    int ending = 0; // 0 : 還沒決定結局 1: 侵蝕度低 4: 侵蝕度高
    string cmd, obj;
    Player p = Player();
    Player *pPtr = &p;
    Game g = Game();
    Map m = Map();
    Story story = Story();
    evenInitializer();
    Event *startEvent = EventManager::getEvent(0);
    Event *kitchen = EventManager::getEvent(1);
    Event *hierarch = EventManager::getEvent(2);
    Event *finalEvent = EventManager::getEvent(3);
    Event *ending1 = EventManager::getEvent(4);
    Event *ending2 = EventManager::getEvent(5);
    Event *ending3 = EventManager::getEvent(6);
    Event *ending4 = EventManager::getEvent(7);

    startEvent->start(0);
    g.setPlayer(pPtr);

    bool flag2 = true; // 看神像有沒有倒過
    bool alive = true; // 有沒有血量歸零
    // 螢幕初始顯示

    // 可能一些教學之類的

    while (true)
    {
        eventProgress = Event::progressing;
        int currentPlayerRoom = p.getRoom(); 
        int currentHp = p.getHealthPoint();
        if (currentHp <= 0)
        {
            alive = false;
            break;
        }
        // 取得當前房間中的物品列表

        vector<Item> itemsInCurrentRoom = m.roomItems[static_cast<Map::Room>(currentPlayerRoom)];
        vector<PureItem> bag = p.bag;
        int Itemcnt = itemsInCurrentRoom.size();
        int numInBag = bag.size();
        int convertedNumber;
        // 玩家需輸入: 可執行的指令 + " " + 所在房間的Item名稱
        // 切換地圖時: switchMap + " " + 1~9的整數

        cin >> cmd >> obj;
        Item objItem;

        // 確認obj是否為存在此房間且可視的item 或 存在的房間

        if (cmd == "switchMap")
        {
            if (!isValidRoom(obj))
            {
                cout << "無效的房間";
                continue;
            }
            else
            {
                int convertedNumber = stoi(obj);
            }
        }
        else if (cmd == "attack" or cmd == "talk" or cmd == "interact" or cmd == "observe")
        {
            if (!isValidObj(obj, Itemcnt, itemsInCurrentRoom))
            {
                cout << "不存在的對象";
                continue;
            }
        }

        // 將obj 轉換成 Item 物件
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
                g.attack(objItem);
                // 檢查玩家是否有長燭台
                for (int i = 0; i < numInBag; i++)
                {
                    if (bag[i].getName() == "long_candlestick")
                    {
                        cout << "前進儲藏室的台詞";
                        startEvent->turnoff();
                        startEvent->end(1, p, -1);
                        g.switchMap(1);
                        continue;
                    }
                }
                cout << "扣血台詞";
                cout << "前進儲藏室的台詞";
                startEvent->turnoff();
                startEvent->end(1, p, -1);
                g.switchMap(1);
                continue;
            }
        }
        else if (eventProgress == 1 or eventProgress == -1)
        {
            // 如果kitchen沒被觸發過就開始事件
            if (cmd == "switchMap")
            {
                if (convertedNumber == 2)
                {
                    g.switchMap(convertedNumber);

                    // 輸出當前房間中的物品名稱
                    cout << "所在的房間有 : ";
                    for (int i = 0; i < Itemcnt; i++)
                    {
                        cout << itemsInCurrentRoom[i].getName() << ", ";
                    }
                    cout << endl;

                    if (!(kitchen->isprogressing()))
                    {
                        kitchen->start(1);
                    }
                    continue;
                }
            }
            // 結束條件
            if ((cmd == "interact" and obj == "weird_grape_guy") and (kitchen->isprogressing()))
            {
                // 檢查玩家是否有葡萄
                for (int i = 0; i < numInBag; i++)
                {
                    if (bag[i].getName() == "grape")
                    {
                        cout << "『謝謝你！你不是葡萄小偷！』";
                        kitchen->turnoff();
                        kitchen->end(2, p, -2);
                        continue;
                    }
                }
                g.interact(objItem);
                continue;
            }
        }
        else if (eventProgress == 2 or eventProgress == -2)
        {
            // 如果hierarch沒被觸發過就開始事件
            if (cmd == "switchMap")
            {
                if (convertedNumber == 8)
                {
                    g.switchMap(convertedNumber);

                    // 輸出當前房間中的物品名稱
                    cout << "所在的房間有 : ";
                    for (int i = 0; i < Itemcnt; i++)
                    {
                        cout << itemsInCurrentRoom[i].getName() << ", ";
                    }
                    cout << endl;

                    if (!(hierarch->isprogressing()))
                    {
                        hierarch->start(1);
                        hierarch->readLines();
                        int sp = p.getSanityPoint();
                        p.updateSanityPoint(sp - 1);
                    }
                    continue;
                }
                // 只會在第一次離開教主房間時啟動, 順便結束這個event
                else if (flag2)
                {
                    cout << "要離開房間時，神像會倒下撞破掛畫，發現後面的實驗室。同時也會發現神像裡面有屍體";
                    flag2 = false;
                    hierarch->turnoff();
                    hierarch->end(3, p, -10);
                    continue;
                }
            }
        }
        else if (eventProgress == -10) // 離開教主房間後拿到鑰匙之前
        {
            // 檢查玩家是否有大門鑰匙(此時決定戰勝的結局是1 or 4)

            for (int i = 0; i < numInBag; i++)
            {
                if ((bag[i].getName() == "key_to_outside") and ending == 0)
                {
                    int sp = p.getSanityPoint();
                    int ep = p.getErosionPoint();
                    ending = story.checkEnding(sp, ep);
                }
                else if ((bag[i].getName() == "key_to_outside") and cmd == "switchMap" and convertedNumber == 0)
                {
                    g.switchMap(convertedNumber);

                    // 輸出當前房間中的物品名稱
                    cout << "所在的房間有 : ";
                    for (int i = 0; i < Itemcnt; i++)
                    {
                        cout << itemsInCurrentRoom[i].getName() << ", ";
                    }
                    cout << endl;
                    finalEvent->start(3);
                    continue;
                }
            }
        }
        else if (eventProgress == 3) // 儀式房 vs 篡位者
        {
            // 聽完提問過後的回答
            int ans;
            cin >> ans;

            if (ans == 1) // 答對，準備被攻擊
            {   
                cout << "台詞";
                p.updateHealthPoint(currentHp-10);
                // 看還有沒有要做甚麼

                if ((cmd == "attack" and obj == "usurper"))
                {
                    // 檢查玩家是否有鏡子1~3
                    int mirrorNum = 0;
                    for (int i = 0; i < numInBag; i++)
                    {
                        if ((bag[i].getName() == "piece_of_mirror_1") or (bag[i].getName() == "piece_of_mirror_2") or (bag[i].getName() == "piece_of_mirror_3"))
                        {
                            mirrorNum++;
                        }
                    }
                    if (mirrorNum == 3) // 鏡子蒐集齊
                    {   
                        g.attack(objItem); 
                        if (ending = 1) // 侵蝕度高->結局1
                        {   
                            cout << "台詞";
                            finalEvent->end(1, p, 4);
                            finalEvent->turnoff();
                            ending1->start(4);
                            break;
                        }
                        else if (ending = 4) // 侵蝕度低->結局4
                        {
                            cout << "台詞";
                            finalEvent->end(1, p, 7);
                            finalEvent->turnoff();
                            ending4->start(7);
                            break;
                        }
                    }
                    else // 鏡子沒蒐集齊->結局2
                    {   
                        cout << "台詞";
                        finalEvent->end(1, p, 5);
                        finalEvent->turnoff();
                        ending4->start(7);
                        alive = false;
                        break;
                    }
                }
            }
            else if (ans == 2) // 答錯，進入結局2
            {   
                cout << "台詞";
                finalEvent->end(1, p, 5);
                finalEvent->turnoff();
                ending4->start(7);
                alive = false;
                break;
            }
            else // 答錯，進入結局2
            {
                cout << "亂答台詞";
                finalEvent->end(1, p, 5);
                finalEvent->turnoff();
                ending4->start(7);
                alive = false;
                break;
            }
        }

        // 通常的處裡狀況   
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
        else if (cmd == "attack")
        {
            g.attack(objItem);
            continue;
        }
        else if (cmd == "talk")
        {
            g.talk(objItem);
            continue;
        }
        else if (cmd == "interact")
        {
            g.interact(objItem);
            continue;
        }
        else if (cmd == "observe")
        {
            g.observe(objItem);
            continue;
        }
        else if (cmd == "help") // 輸入help me
        {
            g.help();
            continue;
        }
        else
        {
            // 輸入不存在指令
            cout << "請輸入有效指令";
            continue;
        }
    }

    // 通關失敗的後果
    if(!alive)
    {
        cout << "遊戲失敗";
    }
    else
    {
        cout << "遊戲結束";
    }

    return 0;
}
