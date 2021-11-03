#ifndef UIvosc_H
#define UIvosc_H

#include <string>
#include <functional>

#include "daisy.h"
#include "daisy_patch.h"
#include "daisysp.h"

#include "Item.h"

#define ITEM_WINDOW_SIZE 4
#define TURN_OFF_OLED_DELAY 2

using namespace daisy;
using namespace daisysp;

class UIvosc{
    public:
        UIvosc(std::string title, DaisyPatch* patch);

        void Display(uint32_t click);

        void CreateMenuItem(Item* menuItem);

        void IncrementMenuItem(int8_t item, uint32_t click);
        void EncoderPressed();

        bool oledOn;

    private:
        int8_t currentItem;
        uint8_t nbItem;
        DaisyPatch *m_patch;
        std::string Title;
        Item* itemList[15];
        uint32_t turnOffOledDelay;
        uint32_t lastUpdate;
};

#endif // UI_H