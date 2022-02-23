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

public:
	CLONE(GameManager);

private:
	virtual void SaveToScene(FILE* p_file) override;
	virtual void LoadFromScene(FILE* p_file) override;

};

