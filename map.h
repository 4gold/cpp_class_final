#include <iostream>
#include <vector>
#include <map>
#ifndef PLAYER_H
#define PLAYER_H
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
    map<Room, vector<Item>> roomItems;
    map<pair<Room, Room>, bool> roomConnections; 
    Map() {
        // 初始化房間之間的連接狀態
        roomConnections[make_pair(ritualRoom, storageRoom)] = false;
        roomConnections[make_pair(storageRoom, ritualRoom)] = false;
        roomConnections[make_pair(storageRoom, restaurant)] = true;
        roomConnections[make_pair(restaurant, storageRoom)] = true;
        roomConnections[make_pair(restaurant, eastCorridor)] = false;
        roomConnections[make_pair(eastCorridor, restaurant)] = false;
        roomConnections[make_pair(eastCorridor, protagonistRoom)] = true;
        roomConnections[make_pair(protagonistRoom, eastCorridor)] = true;
        roomConnections[make_pair(eastCorridor, usurperRoom)] = true;
        roomConnections[make_pair(usurperRoom, eastCorridor)] = true;
        roomConnections[make_pair(leaderRoom, laboratory)] = false;
        roomConnections[make_pair(laboratory, leaderRoom)] = false;
        roomConnections[make_pair(restaurant, kitchen)] = false;
        roomConnections[make_pair(kitchen, restaurant)] = false;
    }

    void updateConnection(int value) {
        switch (value) {
        case 1:
            roomConnections[make_pair(ritualRoom, storageRoom)] = true;
            roomConnections[make_pair(storageRoom, ritualRoom)] = true;
            break;
        case 2:
            roomConnections[make_pair(restaurant, eastCorridor)] = true;
            roomConnections[make_pair(eastCorridor, restaurant)] = true;
            break;
        case 3:
            roomConnections[make_pair(leaderRoom, laboratory)] = true;
            roomConnections[make_pair(laboratory, leaderRoom)] = true;
            break;
        // 添加其他情況的處理
        default:
            cout << "Invalid value for updating connection." << endl;
            break;
        }
    }
    //加item 
    void addItemToRoom(Room room, const Item& item) {
        roomItems[room].push_back(item);
    }

    // 判斷角色移動時路徑是否合理
    bool isValidMove(Room currentRoom, Room nextRoom) {
     auto connection = roomConnections.find(make_pair(currentRoom, nextRoom));
        if (connection != roomConnections.end()&&connection->second) {
        	switch(nextRoom){
        		case 0:
        			cout<<"你靠著微弱的光線，隱約認出這是個石質的房間。地板上散落著一些東西，而房間中央有一個奇怪的的盆子。門的對面有個櫃子靠在牆上。"<<endl;
        			break;
        		case 4:
        			cout<<"右邊的長廊上有幾個呆站的人，穿著一樣的精緻衣服。"<<endl;
        			break;
        		case 5:
        			cout<<"左邊的長廊，牆上精緻的牆面雕刻，和一些標語。"<<endl;
        			break;
        		case 8:
        			cout<<"奢華、井井有條、燈光明亮但優雅。"<<endl;
        			break;
        		case 9:
        			cout<<"穿過掛畫後的空間，是充滿奇怪瓶瓶罐罐的實驗室，正中間有一個乾淨的實驗床。"<<endl;
        			break;
			}
            return true;
        }

        return false;
    }
    void print(){
    	 for (const auto& roomItemPair : roomItems) {
            cout << "Room " << roomItemPair.first << " items: ";
            for (const auto& item : roomItemPair.second) {
                cout << item.name << " ";
            }
            cout << endl;
        }
	}
    Map(const Map& other) {
    // 複製 roomItems
        for (const auto& roomItemPair : other.roomItems) {
            roomItems[roomItemPair.first] = roomItemPair.second;
        }

        // 複製 roomConnections
        for (const auto& connectionPair : other.roomConnections) {
            roomConnections[connectionPair.first] = connectionPair.second;
        }
    }
};
#endif
