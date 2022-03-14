#pragma once

class Layer;

class Scene final : public DX11Obejct
{
public:
   Scene(const std::string& scene_name);
   ~Scene();

   void SetStartScene();

   void Start();
   void Update();
   void FinalUpdate();

public:
   void RegisterGameObject(GameObject* p_game_object);
   void DeregisterGameObject(GameObject* p_game_object, const bool& is_with_layer = false);
   void DeregisterFromParentGameObject(GameObject* p_game_object);

   GameObject* FindGameObject(const std::string& game_object_name);

public:
   const std::string& GetSceneName() { return m_object_name; }
   void SetSceneName(const std::string& scene_name) { m_object_name = scene_name; }

   const std::shared_ptr<Layer>& GetLayer(const UINT& layer_index);
   const std::map<UINT, std::shared_ptr<Layer>>& GetLayerMap() const { return m_layer_map; }

   const std::vector<GameObject*>& GetAllGameObjects() { return m_p_game_object_vector; }
   const std::vector<GameObject*>& GetAllParentGameObjects() { return m_p_parent_game_object_vector; }

private:
	void CreateLayer(const UINT& layer_index);

public:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
   std::map<UINT, std::shared_ptr<Layer>> m_layer_map;

   //부모 자식 관계없이 해당 Scene에 속한 모든 오브젝트들
   std::vector<GameObject*> m_p_game_object_vector;

   //해당 Scene에 속한 최상위 부모 오브젝트들
   std::vector<GameObject*> m_p_parent_game_object_vector;
};