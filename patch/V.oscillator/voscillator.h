#ifndef VOSCILLATOR_H
#define VOSCILLATOR_H

#include <string>

#include "daisy.h"
#include "daisysp.h"
#include "daisy_patch.h"

#include "mircosc.h"
#include "UI.h"

#define PATCH_NAME "V.oscillator 0.01"
#define NB_OSC 8

using namespace daisy;
using namespace daisysp;

class Voscillator{
    public:
        int8_t currentMenuItem;

        Voscillator(DaisyPatch &patch, float sampleRate);
        void AudioCallback(AudioHandle::OutputBuffer out, size_t size);
        void UpdateOled();

    private:
        DaisyPatch &m_patch;

        microsc osc[NB_OSC];
        
        float sampleRate;
        uint32_t click;
        float mainFreq, mainSmpNb, smpPerOsc;
        Parameter  freqctrl, finectrl;
        
        WhiteNoise randomGenerator;

        UI* ui;

        void UpdateControl();

        void CreateUI();
};


#endif // VOSCILLATOR_H