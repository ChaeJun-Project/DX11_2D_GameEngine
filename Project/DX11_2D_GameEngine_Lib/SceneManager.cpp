#include "stdafx.h"
#include "SceneManager.h"

#include "RenderManager.h"

#include "Scene.h"
#include "GameObject.h"

#include "Transform.h"
#include "Camera.h"
#include "Animator2D.h"
#include "ParticleSystem.h"
#include "TileMapRenderer.h"

SceneManager::~SceneManager()
{
	m_p_current_scene.reset();
}

void SceneManager::Initialize()
{
	
}

void SceneManager::Update()
{
	//<summary>
	//ClientState
	//Game = 1
	//Editor = 2
	//</summary>
	
	//Editor State 비트값에 Play가 포함되어있고 Pause가 비포함되어있는 경우 또는 게임모드인 경우
	if ( ((m_editor_state & EditorState::EditorState_Play) && !(m_editor_state & EditorState::EditorState_Pause))
	|| m_client_state == 1 )
	{
		m_p_current_scene->Update();
	}

	m_p_current_scene->FinalUpdate();

	//Editor State 비트값에 Play가 포함되어있고 Pause가 비포함되어있는 경우 또는 게임모드인 경우
	if (((m_editor_state & EditorState::EditorState_Play) && !(m_editor_state & EditorState::EditorState_Pause))
		|| m_client_state == 1)
		//Update Collisio Manager
		CollisionManager::GetInstance()->Update();
}

void SceneManager::CreatePrefab(GameObject* p_game_object)
{
	p_game_object->RegisterPrefab();
}

void SceneManager::SetCurrentScene(const std::shared_ptr<Scene>& p_current_scene)
{
   if(m_p_current_scene != nullptr)
	   m_p_current_scene.reset();

   m_p_current_scene = p_current_scene;
}

void SceneManager::SetEditorState(const UINT& editor_state)
{
	//정지상태로 변경한다면
	if (editor_state == EditorState::EditorState_Stop)
	{
		m_editor_state = editor_state;
	}

	else
	{
		//이미 현재 적용된 상태라면
		if (m_editor_state & editor_state)
		{
			m_editor_state &= ~editor_state; //해당 상태 제거
		}

		//현재 적용된 상태가 아니라면 추가
		else
		{
			if (m_editor_state == EditorState::EditorState_Stop &&
				editor_state == EditorState::EditorState_Play)
			{
				m_p_current_scene->Start();
			}

			m_editor_state |= editor_state;
		}
	}
}