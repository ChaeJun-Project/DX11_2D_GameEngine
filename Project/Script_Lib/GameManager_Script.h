#pragma once
#include <DX11_2D_GameEngine_Lib/Script.h>

class GameManager_Script final : public Script
{
public:
	GameManager_Script();
	explicit GameManager_Script(const GameManager_Script& origin);
	virtual ~GameManager_Script();

	void Start() override;
	void Update() override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(GameManager_Script);

};

