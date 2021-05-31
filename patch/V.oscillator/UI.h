#ifndef UI_H
#define UI_H

#include <string>

class UI{
    public:
        class Item{
            public:
                std::string name;
                // use lambda shit
                void Execute(){

                }
        };
    
        uint8_t currentItem;
};

#endif // UI_H