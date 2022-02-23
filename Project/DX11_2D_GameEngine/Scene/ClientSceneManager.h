#pragma once

class GameObject;

class ClientSceneManager final
{
public:
    static void Initialize();

public:
	static void CreateNewScene();
	static std::shared_ptr<Scene> SaveScene(const std::string& file_path);
private:
	static void SaveGameObject(GameObject* p_game_object, FILE* p_file);
	static void SaveScript(GameObject* p_game_object, FILE* p_file);

public:
	static std::shared_ptr<Scene> LoadScene(const std::string& file_path);
private:
	static GameObject* LoadGameObject(FILE* p_file);
	static void LoadScript(GameObject* p_game_object, FILE* p_file);
};

