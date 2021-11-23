#pragma once

#include "../GUI_Component.h"

class GUI_SpriteRenderer final : public GUI_Component
{
public:
	GUI_SpriteRenderer(const std::string& sprite_renderer_gui_name);
	virtual ~GUI_SpriteRenderer() = default;

	void Render() override;
};