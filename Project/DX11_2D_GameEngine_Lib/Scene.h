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
	std::vector<GameObject*>& GetAllParentGameObjects();
	std::vector<GameObject*>& GetAllGameObjects();
	GameObject* FindGameObjectByName(const std::string& game_object_name);

public:
   void CreateLayer(const UINT& layer_index);
   void AddGameObject(GameObject* p_game_object, UINT layer_index, bool is_move);

public:
   const std::string& GetSceneName() { return m_object_name; }
   void SetSceneName(const std::string& scene_name) { m_object_name = scene_name; }

   const std::shared_ptr<Layer>& GetLayer(const UINT& layer_index);
   const std::map<UINT, std::shared_ptr<Layer>> GetLayerMap() const { return m_layer_map; }

public:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
   std::map<UINT, std::shared_ptr<Layer>> m_layer_map;

   //해당 Scene에 속한 최상위 부모 오브젝트들
   std::vector<GameObject*> m_p_parent_game_object_vector;

   //부모 자식 관계없이 해당 Scene에 속한 모든 오브젝트들
   std::vector<GameObject*> m_p_game_object_vector;
};

