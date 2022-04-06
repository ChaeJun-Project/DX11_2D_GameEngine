#include "stdafx.h"
#include "Wall_Script.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Collider2D.h>
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
	m_p_transform = nullptr;
	m_p_collider2D = nullptr;
}

void Wall_Script::Awake()
{
	m_p_transform = m_p_owner_game_object->GetComponent<Transform>();
	m_p_collider2D = m_p_owner_game_object->GetComponent<Collider2D>();
}

void Wall_Script::PlayerCollisionEnter(GameObject* p_player_game_object)
{
	//Wall
	auto wall_position = m_p_transform->GetTranslation();
	auto wall_scale_x = m_p_transform->GetScale().x;

	auto wall_collider2D_scale_x = m_p_collider2D->GetOffsetScale().x;
	wall_collider2D_scale_x *= 0.5f;
	auto wall_collider2D_position_x = m_p_collider2D->GetOffsetPosition().x;
	wall_collider2D_position_x *= -1.0f;

	//Player
	auto p_player_transform = p_player_game_object->GetComponent<Transform>();
	auto player_position = p_player_transform->GetTranslation();
	auto player_scale_x = p_player_transform->GetScale().x;;

	auto p_player_collider2D = p_player_game_object->GetComponent<Collider2D>();
	auto player_collider2D_scale_x = p_player_collider2D->GetOffsetScale().x;
	player_collider2D_scale_x *= 0.5f;
	auto player_collider2D_position_x = p_player_collider2D->GetOffsetPosition().x;
	player_collider2D_position_x *= -1.0f;

	if (!m_is_wall)
	{
		m_is_wall = true;

		auto p_player_rigidbody2D = p_player_game_object->GetComponent<RigidBody2D>();

		//플레이어보다 오른쪽에 있는 벽일 경우
		if (wall_position.x > player_position.x)
		{
			m_is_right = true;
			p_player_rigidbody2D->SetWall(m_is_right, m_is_wall);

			auto wall_position_left_x = wall_position.x - 100.0f * (wall_collider2D_scale_x + wall_collider2D_position_x) * wall_scale_x;
			m_player_position_x = wall_position_left_x - 100.0f * (player_collider2D_scale_x + player_collider2D_position_x) * player_scale_x;
		}

		//왼쪽에 있는 벽일 경우
		else if (wall_position.x < player_position.x)
		{
			m_is_right = false;
			p_player_rigidbody2D->SetWall(m_is_right, m_is_wall);

			auto wall_position_right_x = wall_position.x + 100.0f * (wall_collider2D_scale_x + wall_collider2D_position_x)* wall_scale_x;
			m_player_position_x = wall_position_right_x + 100.0f * (player_collider2D_scale_x + player_collider2D_position_x) * player_scale_x;
		}

		p_player_rigidbody2D->SetVelocity(Vector2(0.0f, p_player_rigidbody2D->GetVelocity().y));
	}

	player_position.x = m_player_position_x;

	p_player_transform->SetTranslation(player_position);
}

void Wall_Script::PlayerCollisionStay(GameObject* p_player_game_object)
{
	//Player
	auto p_player_transform = p_player_game_object->GetComponent<Transform>();
	auto player_position = p_player_transform->GetTranslation();

	auto p_player_rigidbody2D = p_player_game_object->GetComponent<RigidBody2D>();

	if (m_is_wall)
		p_player_rigidbody2D->SetWall(m_is_right, m_is_wall);
	
	player_position.x = m_player_position_x;

	p_player_transform->SetTranslation(player_position);
}

void Wall_Script::PlayerCollisionExit(GameObject* p_player_game_object)
{
	auto p_player_rigidbody2D = p_player_game_object->GetComponent<RigidBody2D>();

	m_is_wall = false;
	m_player_position_x = 0.0f;
	p_player_rigidbody2D->SetWall(m_is_right, m_is_wall);
}

void Wall_Script::OnCollisionEnter(GameObject* p_other_game_object)
{
	if (p_other_game_object->GetGameObjectTag() == "Player")
		PlayerCollisionEnter(p_other_game_object);
}

void Wall_Script::OnCollisionStay(GameObject* p_other_game_object)
{
	if (p_other_game_object->GetGameObjectTag() == "Player")
		PlayerCollisionStay(p_other_game_object);
}

void Wall_Script::OnCollisionExit(GameObject* p_other_game_object)
{
	if (p_other_game_object->GetGameObjectTag() == "Player")
		PlayerCollisionExit(p_other_game_object);
}

void Wall_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script
}
