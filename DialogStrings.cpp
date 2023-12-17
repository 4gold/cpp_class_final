#include "DialogStrings.h"

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
string diary_player();


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
                break;
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
                "「我的葡萄呢？快給我葡萄！」",
                "眼匡凹陷、形容枯槁的人，正拿著叉子往空盤不停地揮舞。");
                break;
            case 2: // 拿到葡萄後
                setDialogStrings(ds, 
                "「我的葡萄呢？」",
                "整個餐廳的人都盯著你。",
                "他接走了你手上的葡萄，一口吞下，接著走到餐廳大門幫你把門打開。",
                "眼匡凹陷、形容枯槁的人，正拿著叉子往空盤不停地揮舞。");
                break;
            case 3: // 給他葡萄後
                setDialogStrings(ds,
                "他開始專心地吃飯，不理會你了。",
                "",
                "他開始專心地吃飯，不理會你了。",
                "眼匡凹陷、形容枯槁的人，正在吃飯。"
                );
                break;
            default:
                break;
            }
            break;   
        case "usurper"_hash: 
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
                " \"我很嫉妒你, 你知道嗎?\"  ---@--@--- \"神從來看不到我, 我等了十年, 卻是你先入了神的眼?'\"",
                "",
                "\"我等你很久了\"",
                "一個面色陰沈的人，死死的盯著你。");
                break;
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
        case "grape"_hash:
            setDialogStrings(ds, 
            "我還沒失去理智到想與葡萄對話。",
            "在下手前的一瞬間，心頭的一陣寒意遏止了你的行動。",
            "你拿起了一顆葡萄在手中翻弄，手感有些詭異。突然，你和『葡萄』對上了眼。這難道是塗成紫色的...",
            "形狀有點奇怪的葡萄。");  
            break;
        case "painting_on_wall"_hash:
            if (lock) {
                setDialogStrings(ds, 
                "",
                "你對破壞這幅畫有強烈的牴觸感。",
                "掛畫上畫著一群穿著同樣衣服的人，牽手圍著某個人，從上方有一道光打在被圍住的人身上。",
                "一幅大型掛畫。");
            } else {
                // 掛畫撕碎後
                setDialogStrings(ds, 
                "",
                "再破壞也沒有意義了。",
                "掛畫已經被撕碎得看不清原本的畫了。",
                "壞掉的大型掛畫。");
            }
            break;
        case "sculpture"_hash:
            if (lock) {
                setDialogStrings(ds, 
                "你在期待它回你話嗎?",
                "你發現你攻擊雕像的手上沾到了一些顏料。",
                "",
                "一個比你稍高一些的雕像，真實地讓你感到不自在。");
            } else {
                // 雕像倒下後
                setDialogStrings(ds, 
                "你在期待它回你話嗎?",
                "別吧？",
                "",
                "碎掉的雕像倒在地上，從石像碎裂的縫隙可以窺見人的皮膚。");
            }
            break;
        case "trash_can"_hash:
            if (lock) {
                setDialogStrings(ds, 
                "",
                "你踢了垃圾桶，發現垃圾桶被固定在地上。",
                "你翻了翻垃圾桶，找到一片鏡子碎片。",
                "一個實驗室用的垃圾桶。");
            } else {
                // 拿走鏡子碎片2後
                setDialogStrings(ds, 
                "",
                "你踢了垃圾桶，發現垃圾桶被固定在地上。",
                "垃圾桶已經空了。",
                "一個實驗室用的垃圾桶。");
            }
            break;
        case "piece_of_mirror_1"_hash: 
        case "piece_of_mirror_2"_hash: 
        case "piece_of_mirror_3"_hash: 
            setDialogStrings(ds, 
            "它只會模仿我的嘴型，不會發出聲音。",
            "已經是碎片了。",
            "你撿起一片碎掉的鏡子放進背包。",
            "映射出扭曲的臉，一盯著看就覺得頭痛。");
            break;
        case "diary_player"_hash: 
            setDialogStrings(ds, 
            "",
            "",
            diary_player(),
            "一本發發黃的日記，字跡看起來挺新的。");
            break;
        case "drawer"_hash:
            if (lock) {
                setDialogStrings(ds, 
                "「多拉Ａ夢？」沒有藍色機器貓回應你。",
                "你再敲也不會讓他變得比較好開。",
                "裡面有一片碎掉的鏡子。",
                "書桌的抽屜，拉起來有點卡。");
            } else {
                // 拿走鏡子碎片2後
                setDialogStrings(ds, 
                "「多拉Ａ夢？」沒有藍色機器貓回應你。",
                "你再敲也不會讓他變得比較好開。",
                "抽屜沒有其他東西了。",
                "書桌的抽屜，拉起來有點卡。");
            }
            break;
        case "plush_toy"_hash: 
            setDialogStrings(ds, 
            "我不會說羊語。",
            "布絨吸收了你的攻擊",
            "你拿起了破爛的羊寶寶玩偶。",
            "羊寶寶玩偶，看起來破舊還有點髒。");
            break;
        case "sink"_hash:
            if (lock) {
                setDialogStrings(ds, 
                "",
                "你假裝要打向水槽，然後把手洗了乾淨。",
                "你拿起了鏡子碎片。",
                "廚房的水槽，你看到裡面有一片鏡子碎片");
            } else {
                // 拿走鏡子碎片2後
                setDialogStrings(ds, 
                "",
                "你假裝要打向水槽，然後把手洗了乾淨。",
                "水槽裡沒有其他東西了。",
                "廚房的水槽。");
            }
            break;
        case "key_to_outside"_hash: 
            setDialogStrings(ds, 
            "",
            "",
            "你拿起了大門的鑰匙。",
            "你有預感這東西可以帶你離開。");
            break;
        case "god_pearl"_hash: 
            setDialogStrings(ds, 
            "",
            "",
            "你知道這就是蠱蟲的根源，你拿起了他。",
            "一顆玻璃珠，隱約可以看到中間有什麼東西在蠕動。");
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

