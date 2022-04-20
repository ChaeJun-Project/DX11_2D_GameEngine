#pragma once

class ClientSceneManager final
{
public:
    static void Initialize();
	static void InitializeScene();

public:
	static void CreateNewScene();
	static bool SaveScene(const std::string& file_path);

public:
	static std::shared_ptr<Scene> LoadScene(const std::string& file_path);

public:
	static void InitializeRecentScene();
	static void LoadRecentScene();
	static void SaveRecentScene();

public:
	//Recent Scene Name
    static std::string m_recent_scene_name;
	static std::string m_folder_path;
	static std::string m_recent_scene_file_path;
};

std::string ClientSceneManager::m_folder_path = "Engine/";
std::string ClientSceneManager::m_recent_scene_file_path = "RecentScene.txt";