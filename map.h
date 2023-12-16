#include <iostream>
#include <vector>
#include <map>
#include "FuncPool.h"
#include "Item.h"

#ifndef MAP_H
#define MAP_H
using namespace std;

// 定義Map類別，代表地圖
class Map {
private:
   
public:
    // 在指定房間添加物品，依序為0-8號 
     enum Room {
        ritualRoom,//0:儀式房 
        storageRoom,//1儲藏室 
        restaurant,//2餐廳 
		kitchen,//3廚房 
        eastCorridor,//4東長廊 
        westCorridor,//5西長廊 
        protagonistRoom,//6主角房 
        usurperRoom,//7篡位者房 
        leaderRoom,//8教主房 
        laboratory//9實驗室  
    };

    // 使用map來儲存每個房間的物品
    map<Room, vector<Item*>> roomItems;
    map<pair<Room, Room>, bool> roomConnections; 
    Map();

    void updateConnection(int value);
    //加item 
    void addItemToRoom(Room room, Item* item);

    // 判斷角色移動時路徑是否合理
    bool isValidMove(Room currentRoom, Room nextRoom);
    void print();
    Map(const Map& other);
};


#endif
