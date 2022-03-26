#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>
#include "UIAnimationController.h"

class Texture;
class ImageRenderer;
class AudioSource;

class Ready_Script final : public Script, public UIAnimationController
{
public:
	Ready_Script();
	explicit Ready_Script(const Ready_Script& origin);
	~Ready_Script() = default;

	void Start() override;
	void Update() override;

public:
    void Play();

private:
	void RegisterScriptParamData() override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(Ready_Script);
};

