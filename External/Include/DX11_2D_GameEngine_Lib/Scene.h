#pragma once

class Layer;

class Scene final
{
public:
   Scene(const std::string& scene_name);
   ~Scene();

   void Start();
   void Update();
   void LateUpdate();
   void FinalUpdate();

public:
	std::vector<GameObject*>& GetAllParentGameObjects();
	std::vector<GameObject*>& GetAllGameObjects();
	GameObject* FindGameObjectByName(const std::string& game_object_name);

public:
   void AddGameObject(GameObject* p_game_object, UINT layer_index, bool is_move);

public:
   const std::string& GetSceneName() { return m_scene_name; }
   void SetSceneName(const std::string& scene_name) { m_scene_name = scene_name; }

   const std::shared_ptr<Layer>& GetLayer(const UINT& layer_index);
   const std::map<UINT, std::shared_ptr<Layer>> GetLayerMap() const { return this->m_layer_map; }

private:
   std::string m_scene_name;

   std::map<UINT, std::shared_ptr<Layer>> m_layer_map;

   //�ش� Scene�� ���� �ֻ��� �θ� ������Ʈ��
   std::vector<GameObject*> m_p_parent_game_object_vector;

   //�θ� �ڽ� ������� �ش� Scene�� ���� ��� ������Ʈ��
   std::vector<GameObject*> m_p_game_object_vector;
};

