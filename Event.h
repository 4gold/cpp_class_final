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
#include "FuncPool.h"

using namespace std;

#ifndef EVENT_H
#define EVENT_H

class Event
{
private:
    vector<string> lines;
    int len;
    bool active = true;    // event 還沒被觸發時是true, 結束改為false
    bool progress = false; // 正在event中是 true    
    Map presentMap;

public:
    static int progressing;
    Event();
    Event(vector<string> lines, int len);
    ~Event();
    void readLines(); 
    bool checkActive();
    void turnoff();
    void start(int e); // e 為加到 progressing的 Event代號
    void end(int i, Player& p, int s) ; // 關閉active, progress, 之後更新地圖連結狀況，i 為開鎖的case, progressing重製為-1, s為之後準備開始的event編號(加負號)
    bool isprogressing();
};


class EventManager {
private:
    static vector<Event*> events;

public:
    static void addEvent(Event* event) {
        events.push_back(event);
    }

    static Event* getEvent(int index) {
        if (index >= 0 && index < events.size()) {
            return events[index];
        }
        return nullptr; // 或其他適當的錯誤處理
    }
};

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
#endif