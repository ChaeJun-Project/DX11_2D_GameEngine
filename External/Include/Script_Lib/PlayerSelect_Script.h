#pragma once
#include <DX11_2D_GameEngine_Lib/Script.h>
#include "PageController.h"

class GameObject;
class Animator2D;

class PlayerSelect_Script final : public Script, public PageController
{
public:
	PlayerSelect_Script();
	explicit PlayerSelect_Script(const PlayerSelect_Script& origin);
	virtual ~PlayerSelect_Script();

public:
	void Awake() override;
	void OnEnable() override;
	void Update() override;

private:
	void SetXFocus();
	void SetZFocus();

	void SetXSeleted();
	void SetZSeleted();

private:
    void AddAnimationEvent();
	void DisactiveSelf();

	void SetCurrentAnimation
	(
	const std::string& animation_name, 
	const float& animation_speed = 1.0f,
	const bool& is_loop = false, 
	const bool& is_reverse = false
	);

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override {}

public:
	CLONE(PlayerSelect_Script);

private:
#pragma region X
	GameObject* m_p_x_selected_focus = nullptr;
#pragma endregion

#pragma region Z
	GameObject* m_p_z_selected_focus = nullptr;
	Animator2D* m_p_z_preview_animator = nullptr;
#pragma endregion

private:
	enum class CurrentSelectedType : UINT
	{
		X = 0,
		Z = 1,
	};

	CurrentSelectedType m_p_current_selected = CurrentSelectedType::Z;
};

