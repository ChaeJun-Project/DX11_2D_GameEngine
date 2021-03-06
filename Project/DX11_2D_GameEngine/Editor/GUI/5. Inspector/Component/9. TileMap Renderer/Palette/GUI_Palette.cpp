#include "stdafx.h"
#include "GUI_Palette.h"

#include "GUI/Module/ItemList/GUI_ItemList.h"

#include <DX11_2D_GameEngine_Lib/ResourceManager.h>
#include <DX11_2D_GameEngine_Lib/Texture.h>

#include <DX11_2D_GameEngine_Lib/TileMapRenderer.h>

GUI_Palette::GUI_Palette()
{
	m_p_tile_atlas_item_list = new GUI_ItemList();
}

GUI_Palette::~GUI_Palette()
{
	m_p_tile_atlas_texture.reset();

	SAFE_DELETE(m_p_tile_atlas_item_list);

	m_p_current_tile_map = nullptr;

	m_draw_list = nullptr;

	m_tile_position_info_vector.clear();
	m_tile_position_info_vector.shrink_to_fit();
}

void GUI_Palette::Render()
{
	if (ImGui::Begin("Palette", &m_is_active, ImGuiWindowFlags_AlwaysAutoResize))
	{
		//Show Tile Altas Texture Combo
		ImGui::PushItemWidth(150.0f);
		{
			int index = 0;

			const auto& tile_atlas_texture_map = RESOURCE_MANAGER->GetResourceMap(ResourceType::Texture);

			std::string current_tile_atlas_name;

			if (m_p_tile_atlas_texture != nullptr)
			{
				current_tile_atlas_name = m_p_tile_atlas_texture->GetResourceName();
			}

			for (auto& tile_atlas_texture : tile_atlas_texture_map)
			{
				m_p_tile_atlas_item_list->AddItem(tile_atlas_texture.second->GetResourceName());

				if (!current_tile_atlas_name.empty() &&
					tile_atlas_texture.second->GetResourceName() == current_tile_atlas_name)
					m_p_tile_atlas_item_list->SetCurrentListID(index);

				else
					++index;
			}

			const auto& item_list_vector = m_p_tile_atlas_item_list->GetItemList();

			if (ImGui::BeginCombo("##Tile Atlas Texture List", current_tile_atlas_name.c_str()))
			{
				for (UINT i = 0; i < static_cast<UINT>(item_list_vector.size()); ++i)
				{
					const bool is_selected = (m_p_tile_atlas_item_list->GetCurrentListID() == i);
					if (ImGui::Selectable(item_list_vector[i].c_str(), is_selected))
					{
						Initialize();

						m_p_tile_atlas_texture = RESOURCE_MANAGER->GetResource<Texture>(item_list_vector[i]);
						m_tile_atlas_texture_size_x = m_p_tile_atlas_texture->GetWidth();
						m_tile_atlas_texture_size_y = m_p_tile_atlas_texture->GetHeight();
						m_canvas_size = ImVec2
						(
							static_cast<float>(m_tile_atlas_texture_size_x),
							static_cast<float>(m_tile_atlas_texture_size_y)
						);
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
			m_p_tile_atlas_item_list->ClearItemList();

			ImGui::PopItemWidth();
		}

		//Load Tile Atlas Texture
		if (auto pay_load = DragDropEvent::ReceiveDragDropPayLoad(PayLoadType::Texture))
		{
			auto p_tile_atlas_texture = RESOURCE_MANAGER->LoadFromFile<Texture>(std::get<std::string>(pay_load->data));

			if (p_tile_atlas_texture != nullptr && m_p_current_tile_map != nullptr)
			{
				Initialize();

				m_p_tile_atlas_texture = p_tile_atlas_texture;
				m_tile_atlas_texture_size_x = m_p_tile_atlas_texture->GetWidth();
				m_tile_atlas_texture_size_y = m_p_tile_atlas_texture->GetHeight();
				m_canvas_size = ImVec2
				(
					static_cast<float>(m_tile_atlas_texture_size_x),
					static_cast<float>(m_tile_atlas_texture_size_y)
				);
			}
		}

		ImGui::Separator();

		//Show Tile Atlas Texture
		RenderTileAtlasTexture();
	}
	ImGui::End();
}

void GUI_Palette::Initialize()
{
	//Tile Atlas Texture
	m_p_tile_atlas_texture = nullptr;
	m_tile_atlas_texture_size_x = 0;
	m_tile_atlas_texture_size_y = 0;
	m_canvas_size = ImVec2(0.0f, 0.0f);
	m_canvas_left_top = ImVec2(0.0f, 0.0f);
	m_canvas_right_bottom = ImVec2(0.0f, 0.0f);

	//Tile Size
	m_tile_size = Vector2::Zero;

	//Draw Rect
	m_draw_list = nullptr;

	m_tile_count_row = 0; //???? ???? ???? ????
	m_tile_count_column = 0; //???? ???? ???? ????

	m_tile_position_info_vector.clear();
	m_current_tile_index = -1;
}

void GUI_Palette::RenderTileAtlasTexture()
{
	if (m_p_current_tile_map == nullptr)
		return;

	ImVec2 canvas_rs = ImGui::GetContentRegionAvail();

	m_canvas_left_top = ImGui::GetCursorScreenPos();
	m_canvas_right_bottom = m_canvas_left_top;

	//Resize Grid Area
	//X
	if (m_canvas_size.x >= canvas_rs.x)
		m_canvas_right_bottom.x += canvas_rs.x;
	else
		m_canvas_right_bottom.x += m_canvas_size.x;

    //Y
	if (m_canvas_size.y >= canvas_rs.y)
		m_canvas_right_bottom.y += canvas_rs.y;
	else
		m_canvas_right_bottom.y += m_canvas_size.y;

	ImGui::Image
	(
		m_p_tile_atlas_texture ? m_p_tile_atlas_texture->GetShaderResourceView() : nullptr,
		m_canvas_size,
		ImVec2(0.0f, 0.0f),
		ImVec2(1.0f, 1.0f),
		ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
	);

	const bool is_hovered = ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly); // Hovered

	//Show Tile Atlas Texture Size Info
	static std::string tile_atlas_texture_size;
	tile_atlas_texture_size = std::to_string(m_tile_atlas_texture_size_x) + "X" + std::to_string(m_tile_atlas_texture_size_y);
	ImGui::BulletText(tile_atlas_texture_size.c_str());

	//Show Mouse Position In Canvas
	ShowMousePosition(is_hovered);

	//Tile Size
	ShowFloat2("Tile Size", m_tile_size, 70.0f, 100.0f);

	if (ImGui::Button("Apply", ImVec2(110.0f, 0.0f)))
	{
		m_tile_count_row = static_cast<int>(m_canvas_size.y / m_tile_size.y); //?????? ???? ????
		m_tile_count_column = static_cast<int>(m_canvas_size.x / m_tile_size.x); //?????? ???? ????
	}

	AddTilePosition(m_tile_size);

	if (is_hovered && ImGui::IsMouseClicked(0))
	{
		ImGuiIO& io = ImGui::GetIO();
		CalcCurrentPickRect(ImVec2(io.MousePos.x, io.MousePos.y));
	}

	if (!m_tile_position_info_vector.empty())
	{
		m_draw_list = ImGui::GetWindowDrawList();
		m_draw_list->PushClipRect(m_canvas_left_top, ImVec2(m_canvas_right_bottom.x + 1.0f, m_canvas_right_bottom.y + 1.0f));

		//Draw Grid
		//Row
		for (int row = 0; row <= m_tile_count_row; ++row)
		{
			m_draw_list->AddLine
			(
				ImVec2(m_canvas_left_top.x, m_canvas_left_top.y + (m_tile_size.x * (row))),
				ImVec2(m_canvas_right_bottom.x, m_canvas_left_top.y + (m_tile_size.x * (row))),
				IM_COL32(255, 255, 255, 255),
				1.0f
			);
		}
		//Column
		for (int column = 0; column <= m_tile_count_column; ++column)
		{
			//Vertical Cross
			m_draw_list->AddLine
			(
				ImVec2(m_canvas_left_top.x + (m_tile_size.y * (column)), m_canvas_left_top.y),
				ImVec2(m_canvas_left_top.x + (m_tile_size.y * (column)), m_canvas_right_bottom.y),
				IM_COL32(255, 255, 255, 255),
				1.0f
			);
		}

		m_draw_list->PopClipRect();

		if (m_current_tile_index != -1)
		{
			//Draw Pick Rect
			m_draw_list->AddRect
			(
				m_tile_position_info_vector[m_current_tile_index].left_top,
				m_tile_position_info_vector[m_current_tile_index].right_bottom,
				IM_COL32(0, 255, 0, 255),
				1.0f
			);

			ShowSelectedTile(m_tile_size);
		}
	}
}

void GUI_Palette::ShowSelectedTile(const Vector2& tile_size)
{
	int id = m_current_tile_index;

	ImVec2 uv_left_top;
	uv_left_top.x = (m_tile_position_info_vector[id].left_top.x - m_canvas_left_top.x) / m_canvas_size.x;
	uv_left_top.y = (m_tile_position_info_vector[id].left_top.y - m_canvas_left_top.y) / m_canvas_size.y;

	float uv_offset_x = tile_size.x / m_canvas_size.x;
	float uv_offset_y = tile_size.y / m_canvas_size.y;

	ImVec2 uv_right_bottom;
	uv_right_bottom.x = uv_left_top.x + uv_offset_x;
	uv_right_bottom.y = uv_left_top.y + uv_offset_y;

	ImGui::Image
	(
		m_p_tile_atlas_texture->GetShaderResourceView(),
		ImVec2(150.0f, 150.0f),
		uv_left_top,
		uv_right_bottom,
		ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
		ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
}

void GUI_Palette::CalcCurrentPickRect(const ImVec2& current_screen_pos)
{
	for (int i = 0; i < static_cast<int>(m_tile_position_info_vector.size()); ++i)
	{
		if (CheckMousePositionInRect(
			current_screen_pos,
			m_tile_position_info_vector[i].left_top,
			m_tile_position_info_vector[i].right_bottom))
		{
		    //Set Current Tile Atlas Texture
			m_p_current_tile_map->SetTileAtlasTexture(m_p_tile_atlas_texture);

			//Set Current Tile Index
			m_current_tile_index = i;
			m_p_current_tile_map->SetCurrentTileIndex(m_current_tile_index);
		}
	}
}

void GUI_Palette::AddTilePosition(const Vector2& tile_size)
{
	if (!m_tile_position_info_vector.empty())
		m_tile_position_info_vector.clear();

	m_tile_position_info_vector.resize(m_tile_count_row * m_tile_count_column);

	for (int row = 0; row < m_tile_count_row; ++row) //??
	{
		for (int column = 0; column < m_tile_count_column; ++column) //??
		{
			//Left Top
			m_tile_position_info_vector[m_tile_count_column * row + column].left_top = ImVec2
			(
				m_canvas_left_top.x + (tile_size.x * column),
				m_canvas_left_top.y + (tile_size.y * row)
			);

			//Right Bottom
			m_tile_position_info_vector[m_tile_count_column * row + column].right_bottom = ImVec2
			(
				m_tile_position_info_vector[m_tile_count_column * row + column].left_top.x + tile_size.x,
				m_tile_position_info_vector[m_tile_count_column * row + column].left_top.y + tile_size.y
			);
		}
	}
}

void GUI_Palette::ShowMousePosition(const bool& is_hovered)
{
	//Current Mouse Pose
	ImGui::BulletText("Mouse Position In Canvas");
	ImGui::SameLine();
	ImGui::Spacing();
	ImGui::SameLine();

	ImGuiIO& io = ImGui::GetIO();
	const ImVec2 mouse_pos_in_canvas = ImVec2(io.MousePos.x, io.MousePos.y);

	static std::string current_mouse_pose;
	if (is_hovered)
		current_mouse_pose = "X: " + std::to_string(static_cast<int>(mouse_pos_in_canvas.x)) + " Y: " + std::to_string(static_cast<int>(mouse_pos_in_canvas.y));

	else
		current_mouse_pose = "X: " + std::to_string(0) + " Y: " + std::to_string(0);

	ImGui::Text(current_mouse_pose.c_str());
}
