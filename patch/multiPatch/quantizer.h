#pragma once

#include "daisy.h"
#include "daisysp.h"

#include "Processor.h"

using namespace daisy;
using namespace daisysp;

class quantizer : public Processor
{
private:
    DaisyPatch *hw;

public:
    quantizer(/* args */);
    ~quantizer();
    float Process(float in, float ctrl);

    void Init(DaisyPatch *patch){
        hw = patch;
    }

};

float quantizer::Process(float in, float ctrl){
    uint note = ctrl * 48;
    hw->seed.dac.WriteValue(DacHandle::Channel::ONE, note * 1.f / 48.f * 4095);
    // cv out 2 for harmony
    return 0;
}

quantizer::quantizer(/* args */)
{
}

quantizer::~quantizer()
{
}
