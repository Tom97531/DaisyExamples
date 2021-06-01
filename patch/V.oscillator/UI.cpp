#include "UI.h"

UI::UI(std::string title, DaisyPatch& patch){
    m_patch = patch;
    Title = title;
    nbItem = 0;
}

void UI::CreateMenuItem(std::string name, std::function<void()> function){
    itemList[nbItem] = new Item();
    itemList[nbItem]->Init(name, 12 + nbItem*11, function);
    nbItem++;
}


void UI::Display(){
    m_patch.display.Fill(false);

	// Title
	m_patch.display.SetCursor(0, 0);
	std::string str  = Title;
    char*       cstr = &str[0];
    m_patch.display.WriteString(cstr, Font_7x10, true);
	m_patch.display.DrawLine(0,10, m_patch.display.Width(), 10, true);

	// Menu
    for(uint8_t i=0 ; i<nbItem ; i++){
	    m_patch.display.SetCursor(0, itemList[i]->yPosition);
        str = itemList[i]->name;
        m_patch.display.WriteString(cstr, *itemList[i]->currentFont, true);
    }

    m_patch.display.Update();
}

void UI::IncrementMenuItem(int8_t increment){
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
}

void UI::EncoderPressed(){
    itemList[currentItem]->action();
}