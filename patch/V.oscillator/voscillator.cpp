#include "voscillator.h"
#include "UI.h"

Voscillator::Voscillator(DaisyPatch &patch, float sampleRate):
    m_patch(patch),
    currentMenuItem(0),
    sampleRate(sampleRate)
{
	CreateUI();
	freqctrl.Init(patch.controls[patch.CTRL_1], 0.f, 5.0f, Parameter::LINEAR);
	finectrl.Init(patch.controls[patch.CTRL_2], 0.f, 0.5f, Parameter::LINEAR);

	for(uint i=0 ; i<NB_OSC ; i++){
		osc[i].Init(sampleRate);
	}

	randomGenerator.Init();
	click = 0;
}

void Voscillator::AudioCallback(AudioHandle::OutputBuffer out, size_t size)
{
	float sig, currentOutputL, currentOutputR, nextFreq;
	uint32_t currentCycleSmp = 0;
	uint16_t mainSmpNb = 0;
	uint16_t smpPerOsc = 0;
	uint8_t currentOsc = 0;

	// Read Knobs
	UpdateControl();

	// Process and filter 1V/Octave input
	mainFreq = (freqctrl.Process() + finectrl.Process());
	mainFreq = powf(2.f, mainFreq) * 55; // get freq from V

	mainSmpNb = sampleRate / mainFreq;
	smpPerOsc = mainSmpNb / NB_OSC;

	for(uint i=0 ; i<NB_OSC ; i++){
		osc[i].SetFreq(mainFreq);
	}

	// Process audio
	for(size_t i = 0; i < size; i++)
    {
		click++;

		// compute switch
		currentCycleSmp = click % mainSmpNb;
		currentOsc = currentCycleSmp / smpPerOsc;

		for(uint j=0 ; j<NB_OSC ; j++){
			// TODO better reset technique
			if(currentCycleSmp == 0){
				osc[j].Reset();
			}
			sig = osc[j].Process();
			if(j == currentOsc){
				currentOutputL = sig * (1.f - osc[j].GetPan());
				currentOutputR = sig * osc[j].GetPan();
			}
		}

		out[0][i] = currentOutputL;
		out[1][i] = currentOutputR;
	}
}

void Voscillator::UpdateControl()
{
    m_patch.ProcessAllControls();
	ui->IncrementMenuItem(m_patch.encoder.Increment(), click);

	if(m_patch.encoder.Pressed()){
		ui->EncoderPressed();
	}
}

void Voscillator::UpdateOled()
{
	ui->Display(click);
}

void Voscillator::CreateUI(){
	ui = new UI(std::string("V.oscillator 0.1"), m_patch);

	ui->CreateMenuItem(new Item(std::string("RND FineTune"), [=] {
			for(uint i=0 ; i<1 ; i++){
				osc[i].SetFineTune(randomGenerator.Process() * 0.5f);
			}
		}));
	ui->CreateMenuItem(new ParamItem(std::string("FineTune"), [=] {
			for(uint i=0 ; i<1 ; i++){
				osc[i].SetFineTune(randomGenerator.Process() * 0.5f);
			}
		}, -5., 5.));
	ui->CreateMenuItem(new Item(std::string("RND Pan"), [=] {
			for(uint i=0 ; i<1 ; i++){
				osc[i].SetPan(randomGenerator.Process() * 0.5f);
			}
		}));
	ui->CreateMenuItem(new Item(std::string("RND Amp"), [=] {
			for(uint i=0 ; i<1 ; i++){
				osc[i].SetAmp(randomGenerator.Process() * 0.5f);
			}
		}));
	ui->CreateMenuItem(new Item(std::string("To Bootloader"), [=] {
		// TODO: not reseting for some reason
			daisy::System::ResetToBootloader();
		}));	
	ui->CreateMenuItem(new Item(std::string("turn off OLED"), [=] {
			m_patch.display.Fill(false);
			m_patch.display.Update();
			ui->oledOn = false;
		}));
}

//str2 = "debug:" + std::to_string(static_cast<uint32_t>((sampleRate / mainFreq) / NB_OSC));