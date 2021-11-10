#include "voscillator.h"

Voscillator::Voscillator(DaisyPatch *patch, float sampleRate):
    m_patch(patch),
    sampleRate(sampleRate)
{
	freqctrl.Init(patch->controls[patch->CTRL_1], 0.f, 5.0f, Parameter::LINEAR);
	finectrl.Init(patch->controls[patch->CTRL_2], 0.f, 0.5f, Parameter::LINEAR);

	for(uint i=0 ; i<NB_OSC ; i++){
		osc[i].Init(sampleRate);
	}

	randomGenerator.Init();
	click = 0;
}

void Voscillator::AudioCallback(AudioHandle::OutputBuffer out, size_t size)
{
	float sig;
	uint32_t currentOsc;

	// Process 1V/Octave input
	mainFreq = (freqctrl.Process() + finectrl.Process());
	mainFreq = powf(2.f, mainFreq) * 55; // get freq from V

	mainSmpNb = sampleRate / mainFreq;
	smpPerOsc = mainSmpNb / NB_OSC;

	// Process audio
	for(size_t i = 0; i < size; i++)
    {
		click++;
		if(click >= mainSmpNb){
			click = 0;
			if(hardSync){
				for(uint j=0 ; j<NB_OSC ; j++){
					osc[j].Reset();
				}
			}
		}

		currentOsc = click / smpPerOsc;

		for(uint j=0 ; j<NB_OSC ; j++){
			osc[j].SetFreq(mainFreq);
			sig = osc[j].Process();
			if(currentOsc == j){
				out[0][i] = sig * (1.f - osc[j].GetPan());
				out[1][i] = sig * osc[j].GetPan();
			}
		}
	}
}