#ifndef UI_H
#define UI_H

#include <string>
#include <functional>

#include "daisy.h"
#include "daisy_patch.h"
#include "daisysp.h"

#include "Item.h"

#define ITEM_WINDOW_SIZE 4

using namespace daisy;
using namespace daisysp;

class UI{
    public:
        UI(std::string title, DaisyPatch& patch);

        void Display();

        void CreateMenuItem(Item* menuItem);

        void IncrementMenuItem(int8_t item);
        void EncoderPressed();

    private:
        int8_t currentItem;
        uint8_t nbItem;
        DaisyPatch m_patch;
        std::string Title;
        Item* itemList[15];

        
};

#endif // UI_H