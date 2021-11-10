#ifndef VOSCILLATOR_H
#define VOSCILLATOR_H

#include <string>

#include "daisy.h"
#include "daisysp.h"
#include "daisy_patch.h"

#include "mircosc.h"

#define PATCH_NAME "V.oscillator 0.01"
#define NB_OSC 8

using namespace daisy;
using namespace daisysp;

class Voscillator{
    public:
        Voscillator(DaisyPatch *patch, float sampleRate);
        void AudioCallback(AudioHandle::OutputBuffer out, size_t size);

        microsc osc[NB_OSC];

        void setHardSync(bool on){hardSync = on;};
        bool getHardync(){return hardSync;};
        
    private:
        DaisyPatch *m_patch;
        
        float sampleRate;
        uint32_t click;
        float mainFreq, mainSmpNb, smpPerOsc;
        bool hardSync = false;
        Parameter  freqctrl, finectrl;
        
        WhiteNoise randomGenerator;
};


#endif // VOSCILLATOR_H