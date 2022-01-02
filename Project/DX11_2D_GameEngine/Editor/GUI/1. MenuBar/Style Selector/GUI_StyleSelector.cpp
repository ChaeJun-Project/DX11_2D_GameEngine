#include "stdafx.h"
#include "GUI_StyleSelector.h"

#include "GUI/Module/ItemList/GUI_ItemList.h"

GUI_StyleSelector::GUI_StyleSelector()
{
	m_p_item_list = new GUI_ItemList();
}

GUI_StyleSelector::~GUI_StyleSelector()
{
	SAFE_DELETE(m_p_item_list);
}

void GUI_StyleSelector::Render()
{
	if (ImGui::Begin("Style Selector", &m_is_active, ImGuiWindowFlags_NoResize))
	{
		ImGui::Text("Style");
		ImGui::SameLine();

		ImGui::Spacing();
		ImGui::SameLine();

		m_p_item_list->AddItem("Dark");
		m_p_item_list->AddItem("Classic");
		m_p_item_list->AddItem("Light");

		const auto& item_list_vector = m_p_item_list->GetItemList();

		for (UINT i = 0; i < static_cast<UINT>(item_list_vector.size()); ++i)
		{
			if(item_list_vector[i] == current_style)
				m_p_item_list->SetCurrentListID(static_cast<int>(i));
		}

		ImGui::PushItemWidth(100.0f);
		if (ImGui::BeginCombo("##Style", current_style.c_str()))
		{
			for (UINT i = 0; i < 3; ++i)
			{
				const bool is_selected = (*(m_p_item_list->GetCurrentListID()) == i);
				if (ImGui::Selectable(item_list_vector[i].c_str(), is_selected))
				{
					switch (i)
					{
					case 0: ImGui::StyleColorsDark(); 
					break;
					case 1: ImGui::StyleColorsClassic(); 
					break;
					case 2: ImGui::StyleColorsLight(); 
					break;
					}

					current_style = item_list_vector[i];
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			m_p_item_list->ClearItemList();

			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		ImGui::End();
	}
}
