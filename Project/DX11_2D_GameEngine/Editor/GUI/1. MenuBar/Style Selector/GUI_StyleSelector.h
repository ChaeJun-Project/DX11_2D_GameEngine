#pragma once

#include "GUI/IGUI_Popup.h"

class GUI_ItemList;

class GUI_StyleSelector final : public IGUI_Popup
{
public:
	GUI_StyleSelector();
	virtual ~GUI_StyleSelector();

public:
	virtual void Render() override;

private:
    std::string current_style = "Dark";

	GUI_ItemList* m_p_item_list = nullptr;

	friend class GUI_MenuBar;
};