#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>
#include "UIAnimationController.h"

class Texture;
class ImageRenderer;
class AudioSource;

class Warning_Script final : public Script, public UIAnimationController
{
public:
	Warning_Script();
	explicit Warning_Script(const Warning_Script& origin);
	~Warning_Script() = default;

	void Awake() override;
	void OnEnable() override;
	void OnDisable() override;
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
	CLONE(Warning_Script);
};
