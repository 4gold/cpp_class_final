#include <iostream>
#include "Player.h"
using namespace std;

#ifndef STORY_H
#define STORY_H
class Story
{
private:

int sanityThreshold1;
int erosionThreshold1;

int sanityThreshold2;
int erosionThreshold2;

int sanityThreshold3;
int erosionThreshold3;

public:
    Story(/* args */);
    ~Story();
    int checkEnding(int sp, int ep);
};

Story::Story(/* args */)
{
}

Story::~Story()
{
}

int Story::checkEnding(int sp, int ep)
{

    if(sp < sanityThreshold1 and ep < erosionThreshold1)
    {
        return 4; // 結局4(侵蝕度低)
    }

    return 1; // 結局1(侵蝕度高)
}
#endif