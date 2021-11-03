#include "daisy_patch.h"
#include "daisysp.h"

#include "voscillator.h"

using namespace daisy;
using namespace daisysp;

DaisyPatch patch;
Voscillator *Vosc;

static void AudioCallback(AudioHandle::InputBuffer  in,
                          AudioHandle::OutputBuffer out,
                          size_t                    size)
{
	Vosc->AudioCallback(out, size);

	for (size_t i = 0; i < size; i++)
	{
		out[2][i] = in[2][i];
		out[3][i] = in[3][i];
	}
}

int main(void)
{
	patch.Init();
	patch.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_96KHZ);
	float sampleRate = patch.AudioSampleRate();

	Vosc = new Voscillator(&patch, sampleRate);

	patch.StartAdc();
	patch.StartAudio(AudioCallback);
	
	while(1) {
		Vosc->UpdateOled();
	}
}


