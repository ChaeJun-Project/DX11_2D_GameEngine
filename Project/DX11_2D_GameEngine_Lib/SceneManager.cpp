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

void SceneManager::Start()
{
    if(m_client_state == 1)
		m_p_current_scene->Start();
} 

void SceneManager::Update()
{
	//<summary>
	//ClientState
	//Game = 1
	//Editor = 2
	//</summary>

	//Editor State ��Ʈ���� Play�� ���ԵǾ��ְ� Pause�� �����ԵǾ��ִ� ��� �Ǵ� ���Ӹ���� ���
	if (((m_editor_state & EditorState::EditorState_Play) && !(m_editor_state & EditorState::EditorState_Pause))
		|| m_client_state == 1)
	{
		m_p_current_scene->Update();
	}

	m_p_current_scene->FinalUpdate();

	//Editor State ��Ʈ���� Play�� ���ԵǾ��ְ� Pause�� �����ԵǾ��ִ� ��� �Ǵ� ���Ӹ���� ���
	if (((m_editor_state & EditorState::EditorState_Play) && !(m_editor_state & EditorState::EditorState_Pause))
		|| m_client_state == 1)
		//Update Collisio Manager
		COLLISION_MANAGER->Update();
}

void SceneManager::SetCurrentScene(const std::shared_ptr<Scene>& p_current_scene)
{
	if (m_p_current_scene != nullptr)
		m_p_current_scene.reset();

	m_p_current_scene = p_current_scene;

	Start();
}

void SceneManager::SetEditorState(const UINT& editor_state)
{
	//�������·� �����Ѵٸ�
	if (editor_state == EditorState::EditorState_Stop)
	{
		m_editor_state = editor_state;
		m_p_current_scene->Initialize();
	}

	else
	{
		//�̹� ���� ����� ���¶��
		if (m_editor_state & editor_state)
		{
			if (m_editor_state & EditorState::EditorState_Play &&
				editor_state == EditorState::EditorState_Play)
			{
				m_editor_state = EditorState::EditorState_Stop;
				m_p_current_scene->Initialize();
			}

			m_editor_state &= ~editor_state; //�ش� ���� ����
		}

		//���� ����� ���°� �ƴ϶�� �߰�
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