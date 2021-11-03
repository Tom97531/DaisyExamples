#include "UIvosc.h"

UIvosc::UIvosc(std::string title, DaisyPatch* patch){
    m_patch = patch;
    Title = title;
    nbItem = 0;
    currentItem = 0;
    oledOn = true;
    // delay in audio clock count
    turnOffOledDelay = TURN_OFF_OLED_DELAY * m_patch->AudioSampleRate();
}

void UIvosc::CreateMenuItem(Item* menuItem){
    itemList[nbItem] = menuItem;
    nbItem++;
}

void UIvosc::Display(uint32_t click){
    uint8_t firstItem = 0;
    m_patch->display.Fill(false);

    if(!oledOn){
        m_patch->display.Update();
        return;
    }

    if(click - lastUpdate < turnOffOledDelay){
        // Title
        m_patch->display.SetCursor(0, 0);
        std::string str  = Title;
        char*       cstr = &str[0];
        m_patch->display.WriteString(cstr, Font_7x10, true);
        m_patch->display.DrawLine(0,10, m_patch->display.Width(), 10, true);

        // Menu
        if(currentItem >= ITEM_WINDOW_SIZE){
            firstItem = currentItem - ITEM_WINDOW_SIZE + 1;
        }

        // display items
        for(uint8_t i=firstItem ; i<ITEM_WINDOW_SIZE+firstItem && i<nbItem ; i++){
            m_patch->display.SetCursor(0, 12 + (i-firstItem)*10);
            str = itemList[i]->name;
            m_patch->display.WriteString(cstr, *itemList[i]->currentFont, true);
        }
    }

    m_patch->display.Update();
}

void UIvosc::IncrementMenuItem(int8_t increment, uint32_t click){
    // reset font
    itemList[currentItem]->currentFont = &Font_7x10;

    //update current item
    currentItem += increment;
    if(currentItem < 0){
        currentItem = 0;
    }else if(currentItem > nbItem - 1){
        currentItem = nbItem - 1;
    }

    // set flipped font
    itemList[currentItem]->currentFont = &Font_7x10_flipped;

    lastUpdate = click;
}

void UIvosc::EncoderPressed(){
    itemList[currentItem]->action();
}