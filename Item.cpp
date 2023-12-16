
#include "Item.h"

const std::vector<std::string> split(const std::string& str, const std::string& pattern);

Dialog* Item::defaultAliveDialog;
Dialog* Item::defaultDeadDialog;
Dialog* Item::defaultOpenDialog;
Dialog* Item::defaultLockDialog;

Item::Item() {
    this->dialog = new Dialog();
}

Item::Item(const Item& item) {
    this->dialog = new Dialog(*(item.dialog));
    this->name = item.name;
    this->type = item.type;
    this->disable = item.disable;
    this->relatedItem = item.relatedItem;
    std::copy(&(item.effect[0][0]), &(item.effect[0][0]) + 4 * 3, &(this->effect[0][0])); // deep copy dialog
}

Item::Item(const string name, const bool disable, const Dialog* dialog, const int effect[4][3]) {
    this->name = name;
    this->disable = disable;
    this->dialog = new Dialog(dialog);
    std::copy(&effect[0][0], &effect[0][0] + 4 * 3, &(this->effect[0][0])); // deep copy dialog
}

Item::Item(const string name, const bool disable, const int effect[4][3]) {
    this->name = name;
    this->disable = disable;
    this->dialog = new Dialog();
}

Item::Item(const string name, const bool disable) {
    this->name = name;
    this->disable = disable;
    this->dialog = new Dialog();
    for (int i = 0; i < INTERACT_TYPE_CNT; i++) {
        for (int j = 0; j < 3; j++)
            this->effect[i][j] = 0;
    }
    
}

Item::~Item() {
    delete this->dialog;
    if (!this->relatedItem.empty()) {
        for (int i = 0; i < this->relatedItem.size(); i++) {
            relatedItem.pop_back();
        }
        this->relatedItem.clear();
    }
}


string Item::getName() const {
    return this->name;
}

string Item::getType() const  {
    return this->type;
}

bool Item::getDisable() const{
    return this->disable;
}

bool Item::getTakable() const {
    return false;
}

// 根據對應的動作使用物品。
int* Item::useItem(INTERACT_TYPE action) {
    try {
        if (action == INTERACT_TYPE::TALK) talkedAct();
        else if (action == INTERACT_TYPE::ATTACK) attackedAct();
        else if (action == INTERACT_TYPE::INTERACT) interactedAct();
        else if (action == INTERACT_TYPE::OBSERVE) observedAct();
    } catch (exception e) {
        cout << e.what();
    }
    
    int mapValue = -1;
    if (this->name == "plush_toy") mapValue = 4;

    int* result = new int[4];
    std::copy(effect[static_cast<int>(action)], effect[static_cast<int>(action)] + 3, result);
    result[4] = mapValue;
    return result;
}

void Item::updateEffect(const INTERACT_TYPE action, const int hp, const int sanity, const int erosion) {
    this->effect[static_cast<int>(action)][0] = hp;
    this->effect[static_cast<int>(action)][1] = sanity;
    this->effect[static_cast<int>(action)][2] = erosion;
}
void Item::updateDisable() {
    this->disable = !(this->disable);
}

void Item::addRelatedItem(Item* it, RELATED_STATE rs){
    this->relatedItem.push_back(new pair<Item*, RELATED_STATE>(it, rs));
};

// 僅回傳對話的動作
void Item::defaultAct(INTERACT_TYPE act, string type, int determinant) {
    // 區分Item類別決定使用哪個default
    Dialog* defaultDeadOrLockDialog;
    Dialog* defaultAliveOrOpenDialog;

    if (type == INTERACT_TYPE_H::TYPE_PUREITEM) {
        defaultDeadOrLockDialog = Item::defaultLockDialog;
        defaultAliveOrOpenDialog = Item::defaultOpenDialog;
    } else if (type == INTERACT_TYPE_H::TYPE_NPC) {
        defaultDeadOrLockDialog = Item::defaultDeadDialog;
        defaultAliveOrOpenDialog = Item::defaultAliveDialog;
    }

    // print 對應的台詞
    string actDialog = this->dialog->getDialog(act);
    if (actDialog.empty() && determinant == 0) // dead
        FuncPool::delayCout(defaultDeadOrLockDialog->getDialog(act));
    else if (actDialog.empty() && determinant >= 1) // alive default
        FuncPool::delayCout(defaultAliveOrOpenDialog->getDialog(act));
    else { // 有台詞 
        string pattern = "---@--@---";
        vector<string> actDialogs = split(actDialog, pattern);
        if (actDialogs.size() > 1) {
            for (string log : actDialogs) {
                FuncPool::delayCout(log);
                cout << "Enter 進入下一頁" << endl;
                cin.ignore();
            }
        } else {
            FuncPool::delayCout(actDialog);
        }
    }
}

// 初始化default回覆
void Item::initializeDefault() {
    string defaultString = "default";
    Item::defaultAliveDialog = new Dialog();
    Item::defaultAliveDialog->loadNpcDialog(defaultString, 1);
    Item::defaultDeadDialog = new Dialog();
    Item::defaultDeadDialog->loadNpcDialog(defaultString, 0);
    Item::defaultOpenDialog = new Dialog();
    Item::defaultOpenDialog->loadPureItemDialog(defaultString, 1, 0);
    Item::defaultLockDialog = new Dialog();
    Item::defaultLockDialog->loadPureItemDialog(defaultString, 0, 0);
}

const std::vector<std::string> split(const std::string& str, const std::string& pattern) {
    std::vector<std::string> result;
    std::string::size_type begin, end;

    end = str.find(pattern);
    begin = 0;

    while (end != std::string::npos) {
        if (end - begin != 0) {
            result.push_back(str.substr(begin, end-begin)); 
        }    
        begin = end + pattern.size();
        end = str.find(pattern, begin);
    }

    if (begin != str.length()) {
        result.push_back(str.substr(begin));
    }
    return result;        
}

