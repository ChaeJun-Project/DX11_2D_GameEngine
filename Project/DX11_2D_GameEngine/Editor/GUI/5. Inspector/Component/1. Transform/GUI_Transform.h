#pragma once

#include "../GUI_Component.h"

class GUI_Transform final : public GUI_Component
{
public:
	GUI_Transform(const std::string& transform_gui_name);
	~GUI_Transform() = default;

	void Render() override;
};

