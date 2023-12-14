#include "Event.h"

using namespace std;

int Event::progressing = -1;

Event::Event()
{
}
Event::Event(vector<string> line, int l)
{
    this->lines = line;
    this->len = l;
}

Event::~Event()
{
}

void Event::readLines()
{
    int l = this->len;

    for (int i = 0; i < len; i++)
    {
        // 輸出台詞
        FuncPool::delayCout(this->lines[i], true);
    }
    
}

bool Event::checkActive()
{
    if (active == true)
    {
        return true;
    }
    return false;
}

void Event::turnoff()
{
    this->active = false;
}

void Event::start(int e) // e為新的progressing
{   
    this->progressing = e;
    this->progress = true;
    this->readLines();
}

void Event::end(int i, Player& p, int s)
{
    this->progress = false;
    this->turnoff();
    this->presentMap = p.getMap();
    presentMap.updateConnection(i);
    p.updateMap(presentMap);
    this->progressing = s;
}

bool Event::isprogressing()
{
    return progress;
}



