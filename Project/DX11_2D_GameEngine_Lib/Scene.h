#pragma once

class IObejct;

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
   std::list<std::shared_ptr<IObject>> m_object_list;
};

