#include<iostream>
#include<string>
#include<cstdlib>


#include "Dialog.h"
#include "Item.h"
#ifndef PUREITEM_H
#define PUREITEM_H


using namespace std;

/**
 *  非人的道具。
 *  dialog的狀態受到lock影響。
 *  使用完請把disable設為true將道具隱形
 *  地圖移動類道具，在解鎖後會自動轉換為移動
*/
class PureItem : public Item {
    private:
        bool lock; // 此道具是否被鎖定（未完成劇情前置等），True可使用，false不可使用。
        bool takable; // 是否可以被玩家拿取，可拿取的物件在互動完自動進入主角的背包。
        int page; // default 1, 日記等要翻頁的物件用。

        bool attackedAct();
        bool interactedAct();
        bool talkedAct();
        bool observedAct();
        int updateLock(); // 更新道具鎖定的狀態
        int updatephase();
    public:
        PureItem() : Item() {};
        PureItem(const string name, const bool disable, const Dialog* dialog, const int effect[4][3],
                 const bool lock, const bool takable, const int page) 
        : Item(name, disable, dialog, effect), lock(lock), takable(takable), page(page) {};
        PureItem(const string name, const bool disable, const int effect[4][3],
                 const bool lock, const bool takable, const int page) 
        : Item(name, disable, effect), lock(lock), takable(takable), page(page) {
            this->dialog->loadPureItemDialog(this->name, this->lock, this->page);
        };
        ~PureItem() {};

};



bool PureItem::attackedAct() {
    defaultAct(INTERACT_TYPE::ATTACK, INTERACT_TYPE_H::TYPE_PUREITEM, static_cast<int>(lock));
    return true;
}

bool PureItem::interactedAct() {
    // print dialog
    defaultAct(INTERACT_TYPE::INTERACT, INTERACT_TYPE_H::TYPE_NPC, static_cast<int>(this->lock));

    // 修改相關的物件
    int len = this->relatedItem.size();
    for (int i = 0; i < len; i++) {
        RELATED_STATE changeTarget = this->relatedItem.back()->second;
        Item* changeItem = this->relatedItem.back()->first;

        if (changeTarget == RELATED_STATE::DISABLE) { // 將對象修改為可用／不可用。
            (changeItem)->updateDisable();
        } else if (changeTarget == RELATED_STATE::PHASE) { // phase 改變，台詞要跟著改變。
            ((PureItem*)changeItem)->dialog->updateNpcDialog(
                changeItem->getName(), updatePhase()
            );
        } else if (changeTarget == RELATED_STATE::LOCK) { // lock 改變，台詞要跟著改變。
            (changeItem)->updateLock();
        }
        relatedItem.pop_back();           
    }

    return true;
}
bool PureItem::talkedAct() {
    defaultAct(INTERACT_TYPE::TALK, INTERACT_TYPE_H::TYPE_PUREITEM, static_cast<int>(lock));
    return true;
}
bool PureItem::observedAct() {
    defaultAct(INTERACT_TYPE::OBSERVE, INTERACT_TYPE_H::TYPE_PUREITEM, static_cast<int>(lock));
    return true;
}

int PureItem::updateLock() {
    this->lock = !(this->lock);
    return this->lock;
}




#endif