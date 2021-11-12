#include "voscillator.h"

Voscillator::Voscillator(float sampleRate):
    sampleRate(sampleRate)
{
	adsrEnv.Init(sampleRate);

	for(uint i=0 ; i<NB_OSC ; i++){
		osc[i].Init(sampleRate);
	}

	randomGenerator.Init();
	click = 0;
}

void Voscillator::AudioCallback(AudioHandle::OutputBuffer out, size_t size, performaceState state)
{
	float sig;
	uint32_t currentOsc;

	// Process 1V/Octave input
	mainFreq = (state.pitch + state.finePitch);
	mainFreq = powf(2.f, mainFreq) * 55; // get freq from V

	mainSmpNb = sampleRate / mainFreq;
	smpPerOsc = mainSmpNb / NB_OSC;

	// Process audio
	for(size_t i = 0; i < size; i++)
    {
		click++;
		if(click >= mainSmpNb){
			click = 0;
			if(state.hardSync){
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

		if(state.envType == ADSR){
			adsrEnv.SetAttackTime(state.attack);
			adsrEnv.SetDecayTime(state.decay);
			adsrEnv.SetSustainLevel(state.sustain);
			adsrEnv.SetReleaseTime(state.release);
			out[0][i] = out[0][i] * adsrEnv.Process(state.gate);
			out[1][i] = out[1][i] * adsrEnv.Process(state.gate);
		}else if(state.envType == AD){
			adsrEnv.SetAttackTime(state.attack);
			adsrEnv.SetDecayTime(state.decay);
			adsrEnv.SetSustainLevel(0);
			adsrEnv.SetReleaseTime(state.decay);
			out[0][i] = out[0][i] * adsrEnv.Process(state.gate);
			out[1][i] = out[1][i] * adsrEnv.Process(state.gate);
		}// else don't use envelope generator
	}
}