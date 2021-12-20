#pragma once

#include "../GUI_Component.h"

class GUI_ItemList;

class GUI_Camera final : public GUI_Component
{
public:
	GUI_Camera(const std::string& camera_gui_name);
	~GUI_Camera();

	void Render() override;

private:
    void SetProjectionType(Camera* camera, const std::string& projection_type_str);

private:
	GUI_ItemList* m_p_projection_list = nullptr;
	GUI_ItemList* m_p_culling_layer_list = nullptr;
};



