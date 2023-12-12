#include<iostream>
#include<string>
#include<cstdlib>
#include<time.h>

#include <chrono>
#include <thread>

#include "InteractType.h"
#include "Dialog.h"
#include "Item.h"
#include "FuncPool.h"
#include "PureItem.h"

#ifndef HUMANITEM_H
#define HUMANITEM_H

using namespace std;

const int INIT_PHASE = 1;

/**
 * npc，會有生命。
 * HumanItem 會有 dead 以及 alive 的兩種狀態。alive狀態下的human也會有不同劇情階段的台詞。
*/
class HumanItem : public Item {
    private:
        int health; // 生命值歸零為死亡，預設 5
        /* 
           phase紀錄目前劇情進展的情況，用以監測Dialog應回傳的數值
           default = 1, 該角色的劇情推進後會增加
           當health歸零時，phase = 0
        */
        int phase; 
        
        bool attackedAct();
        bool interactedAct();
        bool talkedAct();
        bool observedAct();
        void defaultAct(INTERACT_TYPE act);
        bool fightSim(int npcHealth, int playerHealth); // 觸發戰鬥
        void setDead(); // 設為死亡
    public:
        
        HumanItem() : Item(), health(5), phase(INIT_PHASE){
            this->type = INTERACT_TYPE_H::TYPE_NPC;
        };
        HumanItem(const string name, const bool disable, const Dialog* dialog, const int effect[4][3],
                  const int health) 
        : Item(name, disable, dialog, effect), health(health), phase(INIT_PHASE) {
            this->type = INTERACT_TYPE_H::TYPE_NPC;
            this->dialog->loadNpcDialog(name, phase);
        };
        HumanItem(const string name, const bool disable, const int effect[4][3],
                  const int health) 
        : Item(name, disable, effect), health(health), phase(INIT_PHASE) {
            this->type = INTERACT_TYPE_H::TYPE_NPC;
            this->dialog->loadNpcDialog(name, phase);
        };

        ~HumanItem() {};

        static Dialog* defaultAliveDialog;
        static Dialog* defaultDeadDialog;

        static void initializeDefault();
        void updatePhase();
        int* useItem(INTERACT_TYPE action);
};

Dialog* HumanItem::defaultAliveDialog;
Dialog* HumanItem::defaultDeadDialog;

// 直接印dialog
int* HumanItem::useItem(INTERACT_TYPE action) {

    try {
        bool success = true;
        if (action == INTERACT_TYPE::TALK) success = talkedAct();
        else if (action == INTERACT_TYPE::ATTACK) success = attackedAct();
        else if (action == INTERACT_TYPE::INTERACT) success = interactedAct();
        else if (action == INTERACT_TYPE::OBSERVE) success = observedAct();
    } catch (exception e) {
        cout << e.what();
    }
 
    return effect[static_cast<int>(action)];
}

/* 
    攻擊失敗的結果為：血量理智各-1
    先設為全random，看要不要再依據player狀態加計算。
*/
bool HumanItem::attackedAct() {
    string attackedDialog = dialog->getDialog(INTERACT_TYPE::ATTACK);
    // 未有設特殊的內容，直接開始攻擊
    if (attackedDialog.empty() && phase != 0) {
        int npcHealth = health;
        int playerHealth = 5;
        bool fightResult = fightSim(npcHealth, playerHealth);
        if (fightResult) {
            updateEffect(INTERACT_TYPE::ATTACK, 0, 0, 0);
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
        int playerHit = ((double) rand()/ RAND_MAX )* 2 + 1;
        int npcHit = ((double) rand()/ RAND_MAX )* 2 + 1;

        string playerAttackStr = "你的攻擊對" + this->name + "造成" + to_string(playerHit) + "點傷害";
        string npcAttackStr = this->name + "的攻擊對你造成" + to_string(npcHit) + "點傷害";
        
        FuncPool::delayCout(playerAttackStr, true);
        FuncPool::delayCout(npcAttackStr, true);
        
        npcHealth -= playerHit;
        playerHealth -= npcHit;
        
    }

    // 雙方同時死，無條件判定player勝利
    if (npcHealth <= 0 && playerHealth <= 0) 
        result =  true;
    if (npcHealth <= 0) result = true;
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
    defaultAct(INTERACT_TYPE::INTERACT);

    // 修改相關的物件
    int len = this->relatedItem.size();
    for (int i = 0; i < len; i++) {
        RELATED_STATE changeTarget = this->relatedItem.back()->second;
        Item* changeItem = this->relatedItem.back()->first;

        if (changeTarget == RELATED_STATE::DISABLE) { // 將對象修改為可用／不可用。
            ((HumanItem*)changeItem)->updateDisable();
        } else if (changeTarget == RELATED_STATE::PHASE) { // phase 改變，台詞要跟著改變。
            ((HumanItem*) changeItem)->dialog->updateNpcDialog(
                changeItem->getName(), ++(((HumanItem*) changeItem)->phase)
            );
        } else if (changeTarget == RELATED_STATE::LOCK) { // lock 改變，台詞要跟著改變。
            ((PureItem*) changeItem)->updateLock();
        }
        relatedItem.pop_back();           
    } 

    return true;
}

bool HumanItem::talkedAct() {
    defaultAct(INTERACT_TYPE::TALK);
    return true;
}

bool HumanItem::observedAct() {
    defaultAct(INTERACT_TYPE::OBSERVE);
    return true;
}

void HumanItem::defaultAct(INTERACT_TYPE act) {
    string actDialog = this->dialog->getDialog(act);
    if (actDialog.empty() && this->phase == 0) // dead
        FuncPool::delayCout(defaultDeadDialog->getDialog(act));
    else if (actDialog.empty() && this->phase >= 1) // alive default
        FuncPool::delayCout(defaultAliveDialog->getDialog(act));
    else // 有台詞
        FuncPool::delayCout(actDialog);
}


void HumanItem::initializeDefault() {
    HumanItem::defaultAliveDialog = new Dialog();
    HumanItem::defaultAliveDialog->loadNpcDialog("default", 1);
    HumanItem::defaultDeadDialog = new Dialog();
    HumanItem::defaultDeadDialog->loadNpcDialog("default", 0);
}

#endif