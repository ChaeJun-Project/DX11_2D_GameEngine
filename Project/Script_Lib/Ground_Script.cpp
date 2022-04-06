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
	m_p_transform = nullptr;
	m_p_collider2D = nullptr;
}

void Ground_Script::Awake()
{
	m_p_transform = m_p_owner_game_object->GetComponent<Transform>();
	m_p_collider2D = m_p_owner_game_object->GetComponent<Collider2D>();
}

void Ground_Script::PlayerCollisionEnter(GameObject* p_player_game_object)
{
	//Ground
	auto ground_position_y = m_p_transform->GetTranslation().y;
	auto ground_scale_y = m_p_transform->GetScale().y;

	auto ground_collider2D_scale_y = m_p_collider2D->GetOffsetScale().y;
	ground_collider2D_scale_y *= 0.5f;
	auto ground_collider2D_position_y = m_p_collider2D->GetOffsetPosition().y;
	ground_collider2D_position_y *= -1.0f;

	//Player
	auto p_player_transform = p_player_game_object->GetComponent<Transform>();
	auto player_position = p_player_transform->GetTranslation();
	auto player_scale_y = m_p_transform->GetScale().y;

	auto p_player_collider2D = p_player_game_object->GetComponent<Collider2D>();
	auto player_collider2D_scale_y = p_player_collider2D->GetOffsetScale().y;
	player_collider2D_scale_y *= 0.5f;
	auto player_collider2D_position_y = p_player_collider2D->GetOffsetPosition().y;	
	player_collider2D_position_y *= -1.0f;

	if (!m_is_ground)
	{
		m_is_ground = true;

		auto p_player_rigidbody2D = p_player_game_object->GetComponent<RigidBody2D>();
		p_player_rigidbody2D->SetGround(m_is_ground);
		p_player_rigidbody2D->SetVelocity(Vector2(p_player_rigidbody2D->GetVelocity().x, 0.0f));

		auto ground_position_top_y = ground_position_y + 100.0f * (ground_collider2D_scale_y + ground_collider2D_position_y) * ground_scale_y;
		m_player_position_y = ground_position_top_y + 100.0f * (player_collider2D_scale_y + player_collider2D_position_y) * player_scale_y;
	}

	player_position.y = m_player_position_y;

	p_player_transform->SetTranslation(player_position);
}

void Ground_Script::PlayerCollisionStay(GameObject* p_player_game_object)
{
	auto p_player_transform = p_player_game_object->GetComponent<Transform>();
	auto player_position = p_player_transform->GetTranslation();

	player_position.y = m_player_position_y;
	p_player_transform->SetTranslation(player_position);
}

void Ground_Script::PlayerCollisionExit(GameObject* p_player_game_object)
{
	auto p_player_rigidbody2D = p_player_game_object->GetComponent<RigidBody2D>();

	m_is_ground = false;
	m_player_position_y = 0.0f;
	p_player_rigidbody2D->SetGround(m_is_ground);
}

void Ground_Script::OnCollisionEnter(GameObject* p_other_game_object)
{
	if (p_other_game_object->GetGameObjectTag() == "Player")
		PlayerCollisionEnter(p_other_game_object);
}

void Ground_Script::OnCollisionStay(GameObject* p_other_game_object)
{
	if (p_other_game_object->GetGameObjectTag() == "Player")
		PlayerCollisionStay(p_other_game_object);
}

void Ground_Script::OnCollisionExit(GameObject* p_other_game_object)
{
	if (p_other_game_object->GetGameObjectTag() == "Player")
		PlayerCollisionExit(p_other_game_object);
}

void Ground_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script
}