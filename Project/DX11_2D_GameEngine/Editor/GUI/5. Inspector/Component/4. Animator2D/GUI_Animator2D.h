#pragma once

#include "../GUI_Component.h"

class GUI_AddSpriteAnimation;

class GUI_Animator2D final : public GUI_Component
{
public:
	GUI_Animator2D(const std::string& animator_gui_name);
	~GUI_Animator2D();

public:
	void Render() override;

private:
	//Add Sprite Animation Window
	std::unique_ptr<GUI_AddSpriteAnimation> m_p_add_sprite_animation = nullptr;
};
