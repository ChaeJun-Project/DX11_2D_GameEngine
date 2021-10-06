#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"

SceneManager::SceneManager()
{
    Initialize();
}

SceneManager::~SceneManager()
{
    m_p_current_scene.reset();
}

void SceneManager::Initialize()
{
    m_p_current_scene = std::make_shared<Scene>();
    m_p_current_scene->Initialize();
}

void SceneManager::Progress()
{
    TimeManager::GetInstance()->Update();
    InputManager::GetInstance()->Update();

    m_p_current_scene->Update();
    m_p_current_scene->LateUpdate();
    m_p_current_scene->FinalUpdate();

    m_p_current_scene->Render();
}
