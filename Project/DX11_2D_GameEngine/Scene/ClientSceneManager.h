#pragma once

class GameObject;

class ClientSceneManager final
{
public:
    static void Initialize();
	static void InitializeScene();

public:
	static void CreateNewScene();
	static bool SaveScene(const std::string& file_path);

private:
	static void SaveGameObject(GameObject* p_game_object, FILE* p_file);
	static void SaveScript(GameObject* p_game_object, FILE* p_file);

public:
	static std::shared_ptr<Scene> LoadScene(const std::string& file_path);
private:
	static GameObject* LoadGameObject(FILE* p_file);
	static void LoadScript(GameObject* p_game_object, FILE* p_file);

public:
	//Recent Scene Name
    static std::string m_recent_scene_name;
};

std::string ClientSceneManager::m_recent_scene_name;

