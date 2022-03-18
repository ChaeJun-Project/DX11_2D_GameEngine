#pragma once

#include "../GUI_Component.h"

class GUI_RectTransform final : public GUI_Component
{
public:
	GUI_RectTransform(const std::string& rect_transform_gui_name);
	~GUI_RectTransform() = default;

	void Render() override;
};

