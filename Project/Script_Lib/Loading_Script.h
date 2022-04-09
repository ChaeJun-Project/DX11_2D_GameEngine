#pragma once
#include <DX11_2D_GameEngine_Lib/Script.h>

class Loading_Script final : public Script
{
public:
	Loading_Script();
	explicit Loading_Script(const Loading_Script& origin);
	virtual ~Loading_Script();

public:
	void Awake() override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(Loading_Script);
};

