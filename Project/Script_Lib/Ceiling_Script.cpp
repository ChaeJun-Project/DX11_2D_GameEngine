#include "stdafx.h"
#include "Ceiling_Script.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/RigidBody2D.h>

Ceiling_Script::Ceiling_Script()
	:Script("Ceiling_Script")
{
}

Ceiling_Script::Ceiling_Script(const Ceiling_Script& origin)
	: Script(origin.m_script_name)
{
}

Ceiling_Script::~Ceiling_Script()
{
}

void Ceiling_Script::PlayerCollisionEnter(GameObject* p_player_game_object)
{
	auto p_player_transform = p_player_game_object->GetComponent<Transform>();
	auto player_position = p_player_transform->GetTranslation();

	if (!m_is_ceiling)
	{
		m_is_ceiling = true;

		auto p_player_rigidbody2D = p_player_game_object->GetComponent<RigidBody2D>();
		p_player_rigidbody2D->SetCeiling(m_is_ceiling);
		p_player_rigidbody2D->SetVelocity(Vector2(p_player_rigidbody2D->GetVelocity().x, 0.0f));

		m_player_position_y = player_position.y;
	}

	player_position.y = m_player_position_y;

	p_player_transform->SetTranslation(player_position);
}

void Ceiling_Script::PlayerCollisionStay(GameObject* p_player_game_object)
{
	auto p_player_rigidbody2D = p_player_game_object->GetComponent<RigidBody2D>();

	m_is_ceiling = false;
	m_player_position_y = 0.0f;
	p_player_rigidbody2D->SetCeiling(m_is_ceiling);
}

void Ceiling_Script::OnCollisionEnter(GameObject* p_other_game_object)
{
	if (p_other_game_object->GetGameObjectTag() == "Player")
		PlayerCollisionEnter(p_other_game_object);
}

void Ceiling_Script::OnCollisionStay(GameObject* p_other_game_object)
{
	if (p_other_game_object->GetGameObjectTag() == "Player")
		PlayerCollisionStay(p_other_game_object);
}

void Ceiling_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script
}