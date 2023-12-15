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

void initializeAllItems(map<string, Item*> &items, Player* player);
void putItemsInRoom(map<string, Item*> &items, Map& map);


/** 待補:
 * 各項東西的初始化(Map)
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

    map<string, Item*> allItems;
    initializeAllItems(allItems, pPtr);
    putItemsInRoom(allItems, map);

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

    cout << "help me" << "來了解遊玩方式";

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
        cout << "你覺得沒有辦法再支撐下去了....遊戲失敗";
    }
    else
    {
        cout << "遊戲結束";
    }

    return 0;
}


void initializeAllItems(map<string, Item*> &items, Player* player) {

    // 初始化static的預設對話
    HumanItem::initializeDefault();
    // ------------- key items ----------------------
    int effect[4][3] = {{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}};
    HumanItem* cook = new HumanItem("cook", false, effect, 100, player);
    HumanItem* usurper = new HumanItem("usurper", true, effect, 100, player);
    HumanItem* weird_grape_guy = new HumanItem("weird_grape_guy", false, effect, 100, player);

    int effect2[4][3] = {{0,0,0}, {-10,0,0}, {0,0,0}, {0,0,0}};
    PureItem* cabinet = new PureItem("cabinet", false, effect2, true, false, 0);
    PureItem* long_candlestick = new PureItem("long_candlestick", false, false, true, 0);
    PureItem* grape = new PureItem("grape", true, false, true, 0);

    int effect3[4][3] = {{0,0,0}, {0,-10,10}, {0,0,0}, {0,-1,1}};
    PureItem* painting_on_wall = new PureItem("painting_on_wall", false, effect3, true, false, 0);
    PureItem* door_leader_to_west_hallway = new PureItem("door_leader_to_west_hallway", false, false, false, 0);
    PureItem* sculpture = new PureItem("sculpture", false, true, false, 0);

    PureItem* diary_lab = new PureItem("diary_lab", false, false, false, 6);
    PureItem* trash_can = new PureItem("trash_can", true, false, false, 0);
    
    PureItem* key_to_outside = new PureItem("key_to_outside", false, false, true, 0);
    PureItem* sink = new PureItem("sink", false, true, false, 0);

    PureItem* drawer = new PureItem("drawer", true, false, false, 0);
    PureItem* diary_player = new PureItem("diary_player", false, true, false, 6);
    PureItem* plush_toy = new PureItem("plush_toy", false, false, true, 0);

    PureItem* piece_of_mirror_1 = new PureItem("piece_of_mirror_1", true, false, true, 0); // player's room
    PureItem* piece_of_mirror_2 = new PureItem("piece_of_mirror_2", true, false, true, 0); // lab
    PureItem* piece_of_mirror_3 = new PureItem("piece_of_mirror_3", true, false, true, 0); // kitchen

    PureItem* god_pearl = new PureItem("god_pearl", true, false, true, 0);

    // -----------設定物品間交互--------------
    // 1. 篡位者交流後將降神珠設為可見
    usurper->addRelatedItem(god_pearl, RELATED_STATE::DISABLE);

    // 2. 廚房葡萄事件
    weird_grape_guy->addRelatedItem(grape, RELATED_STATE::DISABLE);
    grape->addRelatedItem(weird_grape_guy, RELATED_STATE::PHASE);

    // 3. 儀式房長燭台
    long_candlestick->addRelatedItem(cabinet, RELATED_STATE::LOCK);
    
    // 4. 離開房間雕像倒下事件
    door_leader_to_west_hallway->addRelatedItem(sculpture, RELATED_STATE::LOCK);
    door_leader_to_west_hallway->addRelatedItem(painting_on_wall, RELATED_STATE::LOCK);

    // 5. 實驗室日記交流後出現垃圾桶，垃圾桶交流後出現鏡子碎片2
    diary_lab->addRelatedItem(trash_can, RELATED_STATE::DISABLE);
    trash_can->addRelatedItem(piece_of_mirror_2, RELATED_STATE::DISABLE);
    

    // 6. 主角日記交流後出現抽屜
    diary_player->addRelatedItem(drawer, RELATED_STATE::DISABLE);

    // 7. 抽屜交流後出現鏡子碎片1
    drawer->addRelatedItem(piece_of_mirror_1, RELATED_STATE::DISABLE);

    // 8. 水槽交流後出現鏡子碎片3
    sink->addRelatedItem(piece_of_mirror_3, RELATED_STATE::DISABLE);

    // 9. 羊寶寶玩偶交流後廚師狀態變更
    plush_toy->addRelatedItem(cook, RELATED_STATE::LOCK);

    // not important items
    PureItem* deadbody_ritaul_room = new PureItem();
    PureItem* basin;
    PureItem* door_to_outside;
    PureItem* broken_cabinet;
    PureItem* table_restaurant;

    // -------------------- insert items-------------------------------
    items.insert(pair<string, Item*>("cook", cook));
    items.insert(pair<string, Item*>("usurper", usurper));
    items.insert(pair<string, Item*>("weird_grape_guy", weird_grape_guy));

    items.insert(pair<string, Item*>("cabinet", cabinet));
    items.insert(pair<string, Item*>("long", long_candlestick));
    items.insert(pair<string, Item*>("grape", grape));

    items.insert(pair<string, Item*>("painting_on_wall", painting_on_wall));
    items.insert(pair<string, Item*>("door_leader_to_west_hallway", door_leader_to_west_hallway));
    items.insert(pair<string, Item*>("sculpture", sculpture));

    items.insert(pair<string, Item*>("diary_lab", diary_lab));
    items.insert(pair<string, Item*>("trash_can", trash_can));

    items.insert(pair<string, Item*>("sink", sink));
    items.insert(pair<string, Item*>("key_to_outside", key_to_outside));

    items.insert(pair<string, Item*>("diary_player", diary_player));
    items.insert(pair<string, Item*>("drawer", drawer));
    items.insert(pair<string, Item*>("plush_toy", plush_toy));
    items.insert(pair<string, Item*>("piece_of_mirror_1", piece_of_mirror_1));
    items.insert(pair<string, Item*>("piece_of_mirror_2", piece_of_mirror_2));
    items.insert(pair<string, Item*>("piece_of_mirror_3", piece_of_mirror_3));
    items.insert(pair<string, Item*>("god_pearl", god_pearl));
}

void putItemsInRoom(map<string, Item*> &items, Map& map) {
    map.addItemToRoom(map.ritualRoom, *(items.find("long_candlestick")->second));
    map.addItemToRoom(map.ritualRoom, *(items.find("cabinet")->second));
    map.addItemToRoom(map.ritualRoom, *(items.find("usurper")->second));

    map.addItemToRoom(map.storageRoom, *(items.find("grape")->second));

    map.addItemToRoom(map.restaurant, *(items.find("cook")->second));
    map.addItemToRoom(map.restaurant, *(items.find("weird_grape_guy")->second));

    map.addItemToRoom(map.leaderRoom, *(items.find("door_leader_to_west_hallway")->second));
    map.addItemToRoom(map.leaderRoom, *(items.find("painting_on_wall")->second));
    map.addItemToRoom(map.leaderRoom, *(items.find("sculpture")->second));

    map.addItemToRoom(map.laboratory, *(items.find("diary_lab")->second));
    map.addItemToRoom(map.laboratory, *(items.find("trash_can")->second));
    map.addItemToRoom(map.laboratory, *(items.find("piece_of_mirror_2")->second));

    map.addItemToRoom(map.kitchen, *(items.find("sink")->second));
    map.addItemToRoom(map.kitchen, *(items.find("piece_of_mirror_3")->second));

    map.addItemToRoom(map.protagonistRoom, *(items.find("plush_toy")->second));
    map.addItemToRoom(map.protagonistRoom, *(items.find("piece_of_mirror_1")->second));
    map.addItemToRoom(map.protagonistRoom, *(items.find("diary_player")->second));
    map.addItemToRoom(map.protagonistRoom, *(items.find("drawer")->second));
}