#pragma once

#include "../GUI_Component.h"

class GUI_Camera final : public GUI_Component
{
public:
	GUI_Camera(const std::string& camera_gui_name);
	virtual ~GUI_Camera() = default;

	void Render() override;
};



