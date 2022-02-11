#pragma once

#include "../GUI_Component.h"

class GUI_AddSpriteAnimation;
class GUI_ItemList;

class GUI_Animator2D final : public GUI_Component
{
public:
	GUI_Animator2D(const std::string& animator_gui_name);
	~GUI_Animator2D();

public:
	void Render() override;

private:
	//Sprite Animation List
	std::unique_ptr<GUI_ItemList> m_p_sprite_animation_list = nullptr;
};
