#include "stdafx.h"
#include "GameObject.h"

template<typename T>
constexpr ComponentType GameObject::GetComponentType()
{
	return ComponentType::NONE;
}

#define REGISTER_COMPONENT_TYPE(T, component_type) template<>  ComponentType GameObject::GetComponentType<T>() { return component_type; }

REGISTER_COMPONENT_TYPE(Transform, ComponentType::Transform);
REGISTER_COMPONENT_TYPE(Camera, ComponentType::Camera);
REGISTER_COMPONENT_TYPE(Renderer, ComponentType::Renderer);
REGISTER_COMPONENT_TYPE(Animator, ComponentType::Animator);
//REGISTER_COMPONENT_TYPE(IScript, ComponentType::Script);
REGISTER_COMPONENT_TYPE(RockManZ_Script, ComponentType::Script);
//TODO: 추후에 다른 Component들도 추가 예정

GameObject::GameObject(const GameObjectType& game_object_type)
	:m_game_object_type(game_object_type)
{

}

GameObject::~GameObject()
{
	for (auto& component : m_component_un_map)
		component.second.reset();

	m_component_un_map.clear();
}

void GameObject::Update()
{
	for (auto& component : m_component_un_map)
		component.second->Update();
}

void GameObject::LateUpdate()
{
	//TODO
}

void GameObject::FinalUpdate()
{
	for (auto& component : m_component_un_map)
		component.second->FinalUpdate();
}

void GameObject::Render()
{
	auto renderer = GetComponent<Renderer>();

	if (renderer == nullptr)
		return;

	renderer->Render();
}
