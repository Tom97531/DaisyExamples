#pragma once 
#include "daisy.h"
#include "daisysp.h"


using namespace daisy;
using namespace daisysp;

class Processor
{
    public:
        FullScreenItemMenu menu;
        virtual float Process(float in, float ctrl) = 0;
};

