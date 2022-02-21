#pragma once
#include <DX11_2D_GameEngine_Lib/Script.h>

class GameManager final : public Script
{
public:
	GameManager();
	~GameManager();

	void Initialize() override;
	void Start() override;
	void Update() override;

public:
	CLONE(GameManager);

private:

};

