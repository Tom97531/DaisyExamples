#include "UI.h"

UI::UI(std::string title, DaisyPatch& patch){
    m_patch = patch;
    Title = title;
    nbItem = 0;
}

void UI::CreateMenuItem(std::string name){
    itemList[nbItem] = new Item();
    itemList[nbItem]->Init(name, 12 + nbItem*11);
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

	// // Menu
    for(uint8_t i=0 ; i<nbItem ; i++){
	    m_patch.display.SetCursor(0, itemList[i]->yPosition);
        str = itemList[i]->name[0];
        m_patch.display.WriteString(cstr, *itemList[i]->currentFont, true);
    }

    m_patch.display.Update();
}

void UI::SetCurrentMenuItem(uint8_t item){
    itemList[currentItem]->currentFont = &Font_7x10;

    currentItem = item;
    itemList[item]->currentFont = &Font_7x10_flipped;
}