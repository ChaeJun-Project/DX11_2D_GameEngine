#pragma once

#include "GUI/IGUI_Popup.h"

class GUI_ItemList;

class Texture;

class TileMapRenderer;

//���� Ÿ���� ����
struct TilePositionInfo
{
	ImVec2 left_top;		//���� �������� ���� ��� ��ǥ
	ImVec2 right_bottom;	//���� �������� ������ �ϴ� ��ǥ
};

class GUI_Palette final : public IGUI_Popup
{
public:
	GUI_Palette();
	virtual ~GUI_Palette();

public:
	virtual void Render() override;

private:
    void RenderTileAtlasTexture();
	void ShowSelectedTile(const Vector2& per_tile_size);
	void CalcCurrentPickRect(const ImVec2& current_screen_pos);
	void AddTilePosition(const Vector2& per_tile_size);
	void ShowMousePosition(const bool& is_hovered);

private:
	//Tile Atlas Texture
	std::shared_ptr<Texture> m_p_tile_atlas_texture = nullptr;
	UINT m_tile_atlas_texture_size_x = 0;
	UINT m_tile_atlas_texture_size_y = 0;
	ImVec2 m_canvas_size = ImVec2(0.0f, 0.0f);
	ImVec2 m_canvas_left_top = ImVec2(0.0f, 0.0f);
	ImVec2 m_canvas_right_bottom = ImVec2(0.0f, 0.0f);

	//Tile Atlas Texture List
	GUI_ItemList* m_p_tile_atlas_item_list = nullptr;
 
	//TileMapRenderer
	TileMapRenderer* m_p_current_tile_map = nullptr;

	//Draw Rect
	ImDrawList* draw_list = nullptr;

	int m_tile_count_row = 0; //Ÿ�� ���� ���� ����
	int m_tile_count_column = 0; //Ÿ�� ���� ���� ����

	std::vector<TilePositionInfo> m_tile_position_info_vector;
	int m_current_tile_index = -1;

	friend class GUI_TileMapRenderer;
};

