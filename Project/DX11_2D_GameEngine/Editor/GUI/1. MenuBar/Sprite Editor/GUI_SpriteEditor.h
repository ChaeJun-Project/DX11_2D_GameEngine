#pragma once

class Animator2D;
class GUI_ItemList;

class SpriteAnimation;
class Texture;

class GUI_SpritePlayer;

class GUI_SpriteEditor final
{
public:
	GUI_SpriteEditor();
	~GUI_SpriteEditor();

public:
    void Initialize();
	void Render();

private:
	void ShowAnimationEditor();
	void DrawRect(const bool& is_hovered, const bool& is_active);

private:
	//Atlas Texture
	std::shared_ptr<Texture> m_p_atlas_texture = nullptr;
	Vector2 m_atlas_texture_size = Vector2::Zero;

	//Atlas Texture List
	GUI_ItemList* m_p_atlas_item_list = nullptr;
	//Sprite Animation List
	GUI_ItemList* m_p_sprtie_animation_item_list = nullptr;

	//Sprite Animation
	bool m_is_create_new_sprite_animation = false;
	std::string m_animation_name;
	std::shared_ptr<SpriteAnimation> m_p_sprite_animation = nullptr;
	int m_frame_index = 0;
	int m_total_frame_count = 0;

	//Draw Rect
	ImDrawList* m_draw_list = nullptr;
	ImVec2 m_canvas_left_top = ImVec2(0.0f, 0.0f);
	ImVec2 m_canvas_right_bottom = ImVec2(0.0f, 0.0f);

	ImVec2 m_mouse_pos_in_canvas = ImVec2(0.0f, 0.0f);

	ImVec2 m_start_point_in_canvas = ImVec2(0.0f, 0.0f);
	ImVec2 m_end_point_in_canvas = ImVec2(0.0f, 0.0f);

	Vector2 m_draw_left_top = Vector2(0.0f, 0.0f);
	Vector2 m_draw_right_bottom = Vector2(0.0f, 0.0f);

	//Render Sprite Editor
	bool m_is_active = false;

	//Sprite Player
	GUI_SpritePlayer* m_p_gui_sprite_player = nullptr;

	friend class GUI_MenuBar;
};

