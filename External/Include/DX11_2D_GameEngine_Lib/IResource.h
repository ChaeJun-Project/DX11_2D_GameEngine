#pragma once

#include "stdafx.h"

class IResource : public DX11Obejct
{
public:
    IResource() = default;
    virtual ~IResource() = default;

public:
    const ResourceType& GetResourceType() { return this->m_resource_type; }
    void SetResourceType(const ResourceType& resource_type) { this->m_resource_type = resource_type; }

    const std::string& GetResourceName() { return this->m_resource_name; }
    void SetResourceName(const std::string& resource_name) { this->m_resource_name = m_resource_name; }

protected:
    ResourceType m_resource_type;
    std::string m_resource_name;
};