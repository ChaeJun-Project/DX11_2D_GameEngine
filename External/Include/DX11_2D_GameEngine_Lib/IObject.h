#pragma once

#include "stdafx.h"

class IComponent;

class IObject : public DX11Obejct
{
public:
	IObject(const ObjectType& object_type)
		:m_object_type(object_type)
	{

	}
	virtual ~IObject() = default;

public:
	virtual void Update() = 0;
	virtual void Render() = 0;

public:
    template<typename T>
    void AddComponent();

	template<typename T>
	std::shared_ptr<T> GetComponent();

	template<typename T>
	void RemoveComponent();

public:
	const ObjectType& GetObjectType() const { return this->m_object_type; }

protected:
	const ObjectType m_object_type;

	std::string object_tag;
	std::string object_name;

	//Component List
	std::list<std::shared_ptr<IComponent>> m_component_list;
};

template<typename T>
inline void IObject::AddComponent()
{
}

template<typename T>
inline std::shared_ptr<T> IObject::GetComponent()
{
	return std::shared_ptr<T>();
}

template<typename T>
inline void IObject::RemoveComponent()
{
}
