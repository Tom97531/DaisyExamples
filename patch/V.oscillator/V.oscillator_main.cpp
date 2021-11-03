#include "daisy_patch.h"
#include "daisysp.h"

#include "voscillator.h"
#include "UiHardware.h"

using namespace daisy;
using namespace daisysp;

DaisyPatch patch;
Voscillator *Vosc;

daisy::UI ui;

FullScreenItemMenu mainMenu;
FullScreenItemMenu VolumeMenu;
FullScreenItemMenu RatioMenu;
FullScreenItemMenu FineTuneMenu;
FullScreenItemMenu PanMenu;
UiEventQueue       eventQueue;

const int                kNumMainMenuItems = 4;
AbstractMenu::ItemConfig mainMenuItems[kNumMainMenuItems];
const int                kNumVolumeMenuItems = NB_OSC + 1;
AbstractMenu::ItemConfig VolumeMenuItems[kNumVolumeMenuItems];
const int                kNumRatioMenuItems = NB_OSC + 1;
AbstractMenu::ItemConfig RatioMenuItems[kNumRatioMenuItems];
const int                kNumFineTuneMenuItems = NB_OSC + 1;
AbstractMenu::ItemConfig FineTuneMenuItems[kNumFineTuneMenuItems];
const int                kNumPanMenuItems = NB_OSC + 1;
AbstractMenu::ItemConfig PanMenuItems[kNumPanMenuItems];

const char* oscStr[] = {"Osc 1", "Osc 2", "Osc 3", "Osc 4", "Osc 5", "Osc 6", "Osc 7", "Osc 8"};
const char* ratioStr[] = {"0.25", "0.5", "1", "2", "4", "8"};

MappedFloatValue *volumeValue[NB_OSC];
MappedStringListValue *ratioValue[NB_OSC];
MappedFloatValue *fineTuneValue[NB_OSC];
MappedFloatValue *panValue[NB_OSC];

void InitUi()
{
	for(int i=0 ; i<NB_OSC ;i++){
		volumeValue[i] = new MappedFloatValue(0.0, 1.0, 0.9);
		ratioValue[i] = new MappedStringListValue(ratioStr, 6, 2);
		fineTuneValue[i] = new MappedFloatValue(-2.0, 2.0, 0.0);
		panValue[i] = new MappedFloatValue(0.0, 1.0, 0.5);
	}

	UI::SpecialControlIds specialControlIds;
    specialControlIds.okBttnId
        = bttnEncoder; // Encoder button is our okay button
    specialControlIds.menuEncoderId
        = encoderMain; // Encoder is used as the main menu navigation encoder

    // This is the canvas for the OLED display.
    UiCanvasDescriptor oledDisplayDescriptor;
    oledDisplayDescriptor.id_     = canvasOledDisplay; // the unique ID
    oledDisplayDescriptor.handle_ = &patch.display; // a pointer to the display
    oledDisplayDescriptor.updateRateMs_  = 50;   // 50ms == 20Hz
    oledDisplayDescriptor.screenSaverTimeOut = 30000;
    oledDisplayDescriptor.clearFunction_ = &ClearCanvas;
    oledDisplayDescriptor.flushFunction_ = &FlushCanvas;

    ui.Init(eventQueue,
            specialControlIds,
            {oledDisplayDescriptor},
            canvasOledDisplay);
}

