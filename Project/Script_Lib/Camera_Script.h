#pragma once

#include <DX11_2D_GameEngine_Lib/Script.h>

class Transform;
class Camera;

class Camera_Script final : public Script
{
public:
	Camera_Script();
	explicit Camera_Script(const Camera_Script& origin);
	virtual ~Camera_Script();

	void Awake() override;
	void Update() override;

private:
	void RegisterScriptParamData() override;

public:
    void SetTarget(GameObject* p_player_game_object);

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(Camera_Script);

private:
    //Camera Object
	Transform* m_p_transform = nullptr;
	Camera* m_p_camera = nullptr;
	
	//Target GameObject
	Transform* m_p_target_transform = nullptr;

	//Map Resolution
	Vector2 m_map_resolution = Vector2::Zero;
	//Camera View Resolution
	Vector2 m_view_resolution = Vector2::Zero;
};

