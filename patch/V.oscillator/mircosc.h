#ifndef MICROSC_H
#define MICROSC_H

#include "daisy.h"
#include "daisysp.h"


using namespace daisy;
using namespace daisysp;

class microsc{
    public:
        Oscillator osc;
        float pan = 0.5f; // 0 L / 0.5 C / 1.0R
        float ratio = 1.f; // power of 2
        float fineTune = 0.f;
        float currentFreq = 1.f;

        void Init(float sampleRate){
            osc.Init(sampleRate);
            osc.SetAmp(0.9);
        }

        void Reset(){
            osc.Reset();
        }

        void SetFreq(float freq){
            currentFreq = (freq + fineTune) * ratio;
            osc.SetFreq(currentFreq);
        }

        void SetPan(float pan){
            this->pan = pan;
        }

        float GetPan(){
            return pan;
        }

        void SetRatio(float ratio){
            this->ratio = ratio;
        }

        void SetAmp(float amp){
            osc.SetAmp(amp);
        }

        void SetFineTune(float ft){
            if(ft != 0){
                fineTune = powf(2.f, ft) * 55; // get freq from V;
            }else{
                fineTune = 0;
            }
        }

        float Process(){
            return osc.Process();
        }
};

#endif // !MICROSC_H