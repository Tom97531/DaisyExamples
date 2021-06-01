#ifndef UI_H
#define UI_H

#include <string>

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
                void Init(std::string name, uint8_t yPos){
                    this->name = name;
                    yPosition = yPos;
                    currentFont = &Font_7x10;
                }

                std::string name;
                uint8_t yPosition;
                FontDef* currentFont;

                // use lambda shit
                // https://stackoverflow.com/questions/9186510/how-can-i-store-a-lambda-expression-as-a-field-of-a-class-in-c11#9186537
                void Execute(){

                }
        };
    
        void Display();

        void CreateMenuItem(std::string name);

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