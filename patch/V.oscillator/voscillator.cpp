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
	float currentOutputL = 0;
	float currentOutputR = 0;
	uint16_t mainSmpNb;
	uint16_t smpPerOsc;
	uint8_t currentOsc;

	// Process and filter 1V/Octave input
	mainFreq = (freqctrl.Process() + finectrl.Process());
	mainFreq = powf(2.f, mainFreq) * 55; // get freq from V

	mainSmpNb = sampleRate / (mainFreq);
	smpPerOsc = mainSmpNb / NB_OSC;

	for(uint i=0 ; i<NB_OSC ; i++){
		osc[i].SetFreq(mainFreq);
	}

	// Process audio
	for(size_t i = 0; i < size; i++)
    {
		click++;
		if(click >= mainSmpNb){
			click = click - mainSmpNb;
		}

		// compute switch
		currentOsc = click / smpPerOsc;

		for(uint j=0 ; j<NB_OSC ; j++){
			sig = osc[j].Process();
			if(currentOsc == j){
				currentOutputL = sig * (1.f - osc[j].GetPan());
				currentOutputR = sig * osc[j].GetPan();
			}
		}

		out[0][i] = currentOutputL;
		out[1][i] = currentOutputR;
	}
}