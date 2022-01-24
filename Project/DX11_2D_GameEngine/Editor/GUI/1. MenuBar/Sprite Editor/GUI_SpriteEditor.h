#pragma once

class Animator2D;
class GUI_ItemList;

class Animation2D;
class Texture;

class GUI_SpritePlayer;

class GUI_SpriteEditor final
{
public:
	GUI_SpriteEditor();
	~GUI_SpriteEditor();

public:
	void Render();

private:
	void ShowAnimationEditor();
	void DrawRect(const bool& is_hovered, const bool& is_active);

private:
	//Atlas Texture
	std::shared_ptr<Texture> m_p_atlas_texture = nullptr;
	//Atlas Texture List
	GUI_ItemList* m_p_atlas_item_list = nullptr;
	Vector2 m_atlas_texture_size = Vector2::Zero;

	//Animation2D
	std::shared_ptr<Animation2D> m_p_animation2D = nullptr;
	//Animation2D List
	GUI_ItemList* m_p_animation_item_list = nullptr;

	ImVec2 canvas_left_top = ImVec2(0.0f, 0.0f);
	ImVec2 canvas_right_bottom = ImVec2(0.0f, 0.0f);

	int m_frame_index = 0;
	int m_total_frame_count = 0;

	//Draw Rect
	ImDrawList* draw_list = nullptr;
	ImVec2 m_mouse_pos_in_canvas = ImVec2(0.0f, 0.0f);
	ImVec2 m_start_point_in_canvas = ImVec2(0.0f, 0.0f);
	ImVec2 m_end_point_in_canvas = ImVec2(0.0f, 0.0f);
	Vector2 m_draw_left_top = Vector2(0.0f, 0.0f);
	Vector2 m_draw_right_bottom = Vector2(0.0f, 0.0f);

	bool m_is_active = false;

	//Sprite Player
	GUI_SpritePlayer* m_p_gui_sprite_player = nullptr;

	friend class GUI_MenuBar;
};

