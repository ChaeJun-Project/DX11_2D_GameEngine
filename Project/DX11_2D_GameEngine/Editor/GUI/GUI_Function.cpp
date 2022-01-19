#include "stdafx.h"
#include "GUI_Function.h"

#include "Scene/ClientSceneManager.h"

#include <DX11_2D_GameEngine_Lib/Core.h>
#include <DX11_2D_GameEngine_Lib/Settings.h>
#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib/Scene.h>

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

void ShowFloat4(const char* label_name, Vector4& value, const float& size, const float& indent, ImGuiInputTextFlags flags)
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
	ShowFloat(label_str, "Z", value.z, size, 0.0f, flags); ImGui::SameLine(); //같은 라인에 시작
	//Draw W
	ShowFloat(label_str, "W", value.z, size, 0.0f, flags);

	ImGui::EndGroup(); //하나의 그룹으로 묶기 해제
}

void ShowColorPicker3(const std::string& label_name, float* data, const ImGuiColorEditFlags& flags)
{
	ImGui::Text(label_name.c_str());
	ImGui::SameLine(100.0f);

	std::string label_tag = "##ColorPicker_" + label_name;
	ImGui::ColorEdit3(label_tag.c_str(), data, flags);
}

void ShowColorPicker4(const std::string& label_name, float* data, const ImGuiColorEditFlags& flags)
{
	ImGui::Text(label_name.c_str());
	ImGui::SameLine(100.0f);

	std::string label_tag = "##ColorPicker_" + label_name;
	ImGui::ColorEdit4(label_tag.c_str() , data, flags);
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
void SaveFile(const std::string& path, const FileType& file_type)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	wchar_t szName[256] = {};

	std::wstring file_folder_path = FileManager::ConvertStringToWString(path);
	SetCurrentDirectory(file_folder_path.c_str()); //해당 경로를 현재 작업 중인 디렉토리로 설정
	switch (file_type)
	{
	case FileType::Scene:
	{
		ofn.lpstrFilter = L"All\0*.*\0Scene\0*.scene\0";

		auto current_scene = SceneManager::GetInstance()->GetCurrentScene();
		auto current_scene_name_with_extension = current_scene->GetSceneName() + ".scene";
		std::wstring scene_name = FileManager::ConvertStringToWString(current_scene_name_with_extension);

		wcscpy_s(szName, scene_name.c_str());
	}
	break;
	case FileType::Tile:
	{
		ofn.lpstrFilter = L"All\0*.*\0Tile\0*.tile\0";
	}
	break;
	case FileType::Animation:
	{
		ofn.lpstrFilter = L"All\0*.*\0Animation\0*.anim\0";
	}
	break;
	}

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = Core::GetInstance()->GetSettings()->GetWindowHandle();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	ofn.lpstrInitialDir = L"."; //현재 디렉토리를 기준으로 파일 다이얼로그 창을 엶

	// Modal
	if (GetSaveFileName(&ofn))
	{
		switch (file_type)
		{
		case FileType::Scene:
			SaveScene(FileManager::ConvertWStringToString(szName));
			break;
		case FileType::Tile:

			break;
		case FileType::Animation:

			break;
		}
	}
}

void SaveScene(const std::string& path)
{
	auto save_scene = ClientSceneManager::SaveScene(path);

	if (save_scene != nullptr)
	{
		auto scene_name = save_scene->GetSceneName();
		EDITOR_LOG_INFO_F("Succeeded in Saving File: [%s]", scene_name.c_str());
	}
}

void LoadFile(const std::string& path, const FileType& file_type)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	wchar_t szName[256] = {};

	std::wstring file_folder_path = FileManager::ConvertStringToWString(path);
	SetCurrentDirectory(file_folder_path.c_str()); //해당 경로를 현재 작업 중인 디렉토리로 설정
	switch (file_type)
	{
	case FileType::Scene:
	{
		ofn.lpstrFilter = L"All\0*.*\0Scene\0*.scene\0";
	}
	break;
	case FileType::Tile:
	{
		ofn.lpstrFilter = L"All\0*.*\0Tile\0*.tile\0";
	}
	break;
	case FileType::Animation:
	{
		ofn.lpstrFilter = L"All\0*.*\0Animation\0*.anim\0";
	}
	break;
	}

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = Core::GetInstance()->GetSettings()->GetWindowHandle();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	ofn.lpstrInitialDir = L"."; //현재 디렉토리를 기준으로 파일 다이얼로그 창을 엶

	// Modal
	if (GetOpenFileName(&ofn))
	{
		switch (file_type)
		{
		case FileType::Scene:
			LoadScene(FileManager::ConvertWStringToString(szName));
			break;
		case FileType::Tile:

			break;
		case FileType::Animation:

			break;
		}
	}
}

void LoadScene(const std::string& path)
{
	auto next_scene = ClientSceneManager::LoadScene(path);

	if (next_scene != nullptr)
	{
		auto scene_name = next_scene->GetSceneName();
		EDITOR_LOG_INFO_F("Succeeded in Loading File: [%s]", scene_name.c_str());
	}

	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Scene_Change;
	event_struct.object_address_1 = next_scene;

	EventManager::GetInstance()->AddEvent(event_struct);
}

