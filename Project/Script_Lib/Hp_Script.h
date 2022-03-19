#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>

class GameObject;
class ImageRenderer;

class GameObjectController;

class Hp_Script final : public Script
{
public:
	Hp_Script();
	explicit Hp_Script(const Hp_Script& origin);
	virtual ~Hp_Script();

	void Start() override;
	void Update() override;

private:
	void RegisterScriptParamData() override;

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(Hp_Script);

private:
	ImageRenderer* p_image_renderer = nullptr;

    std::string m_game_object_name;
	std::string m_script_name;
	GameObjectController* m_p_game_object_contorller = nullptr;

	UINT m_game_object_total_hp = 0;
	UINT m_game_object_current_hp = 0;
};

