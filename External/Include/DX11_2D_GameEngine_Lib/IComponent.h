#pragma once

#include "stdafx.h"

class GameObject;

class IComponent
{
public:
    IComponent(const ComponentType& component_type, GameObject* p_game_object)
        :m_component_type(component_type),
        m_p_game_object(p_game_object)
    {
    }
    virtual ~IComponent() = default;

    virtual void Update() {} //������Ʈ
    virtual void FinalUpdate() = 0; //���� ������Ʈ => ������Ʈ�� �����̸� �ȵ�

public:
    const ComponentType& GetComponentType() const { return this->m_component_type; }
    std::shared_ptr<GameObject> GetGameObject() { SAFE_GET_POINTER(this->m_p_game_object); }
   
protected:
    const ComponentType m_component_type;
    //�ش� ������Ʈ�� �����ϰ� �ִ� ������Ʈ�� �ּ�
    std::shared_ptr<GameObject> m_p_game_object = nullptr;
};