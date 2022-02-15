#include "stdafx.h"
#include "GUI_FileItem.h"

#include "Helper/IconProvider.h"

#include "GUI_FileDialog.h"

GUI_FileItem::GUI_FileItem(const FileThumbnailType& file_thumbnail_type, const PayLoad& pay_load)
	:m_file_thumbnail_type(file_thumbnail_type),
	m_pay_load(pay_load)
{
	//Initialize Thumbnail Texture
	if (m_file_thumbnail_type == FileThumbnailType::Texture)
	{
		m_p_thumbnail_texture = RESOURCE_MANAGER->CreateFileItemThumbnailTexture(std::get<std::string>(m_pay_load.data));
	}

	else
		m_p_thumbnail_texture = ICON_PROVIDER->GetFileThumbnail(m_file_thumbnail_type);


	m_folder_path = FILE_MANAGER->GetDirectoryPathFromPath(std::get<std::string>(m_pay_load.data));
	m_file_name = FILE_MANAGER->GetOriginFileNameFromPath(std::get<std::string>(m_pay_load.data));
	m_file_extension = FILE_MANAGER->GetExtensionFromPath(std::get<std::string>(m_pay_load.data));
}

void GUI_FileItem::Render()
{
	auto& imgui_context = *GImGui;
	auto& style = ImGui::GetStyle();

	auto begin_cursor_screen_pos = ImGui::GetCursorScreenPos();

	//FileItem
	ImGui::BeginGroup();
	{
		//FileItem Thumbnail
		ImGui::PushID(m_file_name.c_str());
		ImGui::PushStyleColor(ImGuiCol_Button, m_is_clicked ? ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] : ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);

		ImGui::Image(m_p_thumbnail_texture->GetShaderResourceView(), m_file_item_size);

		//Check Clicked FileItem
		CheckClickedFileItem();

		//Show Delete FileItem Popup
		ShowMenuPopup();

		//Check Drag FileItem
		DragFileItem();

		//FileItem의 Thumbnail과 Name 사이의 공간을 없앰
		auto cursor_screen_pos = ImGui::GetCursorScreenPos();
		ImGui::SetCursorScreenPos(ImVec2(cursor_screen_pos.x, cursor_screen_pos.y - style.FramePadding.y));

		//FileItem Name
		std::string label_str = "##" + m_file_name;
		auto file_name = m_file_name;
		ImGui::PushItemWidth(m_file_item_size.x);
		if (ImGui::InputText(label_str.c_str(), &file_name, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
		{
			//파일 이름 변경
			FILE_MANAGER->RenameFile(m_folder_path, m_file_extension, m_file_name, file_name);
			m_file_name = file_name;
		}
		ImGui::PopItemWidth();

		auto end_cursor_screen_pos = ImGui::GetCursorScreenPos();
		end_cursor_screen_pos.x += m_file_item_size.x;
		end_cursor_screen_pos.y -= style.FramePadding.y;

		//Draw Clicked State
		if (m_is_clicked)
		{
			auto draw_list = ImGui::GetWindowDrawList();
			draw_list->AddRectFilled(begin_cursor_screen_pos, end_cursor_screen_pos, IM_COL32(0, 0, 200, 80));
		}

		ImGui::PopStyleColor(2);
		ImGui::PopID();
	}
	ImGui::EndGroup();
}

void GUI_FileItem::CheckClickedFileItem()
{
	if (ImGui::IsItemHovered())
	{
		//한 번만 클릭했을 경우(마우스 왼쪽 버튼)
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			m_is_clicked = true;

			switch (m_pay_load.type)
			{
			case PayLoadType::Audio:
				break;
			case PayLoadType::Material:
				break;
			case PayLoadType::Mesh:
				break;
			case PayLoadType::Prefab:
				break;
			case PayLoadType::Texture:
				break;
			}
		}

		//한 번만 클릭했을 경우(마우스 오른쪽 버튼)
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			m_is_clicked = true;

			ImGui::OpenPopup("Delete FileItem Popup");
		}

		//더블 클릭했을 경우
		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			switch (m_pay_load.type)
			{
			case PayLoadType::Folder:
			{
				m_is_clicked = false;

				m_p_owner_file_dialog->SetCurrentFolderPath(std::get<std::string>(m_pay_load.data));
				m_p_owner_file_dialog->IsClearFileItem();
			}
			break;
			case PayLoadType::Scene:
			{
				m_is_clicked = false;

				FileFunction::LoadScene(std::get<std::string>(m_pay_load.data));
			}
			break;
			}
		}
	}

	else
	{
		//어떠한 아이템 위에도 커서가 올라가 있지 않은 상태에서 마우스 왼쪽 버튼을 눌렀을 경우
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			m_is_clicked = false;
		}
	}
}

void GUI_FileItem::DragFileItem()
{
	if (m_pay_load.type != PayLoadType::None && m_pay_load.type != PayLoadType::GameObject && m_pay_load.type != PayLoadType::Folder)
	{
		//드래그 시작
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			DragDropEvent::CreateDragDropPayLoad(m_pay_load);

			ImGui::Image
			(
				m_p_thumbnail_texture->GetShaderResourceView(),
				m_file_item_size
			);

			ImGui::EndDragDropSource();
		}
	}
}

void GUI_FileItem::ShowMenuPopup()
{
	if (!ImGui::BeginPopup("Delete FileItem Popup"))
		return;

	if (ImGui::MenuItem("Delete"))
	{
		FILE_MANAGER->RemoveFile(std::get<std::string>(m_pay_load.data));
		m_p_owner_file_dialog->IsClearFileItem();
	}

	ImGui::EndPopup();
}
