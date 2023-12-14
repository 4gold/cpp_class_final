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


#endif