#pragma once

class GUI_ItemList;

class GUI_StyleSelector final
{
public:
	GUI_StyleSelector();
	~GUI_StyleSelector();

public:
	void Render();

private:
    std::string current_style = "Dark";

	GUI_ItemList* m_p_item_list = nullptr;
};