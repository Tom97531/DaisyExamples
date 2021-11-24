#pragma once
#include "daisy.h"
#include "daisysp.h"

#include "Processor.h"

const char* wavefoldType[] = {"sine", "exp"};
MappedStringListValue wavefoldTypeValue(wavefoldType, 2, 0);

class wavefold : public Processor
{
private:
    static const int                kNumWavefoldMenuItems = 2;
    AbstractMenu::ItemConfig wavefoldMenuItems[kNumWavefoldMenuItems];
public:
    wavefold(/* args */);
    ~wavefold();
    float Process(float in, float ctrl);

};

wavefold::wavefold(/* args */)
{
    // ====================================================================
    // Wavefold menu
    // ====================================================================
	wavefoldMenuItems[0].type = daisy::AbstractMenu::ItemType::valueItem;
	wavefoldMenuItems[0].text = "fold type";
	wavefoldMenuItems[0].asMappedValueItem.valueToModify = &wavefoldTypeValue;

    wavefoldMenuItems[kNumWavefoldMenuItems-1].type = daisy::AbstractMenu::ItemType::closeMenuItem;
    wavefoldMenuItems[kNumWavefoldMenuItems-1].text = "Back";

	menu.Init(wavefoldMenuItems, kNumWavefoldMenuItems);
}

wavefold::~wavefold()
{
}

float wavefold::Process(float in, float ctrl){
    // TODO: add gain from CTRL input
    return sinf(100 * ctrl * in);
}

