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

void Ceiling_Script::OnCollisionEnter(GameObject* other_game_object)
{
	if (other_game_object->GetGameObjectTag() == "Player")
	{
		auto p_transform = other_game_object->GetComponent<Transform>();
		auto position = p_transform->GetTranslation();

		if (!m_is_ceiling)
		{
			m_player_position_y = position.y;
			m_is_ceiling = true;

			auto p_rigidbody2D = other_game_object->GetComponent<RigidBody2D>();
			p_rigidbody2D->SetCeiling(m_is_ceiling);
		}

		position.y = m_player_position_y;

		p_transform->SetTranslation(position);
	}
}

void Ceiling_Script::OnCollisionStay(GameObject* other_game_object)
{
	if (other_game_object->GetGameObjectTag() == "Player")
	{
		auto p_rigidbody2D = other_game_object->GetComponent<RigidBody2D>();

		m_is_ceiling = false;
		m_player_position_y = 0.0f;
		p_rigidbody2D->SetCeiling(m_is_ceiling);
		p_rigidbody2D->SetVelocity(Vector2(p_rigidbody2D->GetVelocity().x, -10.0f));
	}
}

void Ceiling_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script
}