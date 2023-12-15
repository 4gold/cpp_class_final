#include<iostream>
#include<string>
#include<cstdlib>
#include<time.h>


#include "Item.h"
#include "InteractType.h"
#include "Dialog.h"
#include "Player.h"

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
        Player* player;
        
        bool attackedAct();
        bool interactedAct();
        bool talkedAct();
        bool observedAct();

        bool fightSim(int npcHealth, int playerHealth); // 觸發戰鬥
        void setDead(); // 設為死亡
        int updatePhase();
    public:
        
        HumanItem() : Item(), health(5), phase(INIT_PHASE){
            this->type = INTERACT_TYPE_H::TYPE_NPC;
        };
        HumanItem(const string name, const bool disable, const Dialog* dialog, const int effect[4][3],
                  const int health, Player* player) 
        : Item(name, disable, dialog, effect), health(health), phase(INIT_PHASE) {
            this->type = INTERACT_TYPE_H::TYPE_NPC;
            this->dialog->loadNpcDialog(name, phase);
            this->player = player;
        };
        HumanItem(const string name, const bool disable, const int effect[4][3],
                  const int health, Player* player) 
        : Item(name, disable, effect), health(health), phase(INIT_PHASE) {
            this->type = INTERACT_TYPE_H::TYPE_NPC;
            this->dialog->loadNpcDialog(name, phase);
            this->player = player;
        };
        HumanItem(const string name, const bool disable, 
                  const int health, Player* player) 
        : Item(name, disable), health(health), phase(INIT_PHASE) {
            this->type = INTERACT_TYPE_H::TYPE_NPC;
            this->dialog->loadNpcDialog(name, phase);
            this->player = player;
        };

        ~HumanItem() {};
        
};


#endif