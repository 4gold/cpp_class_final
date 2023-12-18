#include "HumanItem.h"


/* 
    攻擊失敗的結果為：血量理智各-1
    先設為全random，看要不要再依據player狀態加計算。
*/
bool HumanItem::attackedAct() {
    string attackedDialog = dialog->getDialog(INTERACT_TYPE::ATTACK);
    // 未有設特殊的內容，直接開始攻擊
    if (attackedDialog.empty() && phase != 0) {
        int npcHealth = health;
        int playerHealth = player->getHealthPoint();
        bool fightResult = fightSim(npcHealth, playerHealth);
        if (fightResult) {
            updateEffect(INTERACT_TYPE::ATTACK, 0, 0, 10); // 殺人升侵蝕度
            setDead();
        } else 
            updateEffect(INTERACT_TYPE::ATTACK, 1, 1, 0);
    } else if (phase != 0) {// 未死亡且無法戰鬥的對象
        FuncPool::delayCout(dialog->getDialog(INTERACT_TYPE::ATTACK), true);
    } else { // 已死亡對象或無設定統一回覆預設       
        FuncPool::delayCout(HumanItem::defaultDeadDialog->getDialog(INTERACT_TYPE::ATTACK), true);
    }

    return true;
}

// return win = true, lose = false
bool HumanItem::fightSim(int npcHealth, int playerHealth) {
    bool result = false;
    
    srand(time(NULL));
    while (npcHealth > 0 && playerHealth > 0) {

        // random generate hit
        int playerHit = ((double) rand()/ RAND_MAX )* 30 + 1;
        int npcHit = ((double) rand()/ RAND_MAX )* 20 + 1;

        string playerAttackStr = "你的攻擊對" + this->name + "造成" + to_string(playerHit) + "點傷害";
        string npcAttackStr = this->name + "的攻擊對你造成" + to_string(npcHit) + "點傷害";
        
        FuncPool::delayCout(playerAttackStr, true);
        FuncPool::delayCout(npcAttackStr, true);
        
        npcHealth -= playerHit;
        playerHealth -= npcHit;
    }

    // 雙方同時死，無條件判定player勝利
    if ((npcHealth <= 0 && playerHealth <= 0) || (npcHealth <= 0))
        result =  true;
    else result = false;

    FuncPool::delayCout((result ? "戰鬥勝利" : "戰鬥失敗"), true);
    return result;

}

void HumanItem::setDead() {
    this->health = 0;
    this->phase = 0;
}
bool HumanItem::interactedAct() {

    // print dialog
    defaultAct(INTERACT_TYPE::INTERACT, INTERACT_TYPE_H::TYPE_NPC, this->phase);

    // 修改相關的物件
    int len = this->relatedItem.size();
    for (int i = 0; i < len; i++) {
        RELATED_STATE changeTarget = this->relatedItem.back()->second;
        Item* changeItem = this->relatedItem.back()->first;

        if (changeTarget == RELATED_STATE::DISABLE) { // 將對象修改為可用／不可用。
            (changeItem)->updateDisable();
        } else if (changeTarget == RELATED_STATE::PHASE) { // phase 改變，台詞要跟著改變。
            ((HumanItem*)changeItem)->dialog->updateNpcDialog(
                changeItem->getName(), changeItem->updatePhase()
            );
        } else if (changeTarget == RELATED_STATE::LOCK) { // lock 改變，台詞要跟著改變。
            (changeItem)->updateLock();
        }
        relatedItem.pop_back();           
    } 

    return true;
}

bool HumanItem::talkedAct() {
    defaultAct(INTERACT_TYPE::TALK, INTERACT_TYPE_H::TYPE_NPC, this->phase);
    return true;
}

bool HumanItem::observedAct() {
    defaultAct(INTERACT_TYPE::OBSERVE, INTERACT_TYPE_H::TYPE_NPC, this->phase);
    return true;
}

int HumanItem::updatePhase() {
    return ++(this->phase);
}

void HumanItem::reloadDialog() {
    this->dialog->loadNpcDialog(this->name, this->phase);
}