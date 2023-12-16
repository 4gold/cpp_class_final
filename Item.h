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
        string chineseName;

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
        Item(const string chineseName, const string name, const bool disable, const Dialog* dialog, const int effect[4][3]);
        Item(const string chineseName, const string name, const bool disable, const int effect[4][3]);
        Item(const string chineseName, const string name, const bool disable);
        virtual ~Item();
        string getChineseName() const;
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




#endif