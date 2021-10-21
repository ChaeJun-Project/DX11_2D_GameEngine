#pragma once

class Layer;

class Scene final
{
public:
   Scene() = default;
   ~Scene();

   void Initialize();

   void Update();
   void LateUpdate();
   void FinalUpdate();
   void Render();

public:
   void AddGameObject(GameObject* p_game_object, UINT layer_index, bool is_move);

public:
   const std::shared_ptr<Layer>& GetLayer(const UINT& layer_index);

private:
   std::map<UINT, std::shared_ptr<Layer>> m_layer_map;
};

