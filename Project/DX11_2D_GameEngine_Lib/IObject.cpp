#include "stdafx.h"
#include "IObject.h"

template<typename T>
constexpr ComponentType IObject::GetComponentType()
{
	return ComponentType::NONE;
}

#define REGISTER_COMPONENT_TYPE(T, component_type) template<>  ComponentType IObject::GetComponentType<T>() { return component_type; }

REGISTER_COMPONENT_TYPE(Transform, ComponentType::Transform);
REGISTER_COMPONENT_TYPE(Camera, ComponentType::Camera);
REGISTER_COMPONENT_TYPE(Animator, ComponentType::Animator);
//TODO: ���Ŀ� �ٸ� Component�鵵 �߰� ����