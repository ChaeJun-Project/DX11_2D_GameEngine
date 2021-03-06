#include "stdafx.h"
#include "EditorObjectManager.h"

#include "EditorObject/GameObject/GameObjectEx.h"
#include "EditorObject/Camera/CameraEx.h"

#include <DX11_2D_GameEngine_Lib/RenderManager.h>
#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/Transform.h>

EditorObjectManager::~EditorObjectManager()
{
   for(auto& game_object_ex : m_game_object_ex_vector)
      SAFE_DELETE(game_object_ex);

   m_game_object_ex_vector.clear();
   m_game_object_ex_vector.shrink_to_fit();
}

void EditorObjectManager::Initialize()
{
    //Create Editor Camera 
    auto editor_camera = new GameObjectEx();
    editor_camera->AddComponent(ComponentType::Transform);
    editor_camera->AddComponent(new CameraEx());

    editor_camera->GetComponent<Transform>()->SetTranslation(Vector3(0.0f, 0.0f, -1000.0f));

    auto camera = editor_camera->GetComponent<Camera>();

    camera->SetProjectionType(ProjectionType::Perspective);
    camera->CullingNothing();

    RENDER_MANAGER->RegisterEditorCamera(camera);

    m_game_object_ex_vector.emplace_back(editor_camera);
}

void EditorObjectManager::Update()
{
    if (SCENE_MANAGER->GetEditorState() == EditorState::EditorState_Stop)
    {
        for (auto& game_object : m_game_object_ex_vector)
            game_object->Update();

        for (auto& game_object : m_game_object_ex_vector)
            game_object->FinalUpdate();
    }
}

void EditorObjectManager::Render()
{
}
