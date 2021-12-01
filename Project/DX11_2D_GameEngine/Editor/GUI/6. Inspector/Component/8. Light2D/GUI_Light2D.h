#pragma once

#include "../GUI_Component.h"

class GUI_Light2D final : public GUI_Component
{
public:
	GUI_Light2D(const std::string& light2D_gui_name);
	~GUI_Light2D() = default;

	void Render() override;
};
