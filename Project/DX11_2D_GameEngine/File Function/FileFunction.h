#pragma once

#include <DX11_2D_GameEngine_Lib/FileManager.h>

class FileFunction final
{
public:
    //Save
	static void SaveFile(const std::string& save_resource_folder_path, const std::string& file_name, const FileType& file_type);

	static void SaveScene(const std::string& scene_path);

	template<typename T>
	static void SaveResource(const std::string& resource_path);

	//Load
	static const std::string LoadFile(const std::string& load_resource_folder_path, const FileType& file_type);

	static void LoadScene(const std::string& scene_path);
};

template<typename T>
void FileFunction::SaveResource(const std::string& resource_path)
{
	auto resource_manager = ResourceManager::GetInstance();

	//Ȯ���ڰ� ���Ե��� ���� ���� ������ �̸�
	auto file_name_without_extension = FILE_MANAGER->GetOriginFileNameFromPath(resource_path);
	auto& p_resource = resource_manager->GetResource<T>(file_name_without_extension);

	//Content ����(������Ʈ�� �۾� ���丮 ���)�� ���� �������� ���ҽ� ������ �ִ� ���������� ���(��� ���)
	auto relative_file_path = FILE_MANAGER->GetRelativeResourcePathFromAbsolutePath_1(resource_path);
	p_resource->SetResourcePath(relative_file_path);

	if (p_resource != nullptr)
		resource_manager->SaveToFile<T>(p_resource, resource_path);
}
