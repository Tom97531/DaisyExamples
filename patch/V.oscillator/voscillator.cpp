#include "voscillator.h"
#include "UI.h"

Voscillator::Voscillator(DaisyPatch &patch, float sampleRate):
    m_patch(patch),
    currentMenuItem(0),
    sampleRate(sampleRate)
{
	freqctrl.Init(patch.controls[patch.CTRL_1], 0.f, 5.0f, Parameter::LINEAR);
	finectrl.Init(patch.controls[patch.CTRL_2], 0.f, 0.5f, Parameter::LINEAR);

	for(uint i=0 ; i<NB_OSC ; i++){
		osc[i].Init(sampleRate);
	}

	randomGenerator.Init();
	click = 0;

	ui = new UI(std::string("V.oscillator 0.01"), m_patch);
	ui->CreateMenuItem(std::string("RND FineTune"));
	ui->CreateMenuItem(std::string("RND Pan"));
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
			// if(currentCycleSmp == 0){
			// 	osc[j].Reset();
			// }
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

	currentMenuItem += m_patch.encoder.Increment();
    if(currentMenuItem < 0){
        currentMenuItem = 0;
    }else if(currentMenuItem > 3){
        currentMenuItem = 3;
    }

	ui->SetCurrentMenuItem(currentMenuItem);
}

// TODO refactor this !
void Voscillator::UpdateOled()
{
	// ui->Display();
    m_patch.display.Fill(false);

	// Title
	m_patch.display.SetCursor(0, 0);
	std::string str  = "V.oscillator 0.01";
    char*       cstr = &str[0];
    m_patch.display.WriteString(cstr, Font_7x10, true);
	m_patch.display.DrawLine(0,10, m_patch.display.Width(), 10, true);

	// Menu 
	m_patch.display.SetCursor(0, 12);
	std::string str0  = "RND FineTune";
    char*       cstr0 = &str0[0];
	FontDef* font0 = &Font_7x10;
	if(currentMenuItem == 0){
		font0 = &Font_7x10_flipped;
		if(m_patch.encoder.Pressed()){
			for(uint i=0 ; i<1 ; i++){
				osc[i].SetFineTune(randomGenerator.Process() * 0.5f);
			}
		}
	}
    m_patch.display.WriteString(cstr0, *font0, true);

	m_patch.display.SetCursor(0, 23);
	std::string str1  = "RND Pan";
    char*       cstr1 = &str1[0];
	FontDef* font1 = &Font_7x10;
	if(currentMenuItem == 1){
		font1 = &Font_7x10_flipped;
		if(m_patch.encoder.Pressed()){
			for(uint i=0 ; i<NB_OSC ; i++){
				osc[i].SetPan(abs(randomGenerator.Process()));
			}
		}
	}
    m_patch.display.WriteString(cstr1, *font1, true);

	m_patch.display.SetCursor(0, 34);
	std::string str2;
	// str2 = "Freq: " ;
	str2 = "debug:" + std::to_string(static_cast<uint32_t>((sampleRate / mainFreq) / NB_OSC));
    char*       cstr2 = &str2[0];
	FontDef* font2 = &Font_7x10;
	if(currentMenuItem == 2){
		font2 = &Font_7x10_flipped;
		if(m_patch.encoder.Pressed()){
			osc[0].SetAmp(0);
		}
	}
    m_patch.display.WriteString(cstr2, *font2, true);
	
	m_patch.display.SetCursor(0, 45);
	std::string str3  = "To bootloader";
    char*       cstr3 = &str3[0];
	FontDef *font3 = &Font_7x10;
	if(currentMenuItem == 3){
		font3 = &Font_7x10_flipped;
		if(m_patch.encoder.Pressed()){
			daisy::System::ResetToBootloader();
		}
	}
	m_patch.display.WriteString(cstr3, *font3, true);

    m_patch.display.Update();
}