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
		m_p_owner_game_object.reset();
	}

	virtual void Update() {} //������Ʈ
	virtual void FinalUpdate() = 0; //���� ������Ʈ => ������Ʈ�� �����̸� �ȵ�

public:
	virtual IComponent* Clone() = 0;

public:
	const ComponentType& GetComponentType() const { return this->m_component_type; }

	const std::shared_ptr<GameObject>& GetGameObject() { SAFE_GET_POINTER(this->m_p_owner_game_object.lock()); }
	void SetGameObject(const std::shared_ptr<GameObject>& p_game_object) { this->m_p_owner_game_object = p_game_object; }

protected:
	const ComponentType m_component_type;
	//�ش� ������Ʈ�� �����ϰ� �ִ� ������Ʈ�� �ּ�
	std::weak_ptr<GameObject> m_p_owner_game_object;
};