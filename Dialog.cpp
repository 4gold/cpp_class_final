#include "Dialog.h"


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

// true = lock/ false = open
// 如果page = 0 表示不需換頁，多頁
void Dialog::updatePureItemDialog(const string name, bool lock, int page) {
    loadPureItemDialog(name, lock, page);
};

void Dialog::loadNpcDialog(string name, int phase) {
    if (this->dialogues != nullptr)
        delete [] this->dialogues;
    this->dialogues = DialogStrings::getDialogNPCString(name, phase);
}

void Dialog::loadPureItemDialog(string name, bool lock, int page) {
    if (this->dialogues != nullptr)
        delete [] this->dialogues;
    this->dialogues = DialogStrings::getDialogPureItemString(name, lock, page);
}
