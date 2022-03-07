#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>

class Camera_Script final : public Script
{
public:
	Camera_Script();
	explicit Camera_Script(const Camera_Script& origin);
	virtual ~Camera_Script();

	void Initialize() override;
	void Start() override;
	void Update() override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(Camera_Script);
};

