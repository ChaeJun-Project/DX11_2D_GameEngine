#pragma once
#include "GUI/IGUI.h"

class GUI_Game final : public IGUI
{
public:
	GUI_Game(const std::string & game_title);
	~GUI_Game();

public:
	void Update() override;
	void Render() override;

private:
	void ShowGame();

private:
	float frame = 0.0f;
};

