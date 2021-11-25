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
        Svf filt;

        float sr;

        static const int kNumNoiseMenuItems = 2;
        AbstractMenu::ItemConfig noiseMenuItems[kNumNoiseMenuItems];
        

    public:
        NoiseFx(/* args */);
        ~NoiseFx();

        void Init(float sampleRate);
        float Process(float in, float ctrl);

        FullScreenItemMenu menu;
};

void NoiseFx::Init(float sampleRate){
    sr = sampleRate;
    filt.Init(sr);
}

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
    
    filt.SetFreq(ctrl*(sr/8));
    filt.Process(wn.Process());
    return filt.Low();
}