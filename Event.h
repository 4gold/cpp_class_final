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


#endif