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
        enum envelopeType{OFF=0, AD, ADSR};
        class performaceState{
            public:
                float pitch;
                float finePitch;

                bool hardSync;

                envelopeType envType;
                bool gate;
                float attack;
                float decay;
                float sustain;
                float release;
        };

        Voscillator(float sampleRate);
        void AudioCallback(AudioHandle::OutputBuffer out, size_t size, performaceState state);

        microsc osc[NB_OSC];

    private:
        float sampleRate;
        uint32_t click;
        float mainFreq, mainSmpNb, smpPerOsc;
        
        WhiteNoise randomGenerator;

        // enveloppe management
        daisysp::Adsr adsrEnv;
};


#endif // VOSCILLATOR_H