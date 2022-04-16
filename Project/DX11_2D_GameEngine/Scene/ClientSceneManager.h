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
	//Recent Scene Name
    static std::string m_recent_scene_name;
};