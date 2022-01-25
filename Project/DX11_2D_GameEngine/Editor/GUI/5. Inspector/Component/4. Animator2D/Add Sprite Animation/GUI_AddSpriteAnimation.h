#pragma once

class GUI_ItemList;

class SpriteAnimation;
class Animator2D;

class GUI_AddSpriteAnimation final
{
public:
	GUI_AddSpriteAnimation();
	~GUI_AddSpriteAnimation();

public:
	void Initialize();
	void Render();

private:
    //현재 참조하고 있는 Animator2D
	Animator2D* p_current_animator2D = nullptr;

	//Sprite Animation List
	std::unique_ptr<GUI_ItemList> m_p_sprite_animation_item_list = nullptr;
	//Added Sprite Animation List
	std::unique_ptr<GUI_ItemList> m_p_added_sprite_animation_item_list = nullptr;

	//Render Add Sprite Animation Window
	bool m_is_active = false;

	friend class GUI_Animator2D;
};

