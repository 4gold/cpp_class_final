#include<iostream>
#include<string>
#include<cstdlib>
#include<fstream>

#include "InteractType.h"
#include "DialogStrings.h"

#ifndef DIALOG_H
#define DIALOG_H

using namespace std;

/**
 * 儲存各種互動的對白。
 * 在初始的時候會讀入所有對白，沒有對白的會維持empty。
 * 在讀取時為空白者，請自行調用函式改取default。
*/
class Dialog{
    friend class HumanItem;
    friend class PureItem;
    private:
        //依序為{talk, Attack, Interact, Observe}
        string* dialogues;
        void loadNpcDialog(string name, int phase);
        void loadPureItemDialog(string name, bool lock, int page);
    public:
        Dialog();
        Dialog(string name);
        Dialog(string* dialogues);
        Dialog(const Dialog* &dialog) : Dialog(dialog->dialogues) {};
        ~Dialog();
        string getDialog(INTERACT_TYPE actType);

        // 切換為新的dialog
        void updateNpcDialog(const string name, const int phase);
        void updatePureItemDialog(const string name, bool lock);
};

Dialog::Dialog() {
    this->dialogues = new string[INTERACT_TYPE_H::INTERACT_TYPE_CNT];
}
Dialog::Dialog(string itemName) {
    this->dialogues = new string[INTERACT_TYPE_H::INTERACT_TYPE_CNT];
}

Dialog::Dialog(string* dialogues) {
    this->dialogues = new string[INTERACT_TYPE_H::INTERACT_TYPE_CNT];
    for (int i = 0; i < INTERACT_TYPE_H::INTERACT_TYPE_CNT; i++)
        this->dialogues[i] = dialogues[i];
}

Dialog::~Dialog() {
    if (this->dialogues != nullptr)
        delete [] this->dialogues;
    this->dialogues = nullptr;
}

string Dialog::getDialog(const INTERACT_TYPE actType) {
    return this->dialogues[static_cast<int>(actType)];
}

// phase = 0 代表 死了，台詞替換為 dead。
// phase >= 1 代表 任務有進展，替換為此 phase 的台詞。
void Dialog::updateNpcDialog(const string name, const int phase) {
    loadNpcDialog(name, phase);
};

// 
void Dialog::updatePureItemDialog(const string name, bool lock, int page) {
    loadPureItemDialog(name, lock, page);
};

void Dialog::loadNpcDialog(string name, int phase) {
    if (this->dialogues != nullptr)
        delete [] this->dialogues;
    this->dialogues = DialogStrings::getDialogNPCString(name, phase);
}

void Dialog::loadPureItemDialog(string name, bool lock, int page) {

}


#endif