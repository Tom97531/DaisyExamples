#pragma once

#include "daisy.h"
#include "daisysp.h"

#include "Processor.h"

using namespace daisy;
using namespace daisysp;

const char* noiseType[] = {"White", "brown", "pink"};
MappedStringListValue noiseTypeValue(noiseType, 3, 0);

class NoiseFx : public Processor
{
    private:
        WhiteNoise wn;
        
        static const int kNumNoiseMenuItems = 2;
        AbstractMenu::ItemConfig noiseMenuItems[kNumNoiseMenuItems];
        

    public:
        NoiseFx(/* args */);
        ~NoiseFx();
        float Process(float in, float ctrl);

        FullScreenItemMenu menu;
};

NoiseFx::NoiseFx(/* args */)
{
    wn.Init();

    // ====================================================================
    // Noise menu
    // ====================================================================
	noiseMenuItems[0].type = daisy::AbstractMenu::ItemType::valueItem;
	noiseMenuItems[0].text = "noise type";
	noiseMenuItems[0].asMappedValueItem.valueToModify = &noiseTypeValue;

    noiseMenuItems[kNumNoiseMenuItems-1].type = daisy::AbstractMenu::ItemType::closeMenuItem;
    noiseMenuItems[kNumNoiseMenuItems-1].text = "Back";

	menu.Init(noiseMenuItems, kNumNoiseMenuItems);
}

NoiseFx::~NoiseFx()
{
}

float NoiseFx::Process(float in, float ctrl){
    return wn.Process();
}