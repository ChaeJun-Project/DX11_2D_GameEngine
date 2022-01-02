#pragma once

class Layer;

class Scene final : public DX11Obejct
{
public:
   Scene(const std::string& scene_name);
   ~Scene();

   void Start();
   void Update();
   void FinalUpdate();

public:
   void AddGameObject(GameObject* p_new_game_object);

   GameObject* FindGameObject(const std::string& game_object_name);

public:
   const std::string& GetSceneName() { return m_object_name; }
   void SetSceneName(const std::string& scene_name) { m_object_name = scene_name; }

   const std::shared_ptr<Layer>& GetLayer(const UINT& layer_index);
   const std::map<UINT, std::shared_ptr<Layer>>& GetLayerMap() const { return m_layer_map; }

   const std::vector<GameObject*>& GetAllParentGameObjects();
   const std::vector<GameObject*>& GetAllGameObjects();

private:
	void CreateLayer(const UINT& layer_index);

public:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
   std::map<UINT, std::shared_ptr<Layer>> m_layer_map;

   //�ش� Scene�� ���� �ֻ��� �θ� ������Ʈ��
   std::vector<GameObject*> m_p_parent_game_object_vector;

   //�θ� �ڽ� ������� �ش� Scene�� ���� ��� ������Ʈ��
   std::vector<GameObject*> m_p_game_object_vector;
};

