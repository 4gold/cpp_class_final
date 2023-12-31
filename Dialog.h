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
    friend class Item;
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
        void updatePureItemDialog(const string name, bool lock, int page);
};


#endif