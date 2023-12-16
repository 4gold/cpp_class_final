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
        cout << itemsInCurrentRoom[i]->getName() << ", ";
    }
    cout << endl;
}


void initializeAllItems(map<string, Item*> &items, Player* player);
void putItemsInRoom(map<string, Item*> &items, Map& map);
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
    initializeAllItems(allItems, pPtr);
    
    putItemsInRoom(allItems, m);

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
        cout << "EventProgress : " << eventProgress << endl;

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
        if (cmds.size() > 2) {
            cout << "請輸入有效指令";
            continue;
        } 
        cmd = cmds[0];
        obj = cmds.back();

        Item* objItem;

        // 確認obj是否為存在此房間且可視的item 或 存在的房間

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
            if (cmd == "switchMap")
            {
                
                if (convertedNumber == 2)
                {   
                    
                    g.switchMap(convertedNumber);

                    // 更新現有房間內的物品
                    itemsInCurrentRoom = m.roomItems[static_cast<Map::Room>(currentPlayerRoom)];
                    showValidItemsInRoom(itemsInCurrentRoom, Itemcnt);

                    if ((kitchen->isactive()))
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
                    if (bag[i]->getName() == "grape")
                    {
                        cout << "『謝謝你！你不是葡萄小偷！』" << endl;
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
                    showValidItemsInRoom(itemsInCurrentRoom, Itemcnt);

                    if ((hierarch->isactive()))
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
                    cout << "要離開房間時，神像會倒下撞破掛畫，發現後面的實驗室。同時也會發現神像裡面有屍體" << endl;
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
                if ((bag[i]->getName() == "key_to_outside") and ending == 0)
                {
                    int sp = p.getSanityPoint();
                    int ep = p.getErosionPoint();
                    ending = story.checkEnding(sp, ep);
                }
                else if ((bag[i]->getName() == "key_to_outside") and cmd == "switchMap" and convertedNumber == 0)
                {
                    g.switchMap(convertedNumber);

                    // 輸出當前房間中的物品名稱
                    showValidItemsInRoom(itemsInCurrentRoom, Itemcnt);

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
                cout << "台詞" << endl;
                p.updateHealthPoint(currentHp-10);
                // 看還有沒有要做甚麼

                if ((cmd == "attack" and obj == "usurper"))
                {
                    // 檢查玩家是否有鏡子1~3
                    int mirrorNum = 0;
                    for (int i = 0; i < numInBag; i++)
                    {
                        if ((bag[i]->getName() == "piece_of_mirror_1") or (bag[i]->getName() == "piece_of_mirror_2") or (bag[i]->getName() == "piece_of_mirror_3"))
                        {
                            mirrorNum++;
                        }
                    }
                    if (mirrorNum == 3) // 鏡子蒐集齊
                    {   
                        g.attack(objItem); 
                        if (ending == 1) // 侵蝕度高->結局1
                        {   
                            cout << "台詞" << endl;
                            finalEvent->end(1, p, 4);
                            finalEvent->turnoff();
                            ending1->start(4);
                            break;
                        }
                        else if (ending == 4) // 侵蝕度低->結局4
                        {
                            cout << "台詞" << endl;
                            finalEvent->end(1, p, 7);
                            finalEvent->turnoff();
                            ending4->start(7);
                            break;
                        }
                    }
                    else // 鏡子沒蒐集齊->結局2
                    {   
                        cout << "台詞" << endl;
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
                cout << "台詞" << endl;
                finalEvent->end(1, p, 5);
                finalEvent->turnoff();
                ending4->start(7);
                alive = false;
                break;
            }
            else // 答錯，進入結局2
            {
                cout << "亂答台詞" << endl;
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
                
                // 更新現有房間內的物品
                itemsInCurrentRoom = m.roomItems[static_cast<Map::Room>(currentPlayerRoom)];

                // // 輸出當前房間中的物品名稱
                cout << "這個房間有 : ";
                for (int i = 0; i < Itemcnt; i++)
                {
                    if(!itemsInCurrentRoom[i]->getDisable())
                    {
                    cout << itemsInCurrentRoom[i]->getName() << ", ";
                    }
                }
                cout << endl;
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
    items.insert(pair<string, Item*>("long_candlestick", long_candlestick));
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

void putItemsInRoom(map<string, Item*> &items, Map& gameMap) {

    gameMap.addItemToRoom(gameMap.ritualRoom, (items["cabinet"]));
    gameMap.addItemToRoom(gameMap.ritualRoom, (items["long_candlestick"]));
    gameMap.addItemToRoom(gameMap.ritualRoom, (items["usurper"]));
    gameMap.addItemToRoom(gameMap.ritualRoom, (items["god_pearl"]));
    
    
    gameMap.addItemToRoom(gameMap.storageRoom, (items["grape"]));

    gameMap.addItemToRoom(gameMap.restaurant, (items["cook"]));
    gameMap.addItemToRoom(gameMap.restaurant, (items["weird_grape_guy"]));

    gameMap.addItemToRoom(gameMap.leaderRoom, (items["door_leader_to_west_hallway"]));
    gameMap.addItemToRoom(gameMap.leaderRoom, (items["painting_on_wall"]));
    gameMap.addItemToRoom(gameMap.leaderRoom, (items["sculpture"]));

    gameMap.addItemToRoom(gameMap.laboratory, (items["diary_lab"]));
    gameMap.addItemToRoom(gameMap.laboratory, (items["trash_can"]));
    gameMap.addItemToRoom(gameMap.laboratory, (items["piece_of_mirror_2"]));

    gameMap.addItemToRoom(gameMap.kitchen, (items["sink"]));
    gameMap.addItemToRoom(gameMap.kitchen, (items["piece_of_mirror_3"]));

    gameMap.addItemToRoom(gameMap.protagonistRoom, (items["plush_toy"]));
    gameMap.addItemToRoom(gameMap.protagonistRoom, (items["piece_of_mirror_1"]));
    gameMap.addItemToRoom(gameMap.protagonistRoom, (items["diary_player"]));
    gameMap.addItemToRoom(gameMap.protagonistRoom, (items["drawer"]));

    cout << gameMap.roomItems[gameMap.ritualRoom].back()->getName();
    
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
        " 突然餐桌傳來一聲尖叫：",
        "『我的葡萄！我的葡萄！有人偷了我的葡萄！』",
        " 你看向餐桌，一個表情癲狂的人揮舞著叉子大叫。說完他望向你，眼神充滿猜疑與憤怒",
        "『是你吧！是你偷了吧！』",
        " 你 : 不是",
        " 你 : 「不是我偷的，我不知道你在說什麼。」",
        "『不然還有誰！除了你還有誰！』",
        " 你 : 「真的不是我，我不知道！」",
        " 他沈默了幾秒後，當你以為他被說服了，卻又開始繼續大叫",
        " ：『我要我的葡萄！沒有葡萄就沒人可以離開！』"
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
        " 你從幻覺中醒來了（理智-1)",
        " 掛畫上畫著一群穿著同樣衣服的人，牽手圍著某個人，從上方有一道光打在被圍住的人身上。"};

    Event* hierarch = new Event(hierarchLines, hierarchLens);
    // 儀式房 vs 篡位者

    int finalEventLens;
    vector<string> finalEventLines = {};

    Event* finalEvent = new Event(finalEventLines, finalEventLens);

    // 結局1 3

    int ending1Lens;
    vector<string> ending1Lines{};

    Event* ending1 = new Event(ending1Lines, ending1Lens);

    // 結局2 4
    int ending2Lens;
    vector<string> ending2Lines{};

    Event* ending2 = new Event(ending2Lines, ending2Lens);

    // 結局3 5

    int ending3Lens;
    vector<string> ending3Lines{};

    Event* ending3 = new Event(ending3Lines, ending3Lens);
    // 結局4 6

    int ending4Lens;
    vector<string> ending4Lines{};

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