#pragma once



class ClientSceneManager
{
public: 
     static void CreateNewScene();
     static void SaveScene(const std::string& file_path);
     static Scene* LoadScene(const std::string& file_path);

private:
     static void SaveGameObject(GameObject* p_game_object, FILE* p_file);
     static GameObject* LoadGameObject(FILE* p_file);

public:
     static void CreatePrefab();
};

