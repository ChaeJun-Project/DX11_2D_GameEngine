#pragma once

class Layer;

class Scene final
{
public:
   Scene() = default;
   ~Scene();

   void Update();
   void LateUpdate();
   void FinalUpdate();

public:
	std::vector<GameObject*>& GetAllGameObjects();
	GameObject* FindGameObjectByName(const std::string& game_object_name);

public:
   void AddGameObject(GameObject* p_game_object, UINT layer_index, bool is_move);

public:
   const std::shared_ptr<Layer>& GetLayer(const UINT& layer_index);
   const std::map<UINT, std::shared_ptr<Layer>> GetLayerMap() const { return this->m_layer_map; }

private:
   std::map<UINT, std::shared_ptr<Layer>> m_layer_map;
};

