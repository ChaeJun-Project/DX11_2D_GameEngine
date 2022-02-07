#pragma once

#include "stdafx.h"

#include "DX11Object.h"

class IResource : public DX11Obejct
{
public:
	IResource(const ResourceType& resource_type, const std::string& resource_name)
		:m_resource_type(resource_type)
	{
		m_object_name = resource_name;
	}

	virtual ~IResource() = default;

	virtual bool SaveToFile(const std::string& resource_path) { return true; }
	virtual bool LoadFromFile(const std::string& resource_path) { return true; }

	virtual void BindPipeline() {}

public:
	const ResourceType& GetResourceType() const { return m_resource_type; }

	const std::string& GetResourceName() const { return m_object_name; }
	void SetResourceName(const std::string& resource_name) { m_object_name = resource_name; }

	const std::string& GetResourcePath() const { return m_resource_path; }
	void SetResourcePath(const std::string& resource_path) { m_resource_path = resource_path; }

public:
	virtual IResource* Clone() = 0;

protected:
	const ResourceType m_resource_type;
	std::string m_resource_path = "None";
};