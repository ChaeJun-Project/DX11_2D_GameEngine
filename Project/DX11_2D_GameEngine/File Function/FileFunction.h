#pragma once

class FileFunction final
{
public:
	static void SaveFile(const std::string& save_path, const std::string& file_name, const FileType& file_type);

	static void SaveScene(const std::string& scene_path);

	static void SaveTileMap(const std::string& tile_map_path);

	static void SaveAnimation2D(const std::string& animation2D_path);

	static const std::string LoadFile(const std::string& load_path, const FileType& file_type);

	static void LoadScene(const std::string& scene_path);
};