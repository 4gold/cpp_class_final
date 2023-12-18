#include <iostream>
#include <string>
#include <cctype>
#include <vector>

#include "Item.h"
#include "InteractType.h"
#include "HumanItem.h"
#include "PureItem.h"
#include "map.h"
#include "Player.h"
#include "story.h"
#include "Game.h"
#include "Event.h"

using namespace std;

bool isValidObj(string obj, int Itemcnt, vector<Item*> itemsInCurrentRoom)
{
    for (int i = 0; i < Itemcnt; i++)
    {
        if (itemsInCurrentRoom[i]->getName() == obj && !itemsInCurrentRoom[i]->getDisable())
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
        if (convertedNumber <= 9 and convertedNumber >= 0)
        {
            return true;
        }
    }

    return false;
}

void showValidItemsInRoom(vector<Item*> itemsInCurrentRoom, int Itemcnt) {
    // 輸出當前房間中的物品名稱
    cout << "所在的房間有 : ";
    for (int i = 0; i < Itemcnt; i++)
    {   
        if (itemsInCurrentRoom[i]->getDisable()) continue; // disable 物件跳過不顯示
        cout << itemsInCurrentRoom[i]->getChineseName() << "(id:" <<  itemsInCurrentRoom[i]->getName() << "), ";
    }
    cout << endl;
}


