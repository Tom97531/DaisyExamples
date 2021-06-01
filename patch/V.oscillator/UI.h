#ifndef UI_H
#define UI_H

#include <string>
#include <functional>

#include "daisy.h"
#include "daisy_patch.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

class UI{
    public:
        UI(std::string title, DaisyPatch& patch);

        class Item{
            public:
                void Init(std::string name, uint8_t yPos, std::function<void()> function){
                    this->name = name;
                    yPosition = yPos;
                    currentFont = &Font_7x10;
                    action = function;
                }

                std::string name;
                uint8_t yPosition;
                FontDef* currentFont;

                std::function<void()> action;
        };
    
        void Display();

        void CreateMenuItem(std::string name, std::function<void()> function);

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