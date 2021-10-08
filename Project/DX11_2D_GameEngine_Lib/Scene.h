#pragma once

class GameObject;

class Scene final
{
public:
   Scene();
   ~Scene();

   void Initialize();

   void Update();
   void LateUpdate();
   void FinalUpdate();
   void Render();

private:
   std::list<std::shared_ptr<GameObject>> m_object_list;
};

