#ifndef VOSCILLATOR_H
#define VOSCILLATOR_H

#include <string>

#include "daisy.h"
#include "daisysp.h"
#include "daisy_patch.h"

#include "mircosc.h"

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
        float sampleRate;
        float mainFreq, mainSmpNb, smpPerOsc;
        Parameter  freqctrl, finectrl;
        microsc osc[NB_OSC];
        WhiteNoise randomGenerator;

        uint32_t click;

        LoggerImpl<LOGGER_INTERNAL> log;

        void UpdateControl();
};


#endif // VOSCILLATOR_H