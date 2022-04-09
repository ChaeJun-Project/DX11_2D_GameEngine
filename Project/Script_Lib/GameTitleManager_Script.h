#pragma once
#include <DX11_2D_GameEngine_Lib/Script.h>

class GameObject;
class Menu_Script;
class PlayerSelect_Script;
class Loading_Script;

class GameTitleManager_Script final : public Script
{
public:
	GameTitleManager_Script();
	explicit GameTitleManager_Script(const GameTitleManager_Script& origin);
	virtual ~GameTitleManager_Script();

public:
	void Start() override;

private:
    void ActiveMenuPage();
	void ActivePlayerSelectPage();
	void ActiveLoadingPage();

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(GameTitleManager_Script);

private:
#pragma region Menu
	GameObject* m_p_menu_page = nullptr;
	Menu_Script* m_p_menu_script = nullptr;
#pragma endregion
	
#pragma region Player Select
	GameObject* m_p_player_select_page = nullptr;
	PlayerSelect_Script* m_p_player_select_script = nullptr;
#pragma endregion

#pragma region Loading
	GameObject* m_p_loading_page = nullptr;
	Loading_Script* m_p_loading_script = nullptr;
#pragma endregion

	GameObject* m_p_current_page = nullptr;
};

