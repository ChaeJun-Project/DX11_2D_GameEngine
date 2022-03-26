#include "stdafx.h"
#include "Door_Script.h"

#include <DX11_2D_GameEngine_Lib/SpriteAnimation.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Animator2D.h>
#include <DX11_2D_GameEngine_Lib/Collider2D.h>
#include <DX11_2D_GameEngine_Lib/RigidBody2D.h>
#include <DX11_2D_GameEngine_Lib/AudioSource.h>

Door_Script::Door_Script()
	:Script("Door_Script")
{
}

Door_Script::Door_Script(const Door_Script& origin)
	: Script(origin.m_script_name)
{
}

Door_Script::~Door_Script()
{
	m_p_transform = nullptr;
	m_p_animator2D = nullptr;
	m_p_collider2D = nullptr;
}

void Door_Script::Start()
{
	m_p_transform = m_p_owner_game_object->GetComponent<Transform>();
	m_p_animator2D = m_p_owner_game_object->GetComponent<Animator2D>();
	m_p_collider2D = m_p_owner_game_object->GetComponent<Collider2D>();
	m_p_audio_source = m_p_owner_game_object->GetComponent<AudioSource>();
}

void Door_Script::Update()
{
	if (m_p_animator2D->GetCurrentAnimation()->GetResourceName()._Equal("Door_Open")
		&& m_p_animator2D->GetCurrentAnimation()->GetIsFinished()
		&& !is_open)
	{
		is_open = true;
		m_is_wall = false;
	}
}

void Door_Script::PlayerCollisionEnter(GameObject* p_player_game_object)
{
	if (!is_open)
	{
		if (!m_p_animator2D->GetIsPlaying())
		{
			m_p_animator2D->Play();
			m_p_audio_source->SetCurrentAudioClip("Door_Open_Sound");
		}

		//Door
		auto door_position = m_p_transform->GetTranslation();
		auto door_scale_x = m_p_transform->GetScale().x;

		auto door_collider2D_scale_x = m_p_collider2D->GetOffsetScale().x;
		door_collider2D_scale_x *= 0.5f;
		auto door_collider2D_position_x = m_p_collider2D->GetOffsetPosition().x;
		door_collider2D_position_x *= -1.0f;

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

			//플레이어보다 오른쪽에 있는 문일 경우
			if (door_position.x > player_position.x)
			{
				m_is_right = true;
				p_player_rigidbody2D->SetWall(m_is_right, m_is_wall);

				auto door_position_left_x = door_position.x - 100.0f * (door_collider2D_scale_x + door_collider2D_position_x) * door_scale_x;
				m_player_position_x = door_position_left_x - 100.0f * (player_collider2D_scale_x + player_collider2D_position_x) * player_scale_x;
			}

			//왼쪽에 있는 문일 경우
			else if (door_position.x < player_position.x)
			{
				m_is_right = false;
				p_player_rigidbody2D->SetWall(m_is_right, m_is_wall);

				auto door_position_right_x = door_position.x + 100.0f * (door_collider2D_scale_x + door_collider2D_position_x) * door_scale_x;
				m_player_position_x = door_position_right_x + 100.0f * (player_collider2D_scale_x + player_collider2D_position_x) * player_scale_x;
			}

			p_player_rigidbody2D->SetVelocity(Vector2(0.0f, p_player_rigidbody2D->GetVelocity().y));
		}

		player_position.x = m_player_position_x;

		p_player_transform->SetTranslation(player_position);
	}
}

void Door_Script::PlayerCollisionStay(GameObject* p_player_game_object)
{
	//Player
	auto p_player_transform = p_player_game_object->GetComponent<Transform>();
	auto player_position = p_player_transform->GetTranslation();

	auto p_player_rigidbody2D = p_player_game_object->GetComponent<RigidBody2D>();

	if (!is_open)
	{
		if (m_is_wall)
			p_player_rigidbody2D->SetWall(m_is_right, m_is_wall);

		player_position.x = m_player_position_x;

		p_player_transform->SetTranslation(player_position);
	}

	else
		p_player_rigidbody2D->SetWall(m_is_right, m_is_wall);
}

void Door_Script::PlayerCollisionExit(GameObject* p_player_game_object)
{
	auto p_player_rigidbody2D = p_player_game_object->GetComponent<RigidBody2D>();

	m_is_wall = false;
	m_player_position_x = 0.0f;
	p_player_rigidbody2D->SetWall(m_is_right, m_is_wall);

	//문이 열려있는 경우
	if (is_open)
	{
		//Door
		auto door_position = m_p_transform->GetTranslation();

		//Player
		auto p_player_transform = p_player_game_object->GetComponent<Transform>();
		auto player_position = p_player_transform->GetTranslation();

		//문이 플레이어보다 오른쪽에 있는 경우
		if (door_position.x > player_position.x)
		{
			is_open = true;
		}

		//문이 플레이어보다 왼쪽에 있는 경우
		else if (door_position.x < player_position.x)
		{
			m_p_animator2D->SetCurrentAnimation("Door_Close");
			m_p_audio_source->SetCurrentAudioClip("Door_Close_Sound");

			is_open = false;
		}
	}
}

void Door_Script::OnCollisionEnter(GameObject* p_other_game_object)
{
	if (p_other_game_object->GetGameObjectTag() == "Player")
		PlayerCollisionEnter(p_other_game_object);
}

void Door_Script::OnCollisionStay(GameObject* p_other_game_object)
{
	if (p_other_game_object->GetGameObjectTag() == "Player")
		PlayerCollisionStay(p_other_game_object);
}

void Door_Script::OnCollisionExit(GameObject* p_other_game_object)
{
	if (p_other_game_object->GetGameObjectTag() == "Player")
		PlayerCollisionExit(p_other_game_object);
}

void Door_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script
}
