#pragma once

#include <DX11_2D_GameEngine_Lib/FileManager.h>

class FileFunction final
{
public:
    //Save
	static bool SaveFile(const std::string& save_resource_folder_path, const std::string& file_name, const FileType& file_type);

	static void SaveScene(const std::string& scene_path);

	template<typename T>
	static void SaveResource(const std::string& resource_path);

	//Load
	static const std::string LoadFile(const std::string& load_resource_folder_path, const FileType& file_type);

	static void LoadScene(const std::string& scene_path);

	//Prefab
	static void CreatePrefabResource(DWORD_PTR p_game_object);
	static void CreatePrefabGameObject(const std::string& prefab_resource_path);

	//Engine Data
	static void LoadPhysics(const std::string& physics_path);
	static void LoadGameResolution(const std::string& resolution_path);
};

template<typename T>
void FileFunction::SaveResource(const std::string& resource_path)
{
	//Ȯ���ڰ� ���Ե��� ���� ���� ������ �̸�
	auto file_name_without_extension = FILE_MANAGER->GetOriginFileNameFromPath(resource_path);
	auto& p_resource = RESOURCE_MANAGER->GetResource<T>(file_name_without_extension);

	//Content ����(������Ʈ�� �۾� ���丮 ���)�� ���� �������� ���ҽ� ������ �ִ� ���������� ���(��� ���)
	auto relative_file_path = resource_path;
	FILE_MANAGER->ReplaceAll(relative_file_path, "\\", "/");
	relative_file_path = FILE_MANAGER->GetRelativeResourcePathFromAbsolutePath(relative_file_path);
	p_resource->SetResourcePath(relative_file_path);

	if (p_resource != nullptr)
		RESOURCE_MANAGER->SaveToFile<T>(p_resource, relative_file_path);
}
