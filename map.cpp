#include "map.h"

Map::Map() {
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

 void Map::updateConnection(int value) {
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
        case 4:
            roomConnections[make_pair(kitchen, restaurant)] = true;
            roomConnections[make_pair(restaurant, kitchen)] = true;
            break;
        // 添加其他情況的處理
        default:
            cout << "Invalid value for updating connection." << endl;
            break;
        }
}

//加item 
void Map::addItemToRoom(Room room, Item* item) {
    roomItems[room].push_back(item);
}

// 判斷角色移動時路徑是否合理
bool Map::isValidMove(Room currentRoom, Room nextRoom) {
    auto connection = roomConnections.find(make_pair(currentRoom, nextRoom));
    if (connection != roomConnections.end()&&connection->second) {

        switch(nextRoom){
            case 0:
                FuncPool::readAndPrintFile("儀式房ASCII.txt");
                cout<<"你靠著微弱的光線，隱約認出這是個石質的房間。地板上散落著一些東西，而房間中央有一個奇怪的的盆子。門的對面有個櫃子靠在牆上。"<<endl;
                break;
            case 1:
                FuncPool::readAndPrintFile("儲物間.txt");
                cout<<"看起來像是個儲物間，房間裏有許多箱子。\n只有擺在比較前面的箱子有移動的痕跡，後面的物品看上去許久沒人動了，上面堆了厚厚一層灰。";
                break;
            case 2:
                FuncPool::readAndPrintFile("餐廳ASCII.txt"); 
                cout<<"一個寬廣的空間，房頂挑高超過四米。有一群人坐在中間的大餐桌吃飯。\n房內沒有現代燈具，蠟燭昏暗的火光在空氣中晃動，讓人感到有些惶恐。";
                break;
            case 3:
                FuncPool::readAndPrintFile("廚房ASCII.txt");
                cout<<"從廚房的環境可以看出這裡的廚師跟長相相符的粗野。\n廚房四處是沒有擦乾淨的髒污痕跡，廚具及食材沒有規則的堆放在房間內各處。"<<endl;
                break;
            case 4:
                FuncPool::readAndPrintFile("東長廊ASCII.txt");
                cout<<"右邊的長廊上有幾個呆站的人，穿著一樣的精緻衣服。"<<endl;
                break;
            case 5:
                FuncPool::readAndPrintFile("西長廊ASCII.txt");
                cout<<"左邊的長廊，牆上精緻的牆面雕刻，和一些標語。"<<endl;
                break;
            case 6:
                FuncPool::readAndPrintFile("篡位者房ASCII.txt");
                cout<<"這個房間令你感到熟悉，陳舊卻不髒亂。房內擺設簡單，只有一張床跟一個書桌。你看到有一本書掉在書桌下的角落。"<<endl;
                break;
            case 8:
                FuncPool::readAndPrintFile("主教房ASCII.txt");
                cout<<"你來到一個燈光明亮的房間，一進門就看到一尊比你還高了半個頭的神像擺在門邊，靜靜地注視著來訪的客人們。\n房內的擺設優雅且奢華，地板上甚至有一張雄皮地毯，跟外面的破敗景象形成強烈的對比。\n直直望向房間的底部，一張大型掛畫幾乎遮住了整面牆。"<<endl;
                break;
            case 9:
                FuncPool::readAndPrintFile("實驗室ASCII.txt");
                cout<<"穿過掛畫後的空間，是充滿奇怪瓶瓶罐罐的實驗室，正中間有一個乾淨的實驗床。"<<endl;
                break;
        }
        return true;
    }

    return false;
}

void Map::print(){
    for (const auto& roomItemPair : roomItems) {
        cout << "Room " << roomItemPair.first << " items: ";
        for (const auto& item : roomItemPair.second) {
            cout << item->getName() << " ";
        }
        cout << endl;
    }
}

Map::Map(const Map& other) {
    // 複製 roomItems
    for (const auto& roomItemPair : other.roomItems) {
        roomItems[roomItemPair.first] = roomItemPair.second;
    }

    // 複製 roomConnections
    for (const auto& connectionPair : other.roomConnections) {
        roomConnections[connectionPair.first] = connectionPair.second;
    }
}