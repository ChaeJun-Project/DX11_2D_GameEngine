#include "stdafx.h"
#include "FileFunction.h"

#include "Scene/ClientSceneManager.h"

#include <DX11_2D_GameEngine_Lib/Core.h>
#include <DX11_2D_GameEngine_Lib/Settings.h>

#include <DX11_2D_GameEngine_Lib/ResourceManager.h>

#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib/Scene.h>

#include <DX11_2D_GameEngine_Lib/TileMap.h>
#include <DX11_2D_GameEngine_Lib/SpriteAnimation.h>

void FileFunction::SaveFile(const std::string& save_resource_folder_path, const std::string& file_name, const FileType& file_type)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	wchar_t szName[256] = {};

	std::string absolute_save_path = ABSOLUTE_CONTENT_PATH;
	absolute_save_path += save_resource_folder_path;
	std::wstring file_folder_path = FILE_MANAGER->ConvertStringToWString(absolute_save_path);
	SetCurrentDirectory(file_folder_path.c_str()); //해당 경로를 현재 작업 중인 디렉토리로 설정

	std::wstring file_name_wstr = FILE_MANAGER->ConvertStringToWString(file_name);
	
	switch (file_type)
	{
	case FileType::Scene:
	{
		ofn.lpstrFilter = L"All\0*.*\0Scene\0*.scene\0";

		file_name_wstr += L".scene";
	}
	break;
	case FileType::TileMap:
	{
		ofn.lpstrFilter = L"All\0*.*\0Tile\0*.tile\0";

		file_name_wstr += L".tile";
	}
	break;
	case FileType::Animation:
	{
		ofn.lpstrFilter = L"All\0*.*\0Animation\0*.anim\0";

		file_name_wstr += L".anim";
	}
	break;
	}

	wcscpy_s(szName, file_name_wstr.c_str());

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
			SaveScene(FILE_MANAGER->ConvertWStringToString(szName));
			break;
		case FileType::TileMap:
			SaveResource<TileMap>(FILE_MANAGER->ConvertWStringToString(szName));
			break;
		case FileType::Animation:
			SaveResource<SpriteAnimation>(FILE_MANAGER->ConvertWStringToString(szName));
			break;
		}
	}
}

void FileFunction::SaveScene(const std::string& scene_path)
{
	auto p_save_scene = ClientSceneManager::SaveScene(scene_path);

	if (p_save_scene != nullptr)
	{
		auto scene_name = p_save_scene->GetSceneName();
		EDITOR_LOG_INFO_F("Succeeded in Saving File: [%s]", scene_name.c_str());
	}
}

const std::string FileFunction::LoadFile(const std::string& load_resource_folder_path, const FileType& file_type)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	wchar_t szName[256] = {};

	std::string absolute_load_path = ABSOLUTE_CONTENT_PATH;
	absolute_load_path += load_resource_folder_path;
	std::wstring file_folder_path = FILE_MANAGER->ConvertStringToWString(absolute_load_path);
	SetCurrentDirectory(file_folder_path.c_str()); //해당 경로를 현재 작업 중인 디렉토리로 설정
	switch (file_type)
	{
	case FileType::Scene:
	{
		ofn.lpstrFilter = L"All\0*.*\0Scene\0*.scene\0";
	}
	break;
	case FileType::TileMap:
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
		return FILE_MANAGER->ConvertWStringToString(szName);
	}

	return std::string();
}

void FileFunction::LoadScene(const std::string& scene_path)
{
	auto next_scene = ClientSceneManager::LoadScene(scene_path);

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