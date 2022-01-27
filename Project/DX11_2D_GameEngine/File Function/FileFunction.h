#pragma once

#include <DX11_2D_GameEngine_Lib/FileManager.h>

class FileFunction final
{
public:
	static void SaveFile(const std::string& save_path, const std::string& file_name, const FileType& file_type);

	static void SaveScene(const std::string& scene_path);

	template<typename T>
	static void SaveResource(const std::string& resource_path);

	static void SaveTileMap(const std::string& tile_map_path);

	static void SaveSpriteAnimation(const std::string& animation2D_path);

	static const std::string LoadFile(const std::string& load_path, const FileType& file_type);

	static void LoadScene(const std::string& scene_path);
};

template<typename T>
void FileFunction::SaveResource(const std::string& resource_path)
{
	auto resource_manager = ResourceManager::GetInstance();

	auto file_name_without_extension = FILE_MANAGER->GetOriginFileNameFromPath(resource_path);
	auto p_resource = resource_manager->GetResource<T>(file_name_without_extension);

	if (p_resource != nullptr)
		resource_manager->SaveToFile<T>(p_resource, resource_path);
}
