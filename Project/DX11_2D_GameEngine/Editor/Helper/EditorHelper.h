#pragma once

#include <DX11_2D_GameEngine_Lib/Singleton.h>

class GameObject;
class IResource;

class EditorHelper final : public Singleton<EditorHelper>
{
	SINGLETON(EditorHelper);

	EditorHelper() = default;
	~EditorHelper()
	{
		m_p_selected_game_object = nullptr;
		m_p_select_resource= nullptr;
	}

public:
	GameObject* GetSelectedGameObject() const { SAFE_GET_POINTER(m_p_selected_game_object); }
	void SetSelectedGameObject(GameObject* p_selected_game_object)
	{
		m_p_select_resource = nullptr;
		m_p_selected_game_object = p_selected_game_object;
	}

	IResource* GetSelectedResource() const { return m_p_select_resource; }
	void SetSelectedResource(IResource* p_selected_resource)
	{
		m_p_selected_game_object = nullptr;
		m_p_select_resource = p_selected_resource;
	}

private:
	GameObject* m_p_selected_game_object = nullptr;
	IResource* m_p_select_resource = nullptr;
};

