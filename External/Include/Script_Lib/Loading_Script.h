#pragma once
#include <DX11_2D_GameEngine_Lib/Script.h>
#include "PageController.h"

class Loading_Script final : public Script, public PageController
{
public:
	Loading_Script();
	explicit Loading_Script(const Loading_Script& origin);
	virtual ~Loading_Script() = default;

public:
    void Start() override;

private:
    void LoadScene();

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override {}

public:
	CLONE(Loading_Script);
};