string diary_player() {
    string str;
    str.append("2010.10.11\n今天入教剛好滿1個月, 大家為我辦了個派對慶祝\n多虧教主在我最無助的時候找到了我, 向我介紹了霂艮教, 現在的我每天都能感受到一股溫暖和平靜\n霂艮教告訴了我世界的真相, 告訴我那些曾經打倒我的不過是虛幻\n");
    str.append("在派對上我跟傑米相約, 要一起努力成為能服侍教主的高等教徒. 在我們聊天時漢森一直瞪著我們, 他總是這樣, 仗著自己從小在霂亙教長大, 總是瞧不起我們這些新來的.\n");
    str.append("---@--@---");
    str.append("2012.12.12\n");
    str.append("高等教徒的其中一個人調去分部了, 傑米得到了試煉的機會, 通過試煉回來後就可以成為高等教徒了! 好為他開心, 但也很不捨, 試煉不知道多久才會結束, 不知道什麼時候能再見到他. 傑米離開前把他的羊寶寶玩偶送給了我, 說是等他回來再還給他. \n");
    str.append("我會想你的, 傑米\n");
    str.append(" ---@--@---");
    str.append("2012.12.18\n");
    str.append("羊玩偶不見了, 我找了好久好不容易才從餐廳的儲藏室的箱子裡找到, 八成又是漢森那個王八蛋. 漢森一直很糾結在傑米比他早得到資格的事情, 說來也奇怪, 教主那麼愛護漢森, 但漢森卻始終拿不到高級教徒的資格. 但也跟我沒關係啊, 幹嘛沒事欺負我, 個性有夠差!\n");
    str.append("還害我被廚房大叔罵了, 大叔看到我拿著羊玩偶非常的生氣, 還想搶走玩偶, 還好我手快沒被他拿走.\n");
    str.append("真受不了漢森跟大叔, 一個兩個都神經兮兮的.\n");
    str.append("---@--@---");
    str.append("2013.01.01\n");
    str.append("我看到教主在森林拿著好幾個箱子跟一個外來者談話, 我隱約聽到了他們說到傑米, 就偷偷靠過去聽, 我不該聽的, 我還能相信誰…\n");
    str.append("---@--@---");
    str.append("2013.01.10\n");
    str.append("傑米已經走了, 我必須要讓大家知道, 我得撕掉那神棍的面皮!\n");
    str.append("---@--@---");
    str.append("2013.02.27 \n");
    str.append("今天趁教主不在, 溜進他的房間找證據, 搜到一半有高等教徒進來, 差點完蛋. 急忙躲進床下時撞到一塊鏡子碎片, 鏡子滑到外面的時候我還以為我命休矣, 沒想到卻是聽到他的慘叫聲, 被嚇跑了\n");
    str.append("這給了我一點靈感, 霂亙教禁止看鏡子的教義, 也許跟教主的把戲有點關係? 總之先把救我一命的鏡子碎片拿回房間了, 藏在抽屜夾層裡, 也許有什麼用處. \n");
    str.append("---@--@---");
    str.append("2013.03.05\n");
    str.append("有人動了我的房間, 雖然這本日記跟鏡子都沒被發現, 但也是時間問題了. 怎麼幫助大家再從長計議, 這裡不能再待了, 今晚就離開. \n");


    return str;
}