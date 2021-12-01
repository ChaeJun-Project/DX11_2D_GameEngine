#pragma once

#include "../GUI_Component.h"

class GUI_SpriteEditor;

class GUI_Animator2D final : public GUI_Component
{
public:
	GUI_Animator2D(const std::string& animator_gui_name);
	~GUI_Animator2D();

public:
	void Render() override;

private:
	//애니메이션 List
	GUI_ItemList* m_p_item_list = nullptr;

	GUI_SpriteEditor* m_p_gui_sprite_editor = nullptr;

	bool m_is_sprite_editor_active = false;
};
