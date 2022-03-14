#include "stdafx.h"
#include "Ground_Script.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Collider2D.h>
#include <DX11_2D_GameEngine_Lib/RigidBody2D.h>

Ground_Script::Ground_Script()
	:Script("Ground_Script")
{
}

Ground_Script::Ground_Script(const Ground_Script& origin)
	: Script(origin.m_script_name)
{
}

Ground_Script::~Ground_Script()
{
}

void Ground_Script::OnCollisionEnter(GameObject* other_game_object)
{
	if (other_game_object->GetGameObjectTag() == "Player")
	{
		auto p_transform = m_p_owner_game_object->GetComponent<Transform>();
		auto ground_position = p_transform->GetTranslation();
		
		p_transform = other_game_object->GetComponent<Transform>();
		auto player_position = p_transform->GetTranslation();

		if (!m_is_ground)
		{
			m_is_ground = true;

			m_player_position_y = player_position.y;

			auto p_rigidbody2D = other_game_object->GetComponent<RigidBody2D>();
			p_rigidbody2D->SetGround(m_is_ground);
		}

		auto y = (ground_position.y - player_position.y);

		player_position.y = m_player_position_y;

		p_transform->SetTranslation(player_position);
	}
}

void Ground_Script::OnCollisionStay(GameObject* other_game_object)
{
	if (other_game_object->GetGameObjectTag() == "Player")
	{
		auto p_transform = other_game_object->GetComponent<Transform>();
		auto position = p_transform->GetTranslation();
		if (m_is_ground)
			m_player_position_y = position.y;

		position.y = m_player_position_y;

		p_transform->SetTranslation(position);
	}
}

void Ground_Script::OnCollisionExit(GameObject* other_game_object)
{
	if (other_game_object->GetGameObjectTag() == "Player")
	{
		auto p_rigidbody2D = other_game_object->GetComponent<RigidBody2D>();

		m_is_ground = false;
		m_player_position_y = 0.0f;
		p_rigidbody2D->SetGround(m_is_ground);
	}
}

void Ground_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script
}