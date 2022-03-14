#pragma once

#include "stdafx.h"

#include "DX11Object.h"

class GameObject;

class IComponent : public DX11Obejct
{
public:
	IComponent(const ComponentType& component_type)
		:m_component_type(component_type)
	{
	}
	virtual ~IComponent()
	{
		m_p_owner_game_object = nullptr;
	}

	virtual void Start() {}
	virtual void Update() {} //업데이트
	virtual void FinalUpdate() = 0; //최종 업데이트 => 오브젝트가 움직이면 안됨

public:
	virtual void SaveToScene(FILE* p_file) override
	{
		switch (m_component_type)
		{
		case ComponentType::Transform:
			fprintf(p_file, "● Transform\n");
			break;
		case ComponentType::Camera:
			fprintf(p_file, "● Camera\n");
			break;
		case ComponentType::SpriteRenderer:
			fprintf(p_file, "● Sprite Renderer\n");
			break;
		case ComponentType::Animator2D:
			fprintf(p_file, "● Animator2D\n");
			break;
		case ComponentType::Animator:
			fprintf(p_file, "● Animator\n");
			break;
		case ComponentType::Collider2D:
			fprintf(p_file, "● Collider2D\n");
			break;
		case ComponentType::Light2D:
			fprintf(p_file, "● Light2D\n");
			break;
		case ComponentType::ParticleSystem:
			fprintf(p_file, "● Particle System\n");
			break;
		case ComponentType::TileMapRenderer:
			fprintf(p_file, "● TileMapRenderer\n");
			break;
		case ComponentType::RigidBody2D:
			fprintf(p_file, "● RigidBody2D\n");
			break;
		case ComponentType::AudioListener:
			fprintf(p_file, "● Audio Listener\n");
			break;
		case ComponentType::AudioSource:
			fprintf(p_file, "● Audio Source\n");
			break;
		case ComponentType::Script:
			fprintf(p_file, "● Script\n");
			break;
		}

		//Component Type
		fprintf(p_file, "%d\n", static_cast<UINT>(m_component_type));

		//Active
		fprintf(p_file, "[Active]\n");
	    fprintf(p_file, "%d\n", m_is_active);
	}
	virtual void LoadFromScene(FILE* p_file) override
	{
		char char_buffer[256] = {};

		//Active 
		FILE_MANAGER->FScanf(char_buffer, p_file);
		int boolen_num = -1;
		fscanf_s(p_file, "%d\n", &boolen_num);
		m_is_active = boolen_num;
	}

public:
	virtual IComponent* Clone() const = 0;

public:
	const ComponentType& GetComponentType() { return m_component_type; }

	GameObject* GetGameObject() { SAFE_GET_POINTER(m_p_owner_game_object); }
	void SetGameObject(GameObject* p_game_object) { m_p_owner_game_object = p_game_object; }

	const bool& GetIsActive() { return m_is_active; }
	void SetIsActive(const bool& is_active) { m_is_active = is_active; }

protected:
	const ComponentType m_component_type;

	//해당 컴포넌트를 소유하고 있는 오브젝트의 주소
	GameObject* m_p_owner_game_object;

	bool m_is_active = true;
};