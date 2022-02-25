#pragma once
#include <DX11_2D_GameEngine_Lib/Script.h>

class GameManager final : public Script
{
public:
	GameManager();
	explicit GameManager(const GameManager& origin);
	virtual ~GameManager();

	void Initialize() override;
	void Start() override;
	void Update() override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(GameManager);

};

