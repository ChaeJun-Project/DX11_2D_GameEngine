#pragma once

#include "../GUI_Component.h"

class GUI_ItemList;
class Light2D;

class GUI_Light2D final : public GUI_Component
{
public:
	GUI_Light2D(const std::string& light2D_gui_name);
	~GUI_Light2D();

	void Render() override;

private:
    void ShowComboLightType(Light2D* p_light2D, Light2DType& light2D_type);

private:
	ImGuiColorEditFlags m_color_edit_flag = 0;
	GUI_ItemList* m_p_light_type_list = nullptr;
};
