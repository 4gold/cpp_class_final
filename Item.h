#include<iostream>
#include<string>
#include<cstdlib>
#include<vector>

#include "Dialog.h"
#include "InteractType.h"
#include "FuncPool.h"
#ifndef ITEM_H
#define ITEM_H

using namespace std;

const std::vector<std::string> split(const std::string& str, const std::string& pattern);

/**
 * Item 分為兩種：PureItem 和 HumanItem
 * HumanItem 會有 dead 以及 alive 的兩種狀態。alive狀態下的human也會有不同階段的台詞。
 * PureItem 有幾個狀態要確認 (1) 是否已使用 (2) 有前置使用條件 (3) 需要翻頁 (4) 有幻覺的
 * 故要由外部獲得的資訊應有：(1) player 目前的持有狀態 (2) player 目前的狀態（幻覺）
 *  
 * 目前設計觸發update的時機點為：每次互動時會去檢查與自己相關的道具。
 * */

// 處理羊寶寶、木櫃，開啟 map 的 connection
class Item {
    friend class Dialog;
    protected:
        string name; 
        string type; // npc or item
        bool disable; // 預設 false，使用過的物品設為true, true的物品不顯示在地圖背包中。
        Dialog* dialog;

        /*
          與此物件互動對player造成的影響
          effect[0] --> talk; 
          effect[1] --> attack; 
          effect[2] --> interact; 
          effect[3] --> observe; 
          
          每行依序是：health, sanity, erosion
        */
        int effect[4][3];
   

        virtual bool attackedAct() { return true; };
        virtual bool interactedAct() { return true; };
        virtual bool talkedAct() { return true; };
        virtual bool observedAct() {return true; };
        void updateEffect(const INTERACT_TYPE action, const int hp, const int sanity, const int erosion);
       
        void defaultAct(INTERACT_TYPE act, string type, int determinant);
    public:
        void addRelatedItem(Item* it, RELATED_STATE rs); // 新增關聯道具與相關性
        /*
          與此物品相關的item，當做到關鍵互動時，會去修改relatedItem的狀態
          可能會修改的參數如下：(1) PureItem-lock (2) HumanItem-phase (3) Item-disable
          pair後方為設定他的互動對象，1 = update lock, 2 = update phase, 3 = disable
          更新完應將pair移除，以防重複觸發
        */       
        vector<pair<Item*, RELATED_STATE>*> relatedItem; 
        Item();
        Item(const Item& item);
        Item(const string name, const bool disable, const Dialog* dialog, const int effect[4][3]);
        Item(const string name, const bool disable, const int effect[4][3]);
        Item(const string name, const bool disable);
        virtual ~Item();
        string getName() const;
        string getType() const;
        bool getDisable() const;
        virtual bool getTakable() const;
        // return 1/0 as true/false, return -1 if it's not PureItem.
        virtual int updateLock() { return -1;}; 
        // return phase if it's HumanItem, return -1 if not.
        virtual int updatePhase() { return -1;}; 
        // 更新disable狀態，會自動設為與現在相反的
        void updateDisable(); 
        // return 對player的影響值，依序是 {health, sanity, erosion, mapConnectionValue}
        // mapConnectionValue 會回傳指定的updateConnection() value, 不打開 Connection 回傳 -1。
        // dialog直接印在terminal
        int* useItem(INTERACT_TYPE action);


        /*
          預設的回覆。
        */
        static Dialog* defaultAliveDialog;
        static Dialog* defaultDeadDialog;
        static Dialog* defaultOpenDialog;
        static Dialog* defaultLockDialog;

        static void initializeDefault();
        
};

Dialog* Item::defaultAliveDialog;
Dialog* Item::defaultDeadDialog;
Dialog* Item::defaultOpenDialog;
Dialog* Item::defaultLockDialog;

Item::Item() {
    this->dialog = new Dialog();
}

Item::Item(const Item& item) {
    this->dialog = new Dialog(*(item.dialog));
    this->name = item.name;
    this->type = item.type;
    this->disable = item.disable;
    this->relatedItem = item.relatedItem;
    std::copy(&(item.effect[0][0]), &(item.effect[0][0]) + 4 * 3, &(this->effect[0][0])); // deep copy dialog
}

Item::Item(const string name, const bool disable, const Dialog* dialog, const int effect[4][3]) {
    this->name = name;
    this->disable = disable;
    this->dialog = new Dialog(dialog);
    std::copy(&effect[0][0], &effect[0][0] + 4 * 3, &(this->effect[0][0])); // deep copy dialog
}

Item::Item(const string name, const bool disable, const int effect[4][3]) {
    this->name = name;
    this->disable = disable;
    this->dialog = new Dialog();
}

