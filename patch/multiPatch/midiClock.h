#pragma once
#include "daisy_patch.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

class midiClock{
    public:
        void Process(){
            hw->midi.Listen();
            while(hw->midi.HasEvents())
            {
                HandleMidiMessage(hw->midi.PopEvent());
            }
        }

        void Init(DaisyPatch *patch){
            hw = patch;
            hw->midi.StartReceive();
            timingClock = 0;
        }

    private:
        int timingClock;
        DaisyPatch *hw;

        void HandleMidiMessage(MidiEvent m)
        {
            switch(m.type)
            {
                case SystemRealTime:
                    if(m.srt_type == TimingClock){
                        timingClock++;
                        if(timingClock == 24){
                            dsy_gpio_write(&hw->gate_output, true);
                            timingClock = 0;
                        }else{
                            dsy_gpio_write(&hw->gate_output, false);
                        }
                    }
                break;
                default: break;
            }
        }
};