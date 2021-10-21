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

	virtual void Update() {} //������Ʈ
	virtual void FinalUpdate() = 0; //���� ������Ʈ => ������Ʈ�� �����̸� �ȵ�

public:
	const ComponentType& GetComponentType() const { return this->m_component_type; }

	GameObject* GetGameObject() { SAFE_GET_POINTER(this->m_p_owner_game_object); }
	void SetGameObject(GameObject* p_game_object) { this->m_p_owner_game_object = p_game_object; }

protected:
	const ComponentType m_component_type;
	//�ش� ������Ʈ�� �����ϰ� �ִ� ������Ʈ�� �ּ�
	GameObject* m_p_owner_game_object;
};