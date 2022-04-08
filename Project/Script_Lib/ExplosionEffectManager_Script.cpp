#include "stdafx.h"
#include "ExplosionEffectManager_Script.h"

#include <random>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/AudioSource.h>

ExplosionEffectManager_Script::ExplosionEffectManager_Script()
	:Script("ExplosionEffectManager_Script")
{
	RegisterScriptParamData();
}

ExplosionEffectManager_Script::ExplosionEffectManager_Script(const ExplosionEffectManager_Script& origin)
	: Script(origin.m_script_name)
{
	RegisterScriptParamData();

	m_is_active = origin.m_is_active;

	m_min_spawn_position = origin.m_min_spawn_position;
	m_max_spawn_position = origin.m_max_spawn_position;

	m_explosion_active_cooltime = origin.m_explosion_active_cooltime;
}

ExplosionEffectManager_Script::~ExplosionEffectManager_Script()
{
	m_p_audio_source = nullptr;

	m_p_explosion_game_object_vector.clear();
	m_p_explosion_game_object_vector.shrink_to_fit();
}

void ExplosionEffectManager_Script::Awake()
{
	m_p_audio_source = m_p_owner_game_object->GetComponent<AudioSource>();

	const auto p_children_game_object_vector = m_p_owner_game_object->GetChilds();

	for (const auto& p_child_game_object : p_children_game_object_vector)
	{ 
		auto p_transform = p_child_game_object->GetComponent<Transform>();
		m_p_explosion_game_object_vector.emplace_back(ExplosionInfo(p_child_game_object, p_transform));
	}
}

void ExplosionEffectManager_Script::OnEnable()
{
	m_p_audio_source->Play();
}

void ExplosionEffectManager_Script::Update()
{
	if (m_p_explosion_game_object_vector.empty())
		return;

	m_accumulate_time += DELTA_TIME_F;
	if (m_accumulate_time >= m_explosion_active_cooltime)
	{
		m_accumulate_time = 0.0f;
		for (const auto& m_p_explosion_game_object : m_p_explosion_game_object_vector)
		{
			if (!m_p_explosion_game_object.m_p_explosion_game_object->GetIsActive())
			{
				m_p_explosion_game_object.m_p_explosion_game_object->SetIsActive(true);
				m_p_explosion_game_object.m_p_explosion_transform->SetLocalTranslation(CreateRandomPosition());
				return;
			}
	    }
	}
}

void ExplosionEffectManager_Script::RegisterScriptParamData()
{
	AddScriptParamData(ScriptParamStruct("Min Spawn", ScriptParamType::Vector2, reinterpret_cast<void*>(&m_min_spawn_position), 100.0f));
	AddScriptParamData(ScriptParamStruct("Max Spawn", ScriptParamType::Vector2, reinterpret_cast<void*>(&m_max_spawn_position), 100.0f));

	AddScriptParamData(ScriptParamStruct("Active Cooltime", ScriptParamType::Float, reinterpret_cast<void*>(&m_explosion_active_cooltime), 100.0f));
}

const Vector3 ExplosionEffectManager_Script::CreateRandomPosition()
{
	std::random_device random_device;
	std::mt19937_64 radom_index_gen(random_device());
	std::uniform_real_distribution<float> dis_x(m_min_spawn_position.x, m_max_spawn_position.x);
	std::uniform_real_distribution<float> dis_y(m_min_spawn_position.y, m_max_spawn_position.y);
	
	return Vector3(dis_x(radom_index_gen), dis_y(radom_index_gen), 0.0f);
}

void ExplosionEffectManager_Script::SaveToScene(FILE* p_file)
{
	Script::SaveToScene(p_file); //Script

	//Min Spawn Position
	fprintf(p_file, "[Min Spawn Position]\n");
	FILE_MANAGER->FPrintf_Vector2(m_min_spawn_position, p_file);

	//Max Spawn Position
	fprintf(p_file, "[Max Spawn Position]\n");
	FILE_MANAGER->FPrintf_Vector2(m_max_spawn_position, p_file);

	//Active Cooltime
	fprintf(p_file, "[Explosion Active Cooltime]\n");
	fprintf(p_file, "%f\n", m_explosion_active_cooltime);
}

void ExplosionEffectManager_Script::LoadFromScene(FILE* p_file)
{
	char char_buffer[256] = { 0 };

	//Min Spawn Position
	FILE_MANAGER->FScanf(char_buffer, p_file); //[Min Spawn Position]
	FILE_MANAGER->FScanf_Vector2(m_min_spawn_position, p_file);

	//Max Spawn Position
	FILE_MANAGER->FScanf(char_buffer, p_file); //[Max Spawn Position]
	FILE_MANAGER->FScanf_Vector2(m_max_spawn_position, p_file);

	//Active Cooltime
	FILE_MANAGER->FScanf(char_buffer, p_file); //[Explosion Active Cooltime]
	fscanf_s(p_file, "%f\n", &m_explosion_active_cooltime);
}
