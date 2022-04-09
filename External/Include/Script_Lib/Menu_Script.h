#pragma once
#include <DX11_2D_GameEngine_Lib/Script.h>
#include "PageController.h"

class ImageRenderer;

class Menu_Script final : public Script, public PageController
{
public:
	Menu_Script();
	explicit Menu_Script(const Menu_Script& origin);
	virtual ~Menu_Script();

public:
	void Awake() override;
	void OnEnable() override;
	void Update() override;

private:
    void LoadContinueButtonTexture();
	void LoadPlayerSelectButtonTexture();

	void SetContinueButtonSelected();
	void SetGameStartButtonSelected();

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override {}

public:
	CLONE(Menu_Script);

private:
#pragma region Button Object ImageRenderer
	ImageRenderer* m_p_continue_image_renderer = nullptr;
	ImageRenderer* m_p_game_start_image_renderer = nullptr;
#pragma endregion

#pragma region Button Image
	std::string m_continue_button_texture_path = "UI/Menu/Continue";
	std::vector<std::shared_ptr<Texture>> m_p_continue_button_textrue_vector; //0: Normal 1: Select

	std::string m_game_start_button_texture_path = "UI/Menu/Game Start";
	std::vector<std::shared_ptr<Texture>> m_p_game_start_button_textrue_vector; //0: Normal 1: Select
#pragma endregion

private:
	enum class CurrentSelectedType : UINT
	{
	    Continue = 0,
		GameStart = 1,
    };

	CurrentSelectedType m_p_current_selected = CurrentSelectedType::GameStart;
};

