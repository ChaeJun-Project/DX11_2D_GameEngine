#pragma once

#include "stdafx.h"

#include "IObject.h"

class IComponent
{
public:
    IComponent(const ComponentType& component_type)
        :m_component_type(component_type)
    {
    }
    virtual ~IComponent() = default;

public:
    const ComponentType& GetComponentType() const { return this->m_component_type; }

protected:
    const ComponentType m_component_type;
    IObject* m_p_game_object = nullptr;
};