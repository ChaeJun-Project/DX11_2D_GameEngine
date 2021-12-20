#include "stdafx.h"
#include "GUI_Function.h"

#include "Scene/ClientSceneManager.h"

#include <DX11_2D_GameEngine_Lib/Core.h>
#include <DX11_2D_GameEngine_Lib/Settings.h>

void ShowInt(const std::string& label_tag, const char* label_name, int& value, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::Text(label_name);
	ImGui::SameLine(indent);
	ImGui::PushItemWidth(size);
	std::string label_str = "##" + label_tag + "_" + FileManager::ConvertCharToString(label_name);
	ImGui::InputInt(label_str.c_str(), &value, flags);
	ImGui::PopItemWidth();
}

void ShowInt2(const char* label_name, int& value_x, int& value_y, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::BeginGroup(); //하나의 그룹으로 묶기
	std::string label_str = FileManager::ConvertCharToString(label_name);
	ImGui::Text(label_name);
	ImGui::SameLine(indent); //같은 라인에서 80만큼 떨어져서 시작

	//Draw X
	ShowInt(label_str, "X", value_x, size, 0.0f, flags); ImGui::SameLine(); //같은 라인에 시작
	//Draw Y
	ShowInt(label_str, "Y", value_y, size, 0.0f, flags);

	ImGui::EndGroup(); //하나의 그룹으로 묶기 해제
}

void ShowFloat(const std::string& label_tag, const char* label_name, float& value, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::Text(label_name);
	ImGui::SameLine(indent);
	ImGui::PushItemWidth(size);
	std::string label_str = "##" + label_tag + "_" + FileManager::ConvertCharToString(label_name);
	ImGui::InputFloat(label_str.c_str(), &value);
	ImGui::PopItemWidth();
}

void ShowFloat2(const char* label_name, Vector2& value, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::BeginGroup(); //하나의 그룹으로 묶기
	std::string label_str = FileManager::ConvertCharToString(label_name);
	ImGui::Text(label_name);
	ImGui::SameLine(indent); //같은 라인에서 80만큼 떨어져서 시작

	//Draw X
	ShowFloat(label_str, "X", value.x, size, 0.0f, flags); ImGui::SameLine(); //같은 라인에 시작
	//Draw Y
	ShowFloat(label_str, "Y", value.y, size, 0.0f, flags);

	ImGui::EndGroup(); //하나의 그룹으로 묶기 해제
}

void ShowFloat3(const char* label_name, Vector3& value, const float& size, const float& indent, ImGuiInputTextFlags flags)
{
	ImGui::BeginGroup(); //하나의 그룹으로 묶기
	ImGui::Text(label_name);
	std::string label_str = FileManager::ConvertCharToString(label_name);
	ImGui::SameLine(indent); //같은 라인에서 80만큼 떨어져서 시작

	//Draw X
	ShowFloat(label_str, "X", value.x, size, 0.0f, flags); ImGui::SameLine(); //같은 라인에 시작
	//Draw Y
	ShowFloat(label_str, "Y", value.y, size, 0.0f, flags); ImGui::SameLine(); //같은 라인에 시작
	//Draw Z
	ShowFloat(label_str, "Z", value.z, size, 0.0f, flags);

	ImGui::EndGroup(); //하나의 그룹으로 묶기 해제
}

const bool CheckMousePositionInRect(const ImVec2& mouse_position, const ImVec2& rect_left_top, const ImVec2& rect_right_bottom)
{
	//현재 마우스 커서의 위치가 Rect 내부에 있는 경우
	if (rect_left_top.x < mouse_position.x && rect_left_top.y < mouse_position.y &&
		rect_right_bottom.x > mouse_position.x && rect_right_bottom.y > mouse_position.y)
		return true;

	else
		return false;
}

void SaveFile(const std::string path, const FileType& file_type)
{
	wchar_t szName[256] = {};

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = Core::GetInstance()->GetSettings()->GetWindowHandle();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	std::wstring file_folder_path;
	switch (file_type)
	{
	case FileType::Scene:
		file_folder_path = L"Scene";
		ofn.lpstrFilter = L"All\0*.*\0Scene\0*.scene\0";
		break;
	case FileType::Tile:
		file_folder_path = L"Tile";
		ofn.lpstrFilter = L"All\0*.*\0Tile\0*.tile\0";
		break;
	case FileType::Animation:
		file_folder_path = L"Animation";
		ofn.lpstrFilter = L"All\0*.*\0Animation\0*.anim\0";
		break;
	}
	
	ofn.lpstrInitialDir = file_folder_path.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Modal
	if (GetSaveFileName(&ofn))
	{
		switch (file_type)
		{
		case FileType::Scene:
			ClientSceneManager::SaveScene(FileManager::ConvertWStringToString(szName));
			break;
		case FileType::Tile:
			
			break;
		case FileType::Animation:
			
			break;
		}
	}
}

void LoadFile(const std::string path, const FileType& file_type)
{
	wchar_t szName[256] = {};

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = Core::GetInstance()->GetSettings()->GetWindowHandle();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	std::wstring file_folder_path;
	switch (file_type)
	{
	case FileType::Scene:
		file_folder_path = L"Scene";
		ofn.lpstrFilter = L"All\0*.*\0Scene\0*.scene\0";
		break;
	case FileType::Tile:
		file_folder_path = L"Tile";
		ofn.lpstrFilter = L"All\0*.*\0Tile\0*.tile\0";
		break;
	case FileType::Animation:
		file_folder_path = L"Animation";
		ofn.lpstrFilter = L"All\0*.*\0Animation\0*.anim\0";
		break;
	}

	ofn.lpstrInitialDir = file_folder_path.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Modal
	if (GetOpenFileName(&ofn))
	{
		switch (file_type)
		{
		case FileType::Scene:
			ClientSceneManager::LoadScene(FileManager::ConvertWStringToString(szName));
			break;
		case FileType::Tile:

			break;
		case FileType::Animation:

			break;
		}
	}
}
