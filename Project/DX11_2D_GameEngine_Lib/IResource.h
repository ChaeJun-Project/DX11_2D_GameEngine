#pragma once

#include "stdafx.h"

#include "DX11Object.h"

class IResource : public DX11Obejct
{
public:
    IResource(const ResourceType& resource_type, const std::string& resource_name)
    :m_resource_type(resource_type), m_resource_name(resource_name)
    {  
    }

    virtual ~IResource() = default;

    virtual void LoadFromFile(const std::string& path) {}
    virtual void SaveFile(const std::string& path) {}

    virtual void BindPipeline() {}

public:
    const ResourceType& GetResourceType() const { return this->m_resource_type; }

    const std::string& GetResourceName() const { return this->m_resource_name; }
    void SetResourceName(const std::string& resource_name) { this->m_resource_name = m_resource_name; }

    const std::string& GetResourcePath() const { return this->m_resource_path; }
    void SetResourcePath(const std::string& resource_path) { this->m_resource_path = resource_path; }

protected:
    const ResourceType m_resource_type;
    std::string m_resource_name;
    std::string m_resource_path;
};