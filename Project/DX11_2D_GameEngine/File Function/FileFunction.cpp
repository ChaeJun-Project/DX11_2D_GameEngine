#include "stdafx.h"
#include "FileFunction.h"

#include "Scene/ClientSceneManager.h"

#include <DX11_2D_GameEngine_Lib/Core.h>
#include <DX11_2D_GameEngine_Lib/Settings.h>

#include <DX11_2D_GameEngine_Lib/ResourceManager.h>

#include <DX11_2D_GameEngine_Lib/SceneManager.h>
#include <DX11_2D_GameEngine_Lib/Scene.h>

#include <DX11_2D_GameEngine_Lib/EventManager.h>

#include <DX11_2D_GameEngine_Lib/TileMap.h>
#include <DX11_2D_GameEngine_Lib/SpriteAnimation.h>

bool FileFunction::SaveFile(const std::string& save_resource_folder_path, const std::string& file_name, const FileType& file_type)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	wchar_t szName[256] = {};

	std::string absolute_save_path = save_resource_folder_path;
	std::wstring file_folder_path = FILE_MANAGER->ConvertStringToWString(absolute_save_path);
	SetCurrentDirectory(file_folder_path.c_str()); //�ش� ��θ� ���� �۾� ���� ���丮�� ����

	std::wstring file_name_wstr = FILE_MANAGER->ConvertStringToWString(file_name);

	switch (file_type)
	{
	case FileType::Scene:
	{
		ofn.lpstrFilter = L"Scene\0*.scene\0All\0*.*\0"; //����
		ofn.lpstrDefExt = L"scene";						 //����Ʈ Ȯ����

		file_name_wstr += L".scene";
	}
	break;
	case FileType::TileMap:
	{
		ofn.lpstrFilter = L"Tile\0*.tile\0All\0*.*\0";	//����
		ofn.lpstrDefExt = L"tile";						//����Ʈ Ȯ����

		file_name_wstr += L".tile";
	}
	break;
	case FileType::Animation:
	{
		ofn.lpstrFilter = L"Animation\0*.anim\0All\0*.*\0"; //����
		ofn.lpstrDefExt = L"anim";							//����Ʈ Ȯ����

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
	ofn.lpstrInitialDir = L"."; //���� ���丮�� �������� ���� ���̾�α� â�� ��

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

		return true;
	}

	   return false;
}

void FileFunction::SaveScene(const std::string& scene_path)
{
	if (scene_path.empty())
		return;

	auto scene_name = FILE_MANAGER->GetOriginFileNameFromPath(scene_path);

	if (ClientSceneManager::SaveScene(scene_path))
		EDITOR_LOG_INFO_F("Scene ���� ���忡 �����߽��ϴ�: [%s]", scene_name.c_str())
	else
		EDITOR_LOG_ERROR_F("Scene ���� ���忡 �����߽��ϴ�: [%s]", scene_name.c_str())
}

const std::string FileFunction::LoadFile(const std::string& load_resource_folder_path, const FileType& file_type)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	wchar_t szName[256] = {};

	std::string absolute_load_path = load_resource_folder_path;
	std::wstring file_folder_path = FILE_MANAGER->ConvertStringToWString(absolute_load_path);
	SetCurrentDirectory(file_folder_path.c_str()); //�ش� ��θ� ���� �۾� ���� ���丮�� ����
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
	ofn.lpstrInitialDir = L"."; //���� ���丮�� �������� ���� ���̾�α� â�� ��

	// Modal
	if (GetOpenFileName(&ofn))
	{
		return FILE_MANAGER->ConvertWStringToString(szName);
	}

	return std::string();
}

void FileFunction::LoadScene(const std::string& scene_path)
{
	if (scene_path.empty())
		return;

	auto scene_name = FILE_MANAGER->GetOriginFileNameFromPath(scene_path);
	auto next_scene = ClientSceneManager::LoadScene(scene_path);

	if (next_scene != nullptr)
		EDITOR_LOG_INFO_F("Scene ���� �ε忡 �����߽��ϴ�: [%s]", scene_name.c_str())
	else
		EDITOR_LOG_ERROR_F("Scene ���� �ε忡 �����߽��ϴ�: [%s]", scene_name.c_str())

	//Change Scene
	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Scene_Change;
	event_struct.object_address_1 = next_scene;

	EVENT_MANAGER->AddEvent(event_struct);
}

#include <DX11_2D_GameEngine_Lib/Prefab.h>
void FileFunction::CreatePrefabResource(DWORD_PTR p_game_object)
{
	GameObject* p_src_game_object = (GameObject*)(p_game_object);
	auto p_prefab = RESOURCE_MANAGER->CreatePrefab(p_src_game_object);
	RESOURCE_MANAGER->SaveToFile<Prefab>(p_prefab, p_prefab->GetResourcePath());
}

void FileFunction::CreatePrefabGameObject(const std::string& prefab_resource_path)
{
	auto prefab_name = FILE_MANAGER->GetOriginFileNameFromPath(prefab_resource_path);
	auto p_prefab = RESOURCE_MANAGER->GetResource<Prefab>(prefab_name);

	//Create Prefab GameObject
	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Create_Object;
	event_struct.object_address_1 = p_prefab->Instantiate(); //Prefab�� GameObject ������ �����Ͽ� ���ο� GameObject �޸� �Ҵ�

	EVENT_MANAGER->AddEvent(event_struct);
}
