#pragma once

#include <DX11_2D_GameEngine_Lib/Singleton.h>

class GameObjectEx;

class EditorObjectManager final : public Singleton<EditorObjectManager>
{
	SINGLETON(EditorObjectManager);

	EditorObjectManager() = default;
	~EditorObjectManager();

public:
	void Initialize();
	void Update();
	void Render();

private:
    std::vector<GameObjectEx*> m_game_object_ex_vector;
};

