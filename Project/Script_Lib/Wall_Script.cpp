#include "stdafx.h"
#include "Wall_Script.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/RigidBody2D.h>

Wall_Script::Wall_Script()
	:Script("Wall_Script")
{
}

Wall_Script::Wall_Script(const Wall_Script& origin)
	: Script(origin.m_script_name)
{
}

Wall_Script::~Wall_Script()
{
}

void Wall_Script::OnCollisionEnter(GameObject* other_game_object)
{
	if (other_game_object->GetGameObjectTag() == "Player")
	{
		auto p_transform = m_p_owner_game_object->GetComponent<Transform>();
		auto wall_position = p_transform->GetTranslation();

		p_transform = other_game_object->GetComponent<Transform>();
		auto player_position = p_transform->GetTranslation();

		if (!m_is_wall)
		{
			m_player_position_x = player_position.x;
			m_is_wall = true;

			auto p_rigidbody2D = other_game_object->GetComponent<RigidBody2D>();

			//플레이어보다 오른쪽에 있는 벽일 경우
			if (wall_position.x > player_position.x)
			{
				m_is_right = true;
				p_rigidbody2D->SetWall(m_is_right, m_is_wall);
			}

			//왼쪽에 있는 벽일 경우
			else if(wall_position.x < player_position.x)
			{
				m_is_right = false;
				p_rigidbody2D->SetWall(m_is_right, m_is_wall);
			}

		}

		player_position.x = m_player_position_x;

		p_transform->SetTranslation(player_position);
	}
}

void Wall_Script::OnCollisionStay(GameObject* other_game_object)
{
	if (other_game_object->GetGameObjectTag() == "Player")
	{
		auto p_transform = other_game_object->GetComponent<Transform>();
		auto position = p_transform->GetTranslation();
		if (m_is_wall)
			m_player_position_x = position.x;

		position.x = m_player_position_x;

		p_transform->SetTranslation(position);
	}
}

void Wall_Script::OnCollisionExit(GameObject* other_game_object)
{
	if (other_game_object->GetGameObjectTag() == "Player")
	{
		auto p_rigidbody2D = other_game_object->GetComponent<RigidBody2D>();

		m_is_wall = false;
		m_player_position_x = 0.0f;
		p_rigidbody2D->SetWall(m_is_right, m_is_wall);
	}
}

void Wall_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script
}
