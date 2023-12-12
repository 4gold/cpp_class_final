
#include<iostream>
#include<string>
#include<cstdlib>

#ifndef INTERACT_TYPE_H
#define INTERACT_TYPE_H

const int INTERACT_TYPE_CNT = 4;
const std::string TYPE_NPC = "npc";
const std::string TYPE_PUREITEM = "item";

// 使用物件的互動類別
enum interactType {
    TALK,
    ATTACK,
    INTERACT,
    OBSERVE
};

typedef enum interactType INTERACT_TYPE;

// 關聯物件的修改標的
enum RelatedState {LOCK, PHASE, DISABLE};

typedef enum RelatedState RELATED_STATE;

#endif