void initializeAllKeyItems(map<string, Item*> &items, Player* player); // 初始化關鍵道具
void initializeAllNoneKeyItems(map<string, Item*> &items, Map &map); // 初始化非關鍵道具，設定在not_key_items.txt
void putKeyItemsInRoom(map<string, Item*> &items, Map& map); // 將關鍵道具塞入地圖
void initializeAllItemsSetting(map<string, Item*> &items); // 初始化道具的設定(扣血術數值等)
void evenInitializer();


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
    initializeAllNoneKeyItems(allItems, m);
    initializeAllKeyItems(allItems, pPtr);
    putKeyItemsInRoom(allItems, m);

    initializeAllItemsSetting(allItems);


    p.setRoom(Map::Room::ritualRoom);


    
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

    cout << "help me" << "來了解遊玩方式" << endl;

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
        // testing value print
        //cout << "EventProgress : " << eventProgress << endl;

        // 取得當前房間中的物品列表
        vector<Item*> itemsInCurrentRoom = m.roomItems[static_cast<Map::Room>(currentPlayerRoom)];
        vector<PureItem*> bag = p.bag;
        int Itemcnt = itemsInCurrentRoom.size();
        int numInBag = bag.size();
        int convertedNumber;
        // 玩家需輸入: 可執行的指令 + " " + 所在房間的Item名稱
        // 切換地圖時: switchMap + " " + 1~9的整數
        string cmdStr;
        getline(cin, cmdStr);
        vector<string> cmds = FuncPool::split(cmdStr, " ");
        if (cmds.size() > 2 || cmds.size() == 0) {
            cout << "請輸入有效指令";
            continue;
        } 
        cmd = cmds[0];
        obj = cmds.back();

        Item* objItem;

        // 確認obj是否為存在此房間且可視的item 或 存在的房間

        if (cmd == "testEnding") {

            cout << "player's sanity point : " << p.getSanityPoint() << endl;
            cout << "player's erosion point : " << p.getErosionPoint() << endl;

            // testing
            g.interact(allItems["key_to_outside"]);
            g.interact(allItems["piece_of_mirror_1"]);
            g.interact(allItems["piece_of_mirror_2"]);
            g.interact(allItems["piece_of_mirror_3"]);

            ending = story.checkEnding(p.getSanityPoint(), p.getErosionPoint());

            finalEvent->start(3);
            continue;
        }
        if (cmd == "switchMap")
        {
            if (!isValidRoom(obj))
            {
                cout << "無效的房間" << endl;
                continue;
            }
            else
            {
                convertedNumber = stoi(obj);
            }
        }
        else if (cmd == "showRoom") {
            cout << "目前位置：" << currentPlayerRoom << endl;
            showValidItemsInRoom(itemsInCurrentRoom, Itemcnt);
            continue;
        }
        else if (cmd == "showBag") {
            cout << "目前背包裡有：";
            for (Item* itemInBag : pPtr->bag )
                cout << itemInBag->getName() << ", ";
            cout << endl;
            continue;
        }
        else if (cmd == "attack" or cmd == "talk" or cmd == "interact" or cmd == "observe")
        {
            if (!isValidObj(obj, Itemcnt, itemsInCurrentRoom))
            {
                cout << "不存在的對象" << endl;
                continue;
            }
        }

        // 將obj 轉換成 Item 物件
        for (int i = 0; i < Itemcnt; i++)
        {
            if (itemsInCurrentRoom[i]->getName() == obj && !itemsInCurrentRoom[i]->getDisable())
            {
                objItem = itemsInCurrentRoom[i];
            }
        }

        // 各個Event的結束條件，結束後更新progress
        if (eventProgress == 0)
        {
            if (cmd == "attack" and obj == "cabinet")
            {   
                string goStorageRoomDialog = "你穿過木櫃，穿過了一段通道，進到了一個充滿灰塵的小房間。"; //穿過儲藏室台詞
                string loseBloodDialog = "hp - 10";
                g.attack(objItem);
                // 檢查玩家是否有長燭台
                for (int i = 0; i < numInBag; i++)
                {
                    if (bag[i]->getName() == "long_candlestick")
                    {
                        cout << goStorageRoomDialog << endl;
                        startEvent->turnoff();
                        startEvent->end(1, p, -1);
                        g.switchMap(1);
                        continue;
                    }
                }
                cout << loseBloodDialog << endl;
                cout << goStorageRoomDialog << endl;

                startEvent->turnoff();
                startEvent->end(1, p, -1);
                g.switchMap(1);
                continue;
            }
        }
        else if (eventProgress == 1 || eventProgress == -1)
        {
            // 如果kitchen沒被觸發過就開始事件
            if (cmd == "switchMap" && convertedNumber == 2)
            {
                bool canMove = g.switchMap(convertedNumber);
                if (!canMove) continue;

                // 更新現有房間內的物品
                currentPlayerRoom = convertedNumber;
                itemsInCurrentRoom = m.roomItems[static_cast<Map::Room>(currentPlayerRoom)];

                if (kitchen->isactive() && !kitchen->isprogressing()) //事件未被觸發過，且並未在執行中
                {
                    kitchen->start(1);
                    // 觸發事件後儲藏室物品才可見。
                    for (Item* itemInStorage : m.roomItems[static_cast<Map::Room>(m.storageRoom)]) {
                        if (itemInStorage->getName().find("door") != string::npos) // 對象為門則不要設定。
                            continue;
                        itemInStorage->updateDisable();
                    }
                }
                continue;         
            }
            // 結束條件
            if ((cmd == "interact" && obj == "weird_grape_guy") && (kitchen->isprogressing()))
            {   
                bool hasGrape = false;
                // 檢查玩家是否有葡萄
                for (int i = 0; i < numInBag; i++)
                {
                    if (bag[i]->getName() == "grape")
                        hasGrape = true;
                          
                } 
                g.interact(objItem);   

                if (hasGrape) {
                    //葡萄怪人狀態更新。
                    allItems["weird_grape_guy"]->updatePhase(); 
                    allItems["weird_grape_guy"]->reloadDialog(); 

                    kitchen->turnoff();
                    kitchen->end(2, p, -2);
                }        
                continue;
            }
        }
        else if (eventProgress == 2 || eventProgress == -2)
        {
            // 如果hierarch沒被觸發過就開始事件
            if (cmd == "switchMap")
            {   
                bool canMove = g.switchMap(convertedNumber);
                if (!canMove) continue;

                if ((convertedNumber == 8) && !hierarch->isprogressing())
                {                    
                    currentPlayerRoom = canMove ? convertedNumber : currentPlayerRoom;
                    itemsInCurrentRoom = m.roomItems[static_cast<Map::Room>(currentPlayerRoom)];

                    if ((hierarch->isactive()))
                    {
                        hierarch->start(2);
                        int sp = p.getSanityPoint();
                        p.updateSanityPoint(sp - 10);
                    }
                    continue;
                } // 只會在第一次離開教主房間時啟動, 順便結束這個event
                else if (hierarch->isprogressing())
                {   
                    cout << "當你要離開房間時，牆邊的雕像突然傾斜，向著房間倒去。你趕緊躲開，只見雕像撞破掛畫，露出了後面的空間（9）。\n"
                        <<  "你好像從雕像破碎的縫隙中看到了人的皮膚。" << endl;
                    flag2 = false;

                    //雕像、掛畫狀態更新。
                    allItems["painting_on_wall"]->updateLock(); 
                    allItems["sculpture"]->updateLock(); 
                    allItems["painting_on_wall"]->reloadDialog(); 
                    allItems["sculpture"]->reloadDialog(); 

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
                if ((bag[i]->getName() == "key_to_outside") and ending == 0)
                {
                    int sp = p.getSanityPoint();
                    int ep = p.getErosionPoint();
                    ending = story.checkEnding(sp, ep);
                }
                else if ((bag[i]->getName() == "key_to_outside") and cmd == "switchMap" and convertedNumber == 0)
                {
                    bool canMove = g.switchMap(convertedNumber);
                    currentPlayerRoom = canMove ? convertedNumber : currentPlayerRoom;
                    itemsInCurrentRoom = m.roomItems[static_cast<Map::Room>(currentPlayerRoom)];

                    finalEvent->start(3);
                    break;
                }
            }
        }
        else if (eventProgress == 3) // 儀式房 vs 篡位者
        {
            // 聽完提問過後的回答
            int ans = 0;
            string ansStr = cmd;
            while ( ans != 1 && ans != 2 && ans != 3) {        
                cin >> ansStr;
                if (ansStr.size() == 1 && isdigit(ansStr[0])) {
                    ans = ansStr[0] - '0';
                    if (ans == 1 || ans == 2 || ans == 3)
                        break;
                } 
                cout << "請輸入有效的選項。" << endl;  
            }
            
            if (ans == 1) // 答對，準備被攻擊
            {   
                string dialogRightAns;
                dialogRightAns.append("你：「漢森，我知道你不滿意我。」\n");
                dialogRightAns.append("你指著地上的屍體，\n");
                dialogRightAns.append("你：「但是我們都被教主騙了。」\n");
                dialogRightAns.append("你：「我知道儀式失敗是你搞的鬼。現在所有人都有“神明”保佑了，你滿意了嗎？」\n");
                dialogRightAns.append("你：「現在把東西給我。」");
                dialogRightAns.append("你小心翼翼的哄騙他，你需要他手上的母蠱來解蠱。\n");

                cout << dialogRightAns << endl;
                p.updateHealthPoint(currentHp-10);
                // 看還有沒有要做甚麼

                cout << "按下 Enter 繼續" << endl;
                cin.ignore();
                
                // 檢查玩家是否有鏡子1~3
                int mirrorNum = 0;
                for (int i = 0; i < numInBag; i++)
                {
                    if ((bag[i]->getName() == "piece_of_mirror_1") || (bag[i]->getName() == "piece_of_mirror_2") || (bag[i]->getName() == "piece_of_mirror_3"))
                    {
                        mirrorNum++;
                    }
                }
                if (mirrorNum == 3) // 鏡子蒐集齊
                {   
                    //g.attack(objItem); 
                    if (ending == 1) // 侵蝕度高->結局1
                    {   
                        vector<string> dialogWinHighErosion{
                            "你試著壓制著漢森，但他的力氣異常的大，你鬥不過他。",
                            "你想起了鏡子，拿出來對著他的臉。",
                            "『不！』漢森大叫著退後，用害怕的雙手遮住了自己的臉。 ",
                            "『這不是我！不要！』",
                            "混亂中他身上的玻璃珠子掉在了地上，砸成了碎片。",
                            "你趕緊蹲下來搜尋剩下的碎片，也不顧手被玻璃碎變渣的滿是鮮血。",
                            "你找到了玻璃珠內的母蠱，正要吞下去時，你改變了主意。",
                            "你覺得有點浪費。",
                            "你：「嘿漢森，不然這樣好了。」",
                            "你：「反正教主都死了，不如我們一起重建教會。」",
                            "你：「你要神我要錢，雙贏。」",
                            " ... ",
                            " ... "
                        };
                        for (string dialog : dialogWinHighErosion)
                            FuncPool::delayCout(dialog);
                        finalEvent->end(1, p, 4);
                        finalEvent->turnoff();
                        ending1->start(4);
                        break;
                    }
                    else if (ending == 4) // 侵蝕度低->結局4
                    {   
                        vector<string> dialogWinLowErosion{
                            "你試著壓制著漢森，但他的力氣異常的大，你鬥不過他。",
                            "你想起了鏡子，拿出來對著他的臉。",
                            "『不！』漢森大叫著退後，用害怕的雙手遮住了自己的臉。 ",
                            "『這不是我！不要！』",
                            "混亂中他身上的玻璃珠子掉在了地上，砸成了碎片。",
                            "你趕緊蹲下來搜尋剩下的碎片，也不顧手被玻璃碎變渣的滿是鮮血。",
                            "你找到了玻璃珠內的母蠱，吞了下去。",
                            "當你吞下蟲的瞬間，你聽到漢森絕望地叫了起來。",
                            "你看到漢森開始溶解。",
                            "『神...那是我的神...』",
                            "漢森掙扎著爬向你，直到完全變成一灘血水。",
                            " ... ",
                            " ... "
                        };

                        for (string dialog : dialogWinLowErosion)
                            FuncPool::delayCout(dialog);
                                                
                        finalEvent->end(1, p, 7);
                        finalEvent->turnoff();
                        ending4->start(7);
                        break;
                    }
                }
                else // 鏡子沒蒐集齊->結局2
                {   
                    finalEvent->end(1, p, 5);
                    finalEvent->turnoff();
                    ending2->start(7);
                    alive = false;
                    break;
                }
                
            }
            else if (ans == 2 || ans == 3) // 答錯，進入結局3
            {      
                string wrongName = ((ans == 2) ? "傑米" : "巴特");
                vector<string> dialogWrongAns {
                    "你：「" + wrongName + "？」",
                    "對方沒有回應你，冷冷的看了你一眼就走了。",
                    "你摸不太清楚頭緒，用大門把鑰匙打開後沒有受到攔阻的離開了房間。",
                    "...",
                    "..."
                    };
                for (string dialog : dialogWrongAns)
                FuncPool::delayCout(dialog);

                finalEvent->end(1, p, 5);
                finalEvent->turnoff();
                ending3->start(7);
                break;
            }
        }

        // 通常的處裡狀況   
        if (cmd == "switchMap")
        {
            int convertedNumber = stoi(obj);
            if (convertedNumber <= 9 and convertedNumber >= 0)
            {
                g.switchMap(convertedNumber);
                // 更新現有房間內的物品
                currentPlayerRoom = convertedNumber;
                itemsInCurrentRoom = m.roomItems[static_cast<Map::Room>(currentPlayerRoom)];
                continue;
            }
            else
            {
                cout << "不存在的房間" << endl;
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
            cout << "請輸入有效指令" << endl;
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


void initializeAllKeyItems(map<string, Item*> &items, Player* player) {

    // 初始化static的預設對話
    HumanItem::initializeDefault();
    // ------------- key items ----------------------
    // npcs
    int effect[4][3] = {{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}};
    HumanItem* cook = new HumanItem("廚師", "cook", false, effect, 100, player);
    HumanItem* usurper = new HumanItem("漢森", "usurper", true, effect, 100, player);
    HumanItem* weird_grape_guy = new HumanItem("怪人", "weird_grape_guy", false, effect, 100, player);
    // items
    // ritual room
    int effect2[4][3] = {{0,0,0}, {-10,0,0}, {0,0,0}, {0,0,0}};
    PureItem* cabinet = new PureItem("木櫃子", "cabinet", false, effect2, true, false, 0);
    PureItem* long_candlestick = new PureItem("長燭台", "long_candlestick", false, false, true, 0);
    PureItem* god_pearl = new PureItem("降神珠", "god_pearl", true, false, true, 0);

    // restaurant
    PureItem* grape = new PureItem("葡萄", "grape", true, false, true, 0);
    // leader room
    int effect3[4][3] = {{0,0,0}, {0,-10,10}, {0,0,0}, {0,-1,1}};
    PureItem* painting_on_wall = new PureItem("牆上的掛畫", "painting_on_wall", false, effect3, true, false, 0);
    PureItem* sculpture = new PureItem("雕像", "sculpture", false, true, false, 0);
    // lab
    PureItem* diary_lab = new PureItem("實驗紀錄", "diary_lab", false, false, false, 6);
    PureItem* trash_can = new PureItem("垃圾桶", "trash_can", false, false, false, 0);
    // kitchen
    PureItem* key_to_outside = new PureItem("大門的鑰匙", "key_to_outside", false, false, true, 0);
    PureItem* sink = new PureItem("水槽", "sink", false, true, false, 0);
    // protagonist's room
    PureItem* drawer = new PureItem("抽屜", "drawer", true, true, false, 0);
    PureItem* diary_player = new PureItem("日記", "diary_player", false, true, false, 6);
    PureItem* plush_toy = new PureItem("羊寶寶玩偶", "plush_toy", false, false, true, 0);

    PureItem* piece_of_mirror_1 = new PureItem("鏡子碎片1", "piece_of_mirror_1", true, false, true, 0); // player's room
    PureItem* piece_of_mirror_2 = new PureItem("鏡子碎片2", "piece_of_mirror_2", true, false, true, 0); // lab
    PureItem* piece_of_mirror_3 = new PureItem("鏡子碎片3", "piece_of_mirror_3", true, false, true, 0); // kitchen


    // -----------設定物品間交互--------------
    // 1. 篡位者交流後將降神珠設為可見
    usurper->addRelatedItem(god_pearl, RELATED_STATE::DISABLE);

    // 2. 廚房葡萄事件
    grape->addRelatedItem(weird_grape_guy, RELATED_STATE::PHASE);

    // 3. 儀式房長燭台
    long_candlestick->addRelatedItem(cabinet, RELATED_STATE::LOCK);
    
    // 4. 離開房間雕像倒下事件
    //door_leader_to_west_hallway->addRelatedItem(sculpture, RELATED_STATE::LOCK);
    //door_leader_to_west_hallway->addRelatedItem(painting_on_wall, RELATED_STATE::LOCK);

    // 5. 垃圾桶交流後出現鏡子碎片2
    //diary_lab->addRelatedItem(trash_can, RELATED_STATE::DISABLE);
    trash_can->addRelatedItem(piece_of_mirror_2, RELATED_STATE::DISABLE);
    
    // 6. 主角日記交流後出現抽屜
    diary_player->addRelatedItem(drawer, RELATED_STATE::DISABLE);

    // 7. 抽屜交流後出現鏡子碎片1 / 取得鏡子碎片1修改抽屜狀態
    drawer->addRelatedItem(piece_of_mirror_1, RELATED_STATE::DISABLE);
    piece_of_mirror_1->addRelatedItem(drawer, RELATED_STATE::LOCK);

    // 8. 水槽交流後出現鏡子碎片3
    sink->addRelatedItem(piece_of_mirror_3, RELATED_STATE::DISABLE);

    // 9. 羊寶寶玩偶交流後廚師狀態變更
    plush_toy->addRelatedItem(cook, RELATED_STATE::PHASE);

    // 10. 拿到大門鑰匙後，結局道具
    key_to_outside->addRelatedItem(usurper, RELATED_STATE::DISABLE);
    usurper->addRelatedItem(god_pearl, RELATED_STATE::DISABLE);

    // -------------------- insert items-------------------------------
    items.insert(pair<string, Item*>("cook", cook));
    items.insert(pair<string, Item*>("usurper", usurper));
    items.insert(pair<string, Item*>("weird_grape_guy", weird_grape_guy));

    items.insert(pair<string, Item*>("cabinet", cabinet));
    items.insert(pair<string, Item*>("long_candlestick", long_candlestick));
    items.insert(pair<string, Item*>("grape", grape));

    items.insert(pair<string, Item*>("painting_on_wall", painting_on_wall));
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

void putKeyItemsInRoom(map<string, Item*> &items, Map& gameMap) {

    gameMap.addItemToRoom(gameMap.ritualRoom, (items["cabinet"]));
    gameMap.addItemToRoom(gameMap.ritualRoom, (items["long_candlestick"]));
    gameMap.addItemToRoom(gameMap.ritualRoom, (items["usurper"]));
    gameMap.addItemToRoom(gameMap.ritualRoom, (items["god_pearl"]));
    
    
    gameMap.addItemToRoom(gameMap.storageRoom, (items["grape"]));

    gameMap.addItemToRoom(gameMap.restaurant, (items["cook"]));
    gameMap.addItemToRoom(gameMap.restaurant, (items["weird_grape_guy"]));

    gameMap.addItemToRoom(gameMap.leaderRoom, (items["painting_on_wall"]));
    gameMap.addItemToRoom(gameMap.leaderRoom, (items["sculpture"]));

    gameMap.addItemToRoom(gameMap.laboratory, (items["diary_lab"]));
    gameMap.addItemToRoom(gameMap.laboratory, (items["trash_can"]));
    gameMap.addItemToRoom(gameMap.laboratory, (items["piece_of_mirror_2"]));

    gameMap.addItemToRoom(gameMap.kitchen, (items["sink"]));
    gameMap.addItemToRoom(gameMap.kitchen, (items["piece_of_mirror_3"]));
    gameMap.addItemToRoom(gameMap.kitchen, (items["key_to_outside"]));

    gameMap.addItemToRoom(gameMap.protagonistRoom, (items["plush_toy"]));
    gameMap.addItemToRoom(gameMap.protagonistRoom, (items["piece_of_mirror_1"]));
    gameMap.addItemToRoom(gameMap.protagonistRoom, (items["diary_player"]));
    gameMap.addItemToRoom(gameMap.protagonistRoom, (items["drawer"]));
    
}


void initializeAllNoneKeyItems(map<string, Item*> &allItems, Map &map) {
    string fileName = "not_key_items_list.txt";
    std::ifstream filein(fileName);
    for (std::string line; std::getline(filein, line);) 
    {  
        //format : englishName,chineseName,room
        vector<string> str = FuncPool::split(line, ",");
        if (str.size() < 3) continue; // 防空行
        string name = str[0];
        string chineseName = str[1];
        Map::Room room = static_cast<Map::Room>(stoi(str[2]));

        // insert items
        PureItem* item = new PureItem(chineseName, name);
        map.addItemToRoom(room, item);
        allItems.insert(pair<string, Item*>(name, item));
    }
    filein.close(); 
}

void initializeAllItemsSetting(map<string, Item*> &allItems) {
    // 設定初始數值
    allItems["watermelon"]->updateDisable(); //設為不可見
    // act, hp, sanity, erosion
    allItems["deadbody_ritaul_room"]->updateEffect(INTERACT_TYPE::ATTACK, 0, 0, 10); // 儀式房屍體，褻瀆屍體升侵蝕
    allItems["watermelon"]->updateEffect(INTERACT_TYPE::ATTACK, 0, 10, 0); // 儲藏室西瓜，willson，回理智
    allItems["sculpture"]->updateEffect(INTERACT_TYPE::ATTACK, 0, -5, 10); // 教主房雕像，裡面有人，攻擊升侵蝕度
    allItems["bed_leader_room"]->updateEffect(INTERACT_TYPE::INTERACT, 10, 0, 0); // 教主房的床，很舒服，躺了回血
    allItems["bed_lab"]->updateEffect(INTERACT_TYPE::ATTACK, -10, 0, 0); // 實驗室床，很硬，打了扣血
    allItems["weird_cups"]->updateEffect(INTERACT_TYPE::INTERACT, -10, 0, 0); // 實驗室的瓶罐，打開會扣血
    allItems["piece_of_mirror_1"]->updateEffect(INTERACT_TYPE::OBSERVE, 0, -10, 0); // 鏡子系列，看了扣理智升侵蝕
    allItems["piece_of_mirror_2"]->updateEffect(INTERACT_TYPE::OBSERVE, 0, -10, 0); // 鏡子系列，看了扣理智升侵蝕
    allItems["piece_of_mirror_3"]->updateEffect(INTERACT_TYPE::OBSERVE, 0, -10, 0); // 鏡子系列，看了扣理智升侵蝕

}


void evenInitializer()
{  

    cout << "event初始化" << endl;

    // 遊戲的開始 0
    int startEvenLen = 5;

    vector<string> startEvenLines{
        " 你從一個不熟悉的房間醒來，房內有些昏暗，從門框透進來的一縷光線是唯一的光源。",
        " 你感覺身體有些酸痛，似乎是在地板上躺了一段時間。",
        " 這是什麼地方？",
        " 你對怎麼來到這個地方毫無頭緒，更糟的是－－",
        " 你好像不知道自己是誰。"
    };

    Event* startEvent = new Event(startEvenLines, startEvenLen);

    // 餐廳事件 1
    int kitchenLen = 10;
    vector<string> kitchenLines = {
        " 你剛進走進房間，一聲尖叫突然傳進你的耳中：",
        "『我的葡萄！我的葡萄！有人偷了我的葡萄！』",
        " 你看向餐桌，一個表情癲狂的人揮舞著叉子大叫。說完他望向你，眼神充滿猜疑與憤怒",
        "『是你吧！是你偷了吧！』",
        " 你 : 不是",
        " 你 : 「不是我偷的，我不知道你在說什麼。」",
        "『不然還有誰！除了你還有誰！』",
        " 你 : 「真的不是我，我不知道！」",
        " 他沈默了幾秒後，當你以為他被說服了，卻又開始繼續大叫",
        " ：『我要我的葡萄！沒有葡萄就沒人可以離開！』",
        " 看來你不得不幫他了，也許剛剛經過的儲藏室裡會有葡萄？"
        };

    Event* kitchen = new Event(kitchenLines, kitchenLen);

    // 教主的房間 2

    int hierarchLens = 7;
    vector<string> hierarchLines = {
        " 突然，你發現你的頭手被禁錮在一個水盆中，雙腕被各割開一道不深的口子。",
        " 鮮血化入了水中，原先平靜無波的水開始扭曲，數百、數千的蟲子蠢蠢欲動，被食物的味道吸引而來，慢慢的匯聚到鮮血的源頭。",
        " 蟲群從手腕蔓延到手臂、脖子、下巴…慢慢覆蓋上你所有浸在水中的身體部位 ",
        " 你的臉上感受到一陣疼痛，是蟲子在啃噬你的皮膚！",
        " 你嚇壞了，掙扎著想逃離卻無法動彈。恐慌中你嗆到了水，缺氧讓意識模糊，你只記得你在意識消散前，隱隱看到蟲子開始覆上了眼睛…",
        " 你從幻覺中醒來了（理智-10)",
        " 掛畫上畫著一群穿著同樣衣服的人，牽手圍著某個人，從上方有一道光打在被圍住的人身上。"};

    Event* hierarch = new Event(hierarchLines, hierarchLens);
    // 儀式房 vs 篡位者

    int finalEventLens;
    vector<string> finalEventLines = {
        " 當你踏入儀式房時，發現有一個人先你一步來到這裡。",
        " 那個人低著頭，你知道他發現你了，但卻沒有任何動作。",
        " 當你想要繞開他從門離開時，你聽到他開口了：",
        "『你知道你對我們來說多們不公平嗎？』",
        "『我從小就待在這裡，總以為只要我努力，有一天神會看到我，結果呢？』", 
        "『先是你的蠢朋友，再來是你，一個兩個廢物都搶在我前面。』", 
        "『事情變成這樣都是你的錯，你就是該死。』",
        " 你知道他是誰了，他是-- ",
        " 請輸入正確的選項數字：(1) 韓森, (2) 傑米, (3)巴特 ",
        "（提示：日記裡有提過。）"
    };

    Event* finalEvent = new Event(finalEventLines, finalEventLens);

    // 結局1 3

    int ending1Lens;
    vector<string> ending1Lines{
        "你跟漢森一起建了新的霂亙教。",
        "說是一起其實也是你一個人操控了一大群人當勞力。",
        "而你為了操控那一大群人更加勞心勞力。",
        "沒想到當教主比社畜更社畜。",
        "但至少有很多人尊敬你，你的自尊心感到滿足。",
        "Ending 1 - 我才是真正的神明"
    };

    Event* ending1 = new Event(ending1Lines, ending1Lens);

    // 結局2 4
    int ending2Lens;
    vector<string> ending2Lines{
        "你試著壓制著漢森，但他的力氣異常的大，你鬥不過他。",
        "『這樣神明就永遠跟我在一起了！』",
        "漢森血紅著眼睛，癲狂的笑著把玻璃珠往嘴巴裡塞。",
        "當他吞下玻璃珠的那刻，你開始感到頭昏。",
        "你想抬起你的手，卻只看到曾經是手的東西正在滴答滴答的往下流。",
        "你覺得你的視野慢慢地模糊。",
        "你什麼也看不見了。",
        "Ending 3 - 溶解"
    };

    Event* ending2 = new Event(ending2Lines, ending2Lens);

    // 結局3 5

    int ending3Lens;
    vector<string> ending3Lines{
        "離開大門後，你回到了正常的世界，雖然你報了警，但沒有人找到你敘述的建築，一切的事就像一場惡夢一樣，沒留下半點痕跡。",
        "你想要忘掉這一切，卻總覺得芒刺在背，無時不刻感覺受到注視。",
        "你真的離開了嗎？",
        "Ending 3 - 是誰在看我？"
    };

    Event* ending3 = new Event(ending3Lines, ending3Lens);
    // 結局4 6

    int ending4Lens;
    vector<string> ending4Lines{
        
        "你離開了教會。",
        "你報了警，整棟建築充滿了血水，但除了儀式房的那句腐屍，沒有找到任何屍體。",
        "事件造成了軒然大波，而你則低調的回到了正常生活。",
        "你去醫院檢查過，身體一切正常。",
        "雖然你的經歷不可能消失，但你可以感覺到他正慢慢變成淡漠的記憶。",
        "神明不再存在了。",
        "Ending 4 - 沒有神的人生"

    };

    Event* ending4 = new Event(ending4Lines, ending4Lens);

    // 放到EventManager
    EventManager::addEvent(startEvent);
    EventManager::addEvent(kitchen);
    EventManager::addEvent(hierarch);
    EventManager::addEvent(finalEvent);
    EventManager::addEvent(ending1);
    EventManager::addEvent(ending2);
    EventManager::addEvent(ending3);
    EventManager::addEvent(ending4);
    cout << "event初始化完成" << endl;
}