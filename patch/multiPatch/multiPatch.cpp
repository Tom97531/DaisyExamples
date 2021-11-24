#include "daisy_patch.h"
#include "daisysp.h"
#include "UiHardware.h"

#include "NoiseFx.h"
#include "wavefold.h"

using namespace daisy;
using namespace daisysp;

DaisyPatch hw;
Parameter CTRL1Parameter, CTRL2Parameter, CTRL3Parameter, CTRL4Parameter;
Processor *CTRL12Processor, *CTRL3Processor, *CTRL4Processor;

NoiseFx noiseProcessor;
wavefold wavefoldProcessor;

daisy::UI ui;

FullScreenItemMenu mainMenu;
FullScreenItemMenu CTRLl2Menu;
FullScreenItemMenu CTRL3Menu;
FullScreenItemMenu CTRL4Menu;
UiEventQueue       eventQueue;

// general menu
const int                kNumMainMenuItems = 3;
AbstractMenu::ItemConfig mainMenuItems[kNumMainMenuItems];
const int                kNumCTRL12MenuItems = 3;
AbstractMenu::ItemConfig CTRL12MenuItems[kNumCTRL12MenuItems];
const int                kNumCTRL3MenuItems = 3;
AbstractMenu::ItemConfig CTRL3MenuItems[kNumCTRL3MenuItems];
const int                kNumCTRL4MenuItems = 3;
AbstractMenu::ItemConfig CTRL4MenuItems[kNumCTRL4MenuItems];

// fx menu
const int                kNumFxMenuItems = 3;
AbstractMenu::ItemConfig fxMenuItems[kNumFxMenuItems];

const char* fxType[] = {"Noise", "Wavefolder", "Bit Crush", "Quantizer"};

MappedStringListValue fx3TypeValue(fxType, 4, 0);

void InitUi()
{
	UI::SpecialControlIds specialControlIds;
    specialControlIds.okBttnId
        = bttnEncoder; // Encoder button is our okay button
    specialControlIds.menuEncoderId
        = encoderMain; // Encoder is used as the main menu navigation encoder

    // This is the canvas for the OLED display.
    UiCanvasDescriptor oledDisplayDescriptor;
    oledDisplayDescriptor.id_     = canvasOledDisplay; // the unique ID
    oledDisplayDescriptor.handle_ = &hw.display; // a pointer to the display
    oledDisplayDescriptor.updateRateMs_  = 50;   // 50ms == 20Hz
    oledDisplayDescriptor.screenSaverTimeOut = 60000;
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
    mainMenuItems[0].text = "CTRL 1+2";
    mainMenuItems[0].asOpenUiPageItem.pageToOpen = &CTRLl2Menu;

    mainMenuItems[1].type = daisy::AbstractMenu::ItemType::openUiPageItem;
    mainMenuItems[1].text = "CTRL 3";
    mainMenuItems[1].asOpenUiPageItem.pageToOpen = &CTRL3Menu;

    mainMenuItems[2].type = daisy::AbstractMenu::ItemType::openUiPageItem;
    mainMenuItems[2].text = "CTRL 4";
    mainMenuItems[2].asOpenUiPageItem.pageToOpen = &CTRL4Menu;

    mainMenu.Init(mainMenuItems, kNumMainMenuItems);

    // ====================================================================
    // CTRL 3 menu
    // ====================================================================
	CTRL3MenuItems[0].type = daisy::AbstractMenu::ItemType::valueItem;
	CTRL3MenuItems[0].text = "fx type";
	CTRL3MenuItems[0].asMappedValueItem.valueToModify = &fx3TypeValue;

    CTRL3MenuItems[1].type = daisy::AbstractMenu::ItemType::openUiPageItem;
    CTRL3MenuItems[1].text = "fx menu";
    CTRL3MenuItems[1].asOpenUiPageItem.pageToOpen = &noiseProcessor.menu;

    CTRL3MenuItems[2].type = daisy::AbstractMenu::ItemType::closeMenuItem;
    CTRL3MenuItems[2].text = "Back";

	CTRL3Menu.Init(CTRL3MenuItems, kNumCTRL3MenuItems);
}

void GenerateUiEvents()
{
    if(hw.encoder.RisingEdge())
        eventQueue.AddButtonPressed(bttnEncoder, 1);

    if(hw.encoder.FallingEdge())
        eventQueue.AddButtonReleased(bttnEncoder);

    const auto increments = hw.encoder.Increment();
    if(increments != 0)
        eventQueue.AddEncoderTurned(encoderMain, increments, 12);
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	hw.ProcessAllControls();
	GenerateUiEvents();

    if(fx3TypeValue.GetIndex() == 0){
        CTRL3MenuItems[1].asOpenUiPageItem.pageToOpen = &noiseProcessor.menu;
        CTRL3Processor = &noiseProcessor;
    }else if(fx3TypeValue.GetIndex() == 1){
        CTRL3MenuItems[1].asOpenUiPageItem.pageToOpen = &wavefoldProcessor.menu;
        CTRL3Processor = &wavefoldProcessor;
    }

	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = in[0][i];
		out[1][i] = in[1][i];
		out[2][i] = CTRL3Processor->Process(in[2][i], CTRL3Parameter.Process());
		out[3][i] = in[3][i];
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_96KHZ);
	float sampleRate = hw.AudioSampleRate();\

    CTRL1Parameter.Init(hw.controls[hw.CTRL_1], 0.f, 1.0f, Parameter::LINEAR);
    CTRL2Parameter.Init(hw.controls[hw.CTRL_2], 0.f, 1.0f, Parameter::LINEAR);
    CTRL3Parameter.Init(hw.controls[hw.CTRL_3], 0.f, 1.0f, Parameter::LINEAR);
    CTRL4Parameter.Init(hw.controls[hw.CTRL_4], 0.f, 1.0f, Parameter::LINEAR);

    InitUi();
    InitUiPages();
	ui.OpenPage(mainMenu);

	hw.StartAdc();
	hw.StartAudio(AudioCallback);

	while(1) {
		ui.Process();
	}
}
