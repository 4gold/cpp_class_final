#include<iostream>
#include<string>
#include<cstdlib>
#include<fstream>

#include "InteractType.h"

#ifndef DIALOG_STRINGS_H
#define DIALOG_STRINGS_H

using namespace std;
using hash_t = size_t;

// 用來把字串hash成數字，以用於switch-case
// 參考：https://liam.page/2018/11/05/apply-C-style-string-in-switch-case-clause/
constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;

constexpr hash_t hash_compile_time(const char* str, hash_t last_value = basis) {
    return *str ? hash_compile_time(str + 1, (*str ^ last_value) * prime) : last_value;
}

hash_t hash_run_time(const char* str) {
    hash_t ret = basis;

    while (*str) {
        ret ^= *str;
        ret *= prime;
        str++;
    }

    return ret;
}

constexpr hash_t operator "" _hash(const char* p, size_t) {
    return hash_compile_time(p);
}
//------------------------------------
void setDialogStrings(string* &strs, string talk, string attack, string interact, string observe);


/**
 * 統整所有台詞，寫死輸出文字用
 * */
class DialogStrings {
    friend class Dialog;
    private:
        static string* getDialogNPCString(const string name, const int phase);   
        static string* getDialogPureItemString(const string name, const bool lock, const int page);
};

string* DialogStrings::getDialogNPCString(const string name, const int phase) {
    string* ds = new string[4];
    switch (hash_run_time(name.c_str())) {
        case "cook"_hash: 
            switch (phase)
            {
            case 0:
                setDialogStrings(ds, 
                "",
                "",
                "",
                "死掉的廚師，白色的廚師衣被染成紅色。");
                break;
            case 1:
                setDialogStrings(ds, 
                "「來廚房幹嘛？要吃飯乖乖在餐桌等！」",
                "你看了看他全身的肌肉，放棄了找死的想法。",
                "「幹嘛？」",
                "一個身材魁武的男子，寬鬆的廚師衣服也遮不住他的肌肉線條。");
                break;
            case 2:
                setDialogStrings(ds, 
                "「不要過來！」",
                "",
                "「不要過來！」",
                "他很害怕你。");
            default:
                break;
            }
            break;   
        case "weird_grape_guy"_hash: 
            switch (phase)
            {
            case 0:
                setDialogStrings(ds, 
                "",
                "",
                "",
                "");
                break;
            case 1:
                setDialogStrings(ds, 
                "「葡萄！葡萄！」",
                "整個餐廳的人都盯著你。",
                "「嘿嘿嘿嘿嘿嘿。」",
                "眼匡凹陷、形容枯槁的人，正拿著叉子往空盤不停地揮舞。");
                break;
            case 2: // 拿到葡萄後
                setDialogStrings(ds, 
                "他接走了你手上的葡萄，一口吞下，接著走到餐廳大門幫你把門打開。",
                "整個餐廳的人都盯著你。",
                "「我的葡萄呢？」",
                "眼匡凹陷、形容枯槁的人，正拿著叉子往空盤不停地揮舞。");
            case 3: // 給他葡萄後
                setDialogStrings(ds,
                "他開始專心地吃飯，不理會你了。",
                "",
                "他開始專心地吃飯，不理會你了。",
                "眼匡凹陷、形容枯槁的人，正在吃飯。"
                );
            default:
                break;
            }
            break;   
          
            
        case "default"_hash:
            switch(phase) 
            {
            case 1:
                setDialogStrings(ds, 
                "他一點反應都沒有，你被無視了。",
                "進入戰鬥。",
                "他不想理你。",
                "人。");
                break;
            case 0:
                setDialogStrings(ds, 
                "死人無法交談。",
                "你在屍體上多戳了幾下。",
                "死人無法互動。",
                "死掉的屍體。");
                break;
            }
            break;
        default:
            setDialogStrings(ds, 
            "",
            "",
            "",
            "");
    }
    return ds;
}
// talk, attack, interact, observe
string* DialogStrings::getDialogPureItemString(const string name, const bool lock, const int page) {
    string* ds = new string[4];
    switch (hash_run_time(name.c_str())) {
        case "cabinet"_hash: 
            if (lock) {
                setDialogStrings(ds, 
                "",
                "木櫃的板子破了，木片刺傷了你的手。\n 破掉的櫃子後面是一條小路。",
                "木櫃被什麼東西卡住了，推不動。",
                "有些年頭的木質櫃子，上面沒有放任何東西。你敲了敲櫃子，後面好像是空心的。");
            } else {
                // 獲得燭台後
                setDialogStrings(ds, 
                "",
                "木櫃的板子破了。\n 破掉的櫃子後面是一條小路。",
                "你用燭台敲碎了櫃子，破掉的櫃子後面是一條小路。",
                "有些年頭的木質櫃子，上面沒有放任何東西。你敲了敲櫃子，後面好像是空心的。");
            }
            break;  
        case "broken cabinet"_hash:
            setDialogStrings(ds, 
            "",
            "櫃子已經破了。",
            "",
            "破了個大洞的櫃子，從洞後面露出一條小路。");  
            break;  

        case "diary_lab"_hash:
            setDialogStrings(ds, 
            "",
            "",
            "19xx.10.11 \n實驗對象:S014, 14 歲, 男_報廢\n實驗方法:注射, 20ml \n實驗結果:注射後3分鐘開始嚴重的精神錯亂, 試圖攻擊實驗者 \n---@--@---19xx.10.12 \n實驗對象:S016, 13歲, 女_報廢\n實驗方法:注射, 10ml \n實驗結果:與S014相同\n\n備注:不是劑量問題,需要想別的方法\n---@--@---19xx.11.15\n實驗對象:S025, 12歲, 女\n實驗方法:以鮮血引導其主動進入受試者\n實驗結果:受試者產生強烈幻覺, 精神恍惚\n\n備註:S025受母蠱控制\n19xx.11.25\n加註:S025在看到反射物體時, 陷入嚴重的恐慌症, 移除房間內所有的反光物體\n---@--@---最後一頁是一行混亂的筆跡\n該死的, S025不見了! 其他實驗體也都融的一點不剩了! 到底怎麼回事!\n\n (這是最後一頁了) ",
            "一本記載著不明實驗紀錄的本子。");  
            break;  
        case "default"_hash:
            if (lock)
                setDialogStrings(ds, 
                "沒有反應。",
                "沒有反應。",
                "沒有反應。",
                "物品。"
                );
            else 
                setDialogStrings(ds, 
                "沒有反應。",
                "沒有反應。",
                "沒有反應。",
                "你已經使用過這個道具。"
                );
            break;
        
        default:
            setDialogStrings(ds, 
            "",
            "",
            "",
            "");
    }
    return ds;
}

void setDialogStrings(string* &strs, string talk, string attack, string interact, string observe) {
    strs[static_cast<int>(INTERACT_TYPE::TALK)] = talk;
    strs[static_cast<int>(INTERACT_TYPE::ATTACK)] = attack;
    strs[static_cast<int>(INTERACT_TYPE::INTERACT)] = interact;
    strs[static_cast<int>(INTERACT_TYPE::OBSERVE)] = observe;
}

#endif