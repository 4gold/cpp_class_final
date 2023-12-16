#include<iostream>
#include<string>
#include<cstdlib>
#include<time.h>
#include <fstream>

#include <chrono>
#include <thread>

#ifndef FUNCPOOL_H
#define FUNCPOOL_H

using namespace std;

const int TEXT_INTERVAL_MILSEC = 500;

/**
 * 共用函式集合
*/
class FuncPool {
    public:
        /*
        cout後會短暫暫停，間隔時間請用TEXT_INTERVAL_MILSEC設定。
        */
        template<typename T>
        static void delayCout(T t, bool changeLine){
            cout << t << (changeLine ? "\n" : "");
            //std::this_thread::sleep_for(std::chrono::milliseconds(TEXT_INTERVAL_MILSEC));
        };

        template<typename T>
        static void delayCout(T t){
            delayCout(t, true);
        };

        static void readAndPrintFile(string fileName) {
            std::ifstream filein(fileName);
            for (std::string line; std::getline(filein, line); ) 
            {
                std::cout << line << std::endl;
            }
            filein.close();
        }

        const static std::vector<std::string> split(const std::string& str, const std::string& pattern) {
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

};

#endif