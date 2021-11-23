#pragma once

#include "../GUI_Component.h"

class GUI_Collider2D final : public GUI_Component
{
public:
	GUI_Collider2D(const std::string& collider2D_name);
	virtual ~GUI_Collider2D() = default;

	void Render() override;
};
