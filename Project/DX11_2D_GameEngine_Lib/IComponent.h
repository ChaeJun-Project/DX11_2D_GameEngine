#pragma once

#include "stdafx.h"

#include "IObject.h"

class IComponent
{
public:
    IComponent(const ComponentType& component_type, IObject* p_game_object)
        :m_component_type(component_type),
        m_p_game_object(p_game_object)
    {
    }
    virtual ~IComponent() = default;

    virtual void Update() {} //업데이트
    virtual void LateUpdate() {} //카메라가 특정 오브젝트의 위치에 따라 이동
    virtual void FinalUpdate() = 0; //최종 업데이트 => 오브젝트가 움직이면 안됨

public:
    const ComponentType& GetComponentType() const { return this->m_component_type; }
    IObject* GetGameObject() { SAFE_GET_POINTER(this->m_p_game_object); }
   
protected:
    const ComponentType m_component_type;
    //해당 컴포넌트를 소유하고 있는 오브젝트의 주소
    IObject* m_p_game_object = nullptr; 
};