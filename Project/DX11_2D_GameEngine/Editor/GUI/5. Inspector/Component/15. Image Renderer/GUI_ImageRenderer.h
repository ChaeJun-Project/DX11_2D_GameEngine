#pragma once

#include "../GUI_Component.h"

class GUI_ImageRenderer final : public GUI_Component
{
public:
	GUI_ImageRenderer(const std::string& image_renderer_gui_name);
	~GUI_ImageRenderer();

	void Render() override;
};
