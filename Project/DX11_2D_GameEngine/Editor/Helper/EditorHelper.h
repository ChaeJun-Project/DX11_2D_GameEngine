#pragma once

#include <DX11_2D_GameEngine_Lib/Singleton.h>

class GameObject;

class EditorHelper final : public Singleton<EditorHelper>
{
	SINGLETON(EditorHelper);

	EditorHelper() = default;
	~EditorHelper();

public:
	GameObject* GetSelectedGameObject() const { SAFE_GET_POINTER(m_selected_game_object); }
	void SetSelectedGameObject(GameObject* selected_game_object)
	{
		m_selected_game_object = selected_game_object;
	}

private:
	GameObject* m_selected_game_object = nullptr;
};

