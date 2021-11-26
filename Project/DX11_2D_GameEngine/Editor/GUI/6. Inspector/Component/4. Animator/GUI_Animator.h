#pragma once

#include "../GUI_Component.h"

class GUI_Animator final : public GUI_Component
{
public:
	GUI_Animator(const std::string& animator_gui_name);
	~GUI_Animator() = default;

	void Render() override;
};