Item::Item(const string name, const bool disable) {
    this->name = name;
    this->disable = disable;
    this->dialog = new Dialog();
    for (int i = 0; i < INTERACT_TYPE_CNT; i++) {
        for (int j = 0; j < 3; j++)
            this->effect[i][j] = 0;
    }
    
}

Item::~Item() {
    delete this->dialog;
    if (!this->relatedItem.empty()) {
        for (int i = 0; i < this->relatedItem.size(); i++) {
            relatedItem.pop_back();
        }
        this->relatedItem.clear();
    }
}


string Item::getName() const {
    return this->name;
}

string Item::getType() const  {
    return this->type;
}

bool Item::getDisable() const{
    return this->disable;
}

bool Item::getTakable() const {
    return false;
}

// 根據對應的動作使用物品。
int* Item::useItem(INTERACT_TYPE action) {
    try {
        if (action == INTERACT_TYPE::TALK) talkedAct();
        else if (action == INTERACT_TYPE::ATTACK) attackedAct();
        else if (action == INTERACT_TYPE::INTERACT) interactedAct();
        else if (action == INTERACT_TYPE::OBSERVE) observedAct();
    } catch (exception e) {
        cout << e.what();
    }
    
    int mapValue = -1;
    if (this->name == "plush_toy") mapValue = 4;

    int* result = new int[4];
    std::copy(effect[static_cast<int>(action)], effect[static_cast<int>(action)] + 3, result);
    result[4] = mapValue;
    return result;
}

void Item::updateEffect(const INTERACT_TYPE action, const int hp, const int sanity, const int erosion) {
    this->effect[static_cast<int>(action)][0] = hp;
    this->effect[static_cast<int>(action)][1] = sanity;
    this->effect[static_cast<int>(action)][2] = erosion;
}
void Item::updateDisable() {
    this->disable = !(this->disable);
}

void Item::addRelatedItem(Item* it, RELATED_STATE rs){
    this->relatedItem.push_back(new pair<Item*, RELATED_STATE>(it, rs));
};

// 僅回傳對話的動作
void Item::defaultAct(INTERACT_TYPE act, string type, int determinant) {
    // 區分Item類別決定使用哪個default
    Dialog* defaultDeadOrLockDialog;
    Dialog* defaultAliveOrOpenDialog;

    if (type == INTERACT_TYPE_H::TYPE_PUREITEM) {
        defaultDeadOrLockDialog = Item::defaultLockDialog;
        defaultAliveOrOpenDialog = Item::defaultOpenDialog;
    } else if (type == INTERACT_TYPE_H::TYPE_NPC) {
        defaultDeadOrLockDialog = Item::defaultDeadDialog;
        defaultAliveOrOpenDialog = Item::defaultAliveDialog;
    }

    // print 對應的台詞
    string actDialog = this->dialog->getDialog(act);
    if (actDialog.empty() && determinant == 0) // dead
        FuncPool::delayCout(defaultDeadOrLockDialog->getDialog(act));
    else if (actDialog.empty() && determinant >= 1) // alive default
        FuncPool::delayCout(defaultAliveOrOpenDialog->getDialog(act));
    else { // 有台詞 
        string pattern = "---@--@---";
        vector<string> actDialogs = split(actDialog, pattern);
        if (actDialogs.size() > 1) {
            for (string log : actDialogs) {
                FuncPool::delayCout(log);
                cout << "Enter 進入下一頁" << endl;
                cin.ignore();
            }
        } else {
            FuncPool::delayCout(actDialog);
        }
    }
}

// 初始化default回覆
void Item::initializeDefault() {
    string defaultString = "default";
    Item::defaultAliveDialog = new Dialog();
    Item::defaultAliveDialog->loadNpcDialog(defaultString, 1);
    Item::defaultDeadDialog = new Dialog();
    Item::defaultDeadDialog->loadNpcDialog(defaultString, 0);
    Item::defaultOpenDialog = new Dialog();
    Item::defaultOpenDialog->loadPureItemDialog(defaultString, 1, 0);
    Item::defaultLockDialog = new Dialog();
    Item::defaultLockDialog->loadPureItemDialog(defaultString, 0, 0);
}

const std::vector<std::string> split(const std::string& str, const std::string& pattern) {
    std::vector<std::string> result;
    std::string::size_type begin, end;

    end = str.find(pattern);
    begin = 0;

    while (end != std::string::npos) {
        if (end - begin != 0) {
            result.push_back(str.substr(begin, end-begin)); 
        }    
        begin = end + pattern.size();
        end = str.find(pattern, begin);
    }

    if (begin != str.length()) {
        result.push_back(str.substr(begin));
    }
    return result;        
}



#endif