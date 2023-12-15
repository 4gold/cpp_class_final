#include "PureItem.h"


bool PureItem::getTakable() const {
    return this->takable;
}

bool PureItem::attackedAct() {
    defaultAct(INTERACT_TYPE::ATTACK, INTERACT_TYPE_H::TYPE_PUREITEM, static_cast<int>(lock));
    return true;
}

bool PureItem::interactedAct() {
    // print dialog
    defaultAct(INTERACT_TYPE::INTERACT, INTERACT_TYPE_H::TYPE_PUREITEM, static_cast<int>(this->lock));

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



