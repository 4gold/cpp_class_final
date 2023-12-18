#include <iostream>
#include "story.h"
using namespace std;

Story::Story(/* args */)
{
}

Story::~Story()
{
}

int Story::checkEnding(int sp, int ep)
{

    if(sp > sanityThreshold1 and ep < erosionThreshold1)
    {
        return 4; // 結局4(侵蝕度低)
    }

    return 1; // 結局1(侵蝕度高)
}
