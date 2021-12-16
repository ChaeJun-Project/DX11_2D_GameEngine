#include "stdafx.h"
#include "GUI_ItemList.h"

GUI_ItemList::~GUI_ItemList()
{
	ClearItemList();
}

void GUI_ItemList::SetCurrentListID(const std::string& item_str)
{
	for (UINT i = 0; i < static_cast<UINT>(m_item_list_vector.size()); ++i)
	{
		if( m_item_list_vector[i] == item_str)
			m_current_index = static_cast<int>(i);
	}
}