void InitUiPages()
{
    // ====================================================================
    // The main menu
    // ====================================================================

    mainMenuItems[0].type = daisy::AbstractMenu::ItemType::openUiPageItem;
    mainMenuItems[0].text = "Volume";
    mainMenuItems[0].asOpenUiPageItem.pageToOpen = &VolumeMenu;

    mainMenuItems[1].type = daisy::AbstractMenu::ItemType::openUiPageItem;
    mainMenuItems[1].text = "Ratio";
    mainMenuItems[1].asOpenUiPageItem.pageToOpen = &RatioMenu;

    mainMenuItems[2].type = daisy::AbstractMenu::ItemType::openUiPageItem;
    mainMenuItems[2].text = "Fine Tune";
    mainMenuItems[2].asOpenUiPageItem.pageToOpen = &FineTuneMenu;

    mainMenuItems[3].type = daisy::AbstractMenu::ItemType::openUiPageItem;
    mainMenuItems[3].text = "Pan";
    mainMenuItems[3].asOpenUiPageItem.pageToOpen = &PanMenu;

    mainMenu.Init(mainMenuItems, kNumMainMenuItems);

    // ====================================================================
    // The "Volume" menu
    // ====================================================================

	for(int i=0 ; i<NB_OSC ; i++){
		VolumeMenuItems[i].type = daisy::AbstractMenu::ItemType::valueItem;
		VolumeMenuItems[i].text = oscStr[i];
		VolumeMenuItems[i].asMappedValueItem.valueToModify
			= volumeValue[i];
	}
    VolumeMenuItems[NB_OSC].type = daisy::AbstractMenu::ItemType::closeMenuItem;
    VolumeMenuItems[NB_OSC].text = "Back";

    VolumeMenu.Init(VolumeMenuItems, kNumVolumeMenuItems);

    // ====================================================================
    // The "Ratio" menu
    // ====================================================================

	for(int i=0 ; i<NB_OSC ; i++){
		RatioMenuItems[i].type = daisy::AbstractMenu::ItemType::valueItem;
		RatioMenuItems[i].text = oscStr[i];
		RatioMenuItems[i].asMappedValueItem.valueToModify
			= ratioValue[i];
	}
    RatioMenuItems[NB_OSC].type = daisy::AbstractMenu::ItemType::closeMenuItem;
    RatioMenuItems[NB_OSC].text = "Back";

    RatioMenu.Init(RatioMenuItems, kNumRatioMenuItems);

    // ====================================================================
    // The "FineTune" menu
    // ====================================================================

	for(int i=0 ; i<NB_OSC ; i++){
		FineTuneMenuItems[i].type = daisy::AbstractMenu::ItemType::valueItem;
		FineTuneMenuItems[i].text = oscStr[i];
		FineTuneMenuItems[i].asMappedValueItem.valueToModify
			= fineTuneValue[i];
	}
    FineTuneMenuItems[NB_OSC].type = daisy::AbstractMenu::ItemType::closeMenuItem;
    FineTuneMenuItems[NB_OSC].text = "Back";

    FineTuneMenu.Init(FineTuneMenuItems, kNumFineTuneMenuItems);

    // ====================================================================
    // The "Pan" menu
    // ====================================================================

	for(int i=0 ; i<NB_OSC ; i++){
		PanMenuItems[i].type = daisy::AbstractMenu::ItemType::valueItem;
		PanMenuItems[i].text = oscStr[i];
		PanMenuItems[i].asMappedValueItem.valueToModify
			= panValue[i];
	}
    PanMenuItems[NB_OSC].type = daisy::AbstractMenu::ItemType::closeMenuItem;
    PanMenuItems[NB_OSC].text = "Back";

    PanMenu.Init(PanMenuItems, kNumPanMenuItems);

}

void GenerateUiEvents()
{
    if(patch.encoder.RisingEdge())
        eventQueue.AddButtonPressed(bttnEncoder, 1);

    if(patch.encoder.FallingEdge())
        eventQueue.AddButtonReleased(bttnEncoder);

    const auto increments = patch.encoder.Increment();
    if(increments != 0)
        eventQueue.AddEncoderTurned(encoderMain, increments, 12);
}

static void AudioCallback(AudioHandle::InputBuffer  in,
                          AudioHandle::OutputBuffer out,
                          size_t                    size)
{
	patch.ProcessAllControls();
	GenerateUiEvents();

	// set osc
	for(int i=0 ; i<NB_OSC ; i++){
		Vosc->osc[i].SetAmp(volumeValue[i]->Get());
		if(ratioValue[i]->GetIndex() == 0){
			Vosc->osc[i].SetRatio(0.25);
		}else if(ratioValue[i]->GetIndex() == 1){
			Vosc->osc[i].SetRatio(0.5);
		}else if(ratioValue[i]->GetIndex() == 2){
			Vosc->osc[i].SetRatio(1.0);
		}else if(ratioValue[i]->GetIndex() == 3){
			Vosc->osc[i].SetRatio(2.0);
		}else if(ratioValue[i]->GetIndex() == 4){
			Vosc->osc[i].SetRatio(4.0);
		}else if(ratioValue[i]->GetIndex() == 5){
			Vosc->osc[i].SetRatio(8.0);
		}
		Vosc->osc[i].SetFineTune(fineTuneValue[i]->Get());
		Vosc->osc[i].SetPan(panValue[i]->Get());
	}

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

    InitUi();
    InitUiPages();
	ui.OpenPage(mainMenu);

	Vosc = new Voscillator(&patch, sampleRate);

	patch.StartAdc();
	patch.StartAudio(AudioCallback);
	
	while(1) {
		ui.Process();
	}
}


