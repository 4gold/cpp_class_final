#include <iostream>
#include "Player.h"
using namespace std;

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
        return 1; // 結局1
    }

    else if (sp < sanityThreshold2 and ep < erosionThreshold2)
    {
        return 2; // 結局2
    }

    return 3;   // 結局3
}