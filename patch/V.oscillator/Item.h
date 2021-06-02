#ifndef ITEM_H

#include <string>
#include <functional>

#include "daisy.h"
#include "daisy_patch.h"
#include "daisysp.h"

class Item{
    public:
        Item(std::string name, std::function<void()> function){
            this->name = name;
            currentFont = &Font_7x10;
            action = function;
        }

        std::string name;
        uint8_t yPosition;
        FontDef* currentFont;

        std::function<void()> action;

        void SetCurrentYPos(uint8_t ypos){
            yPosition = ypos;
        }
};

class ParamItem: public Item {
    public:
        ParamItem(std::string name, std::function<void()> function, 
                  float min, float max):
                    Item(name, function),
                    min(min),
                    max(max),
                    currentValue(0)
        {}

        float min, max;
        float currentValue;
};

#endif // !ITEM_H