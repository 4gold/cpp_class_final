#include<iostream>
#include<string>
#include<cstdlib>
#include<fstream>

#include "InteractType.h"

#ifndef DIALOG_STRINGS_H
#define DIALOG_STRINGS_H

using namespace std;

/**
 * 統整所有台詞，寫死輸出文字用
 * */
class DialogStrings {
    friend class Dialog;
    private:
        static string* getDialogNPCString(const string name, const int phase);   
        static string* getDialogPureItemString(const string name, const bool lock, const int page);
};


#endif