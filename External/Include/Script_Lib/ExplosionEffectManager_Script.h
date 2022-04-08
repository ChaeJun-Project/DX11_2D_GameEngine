#pragma once
#include <DX11_2D_GameEngine_Lib/Script.h>

struct ExplosionInfo final
{
	ExplosionInfo(GameObject* p_explosion_game_object, Transform* p_explosion_transform)
		:m_p_explosion_game_object(p_explosion_game_object),
		m_p_explosion_transform(p_explosion_transform)
	{
	}

	GameObject* m_p_explosion_game_object = nullptr;
	Transform* m_p_explosion_transform = nullptr;
};

class GameObject;
class AudioSource;

class ExplosionEffectManager_Script final : public Script
{
public:
	ExplosionEffectManager_Script();
	explicit ExplosionEffectManager_Script(const ExplosionEffectManager_Script& origin);
	virtual ~ExplosionEffectManager_Script();

public:
	void Awake() override;
	void OnEnable() override;
	void Update() override;

private:
	void RegisterScriptParamData() override;

private:
	const Vector3 CreateRandomPosition();

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

public:
	CLONE(ExplosionEffectManager_Script);

private:
	AudioSource* m_p_audio_source = nullptr;

	Vector2 m_min_spawn_position = Vector2::Zero;
	Vector2 m_max_spawn_position = Vector2::Zero;

	std::vector<ExplosionInfo> m_p_explosion_game_object_vector;

	float m_explosion_active_cooltime = 0.0f;
	float m_accumulate_time = 0.0f;
};

