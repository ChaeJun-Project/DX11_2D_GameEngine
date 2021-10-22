#include "stdafx.h"
#include "GameObject.h"

#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"

#include "Transform.h"
#include "Camera.h"
#include "Renderer.h"
#include "Animator.h"
#include "Script.h"
#include "Collider2D.h"

template<typename T>
constexpr ComponentType GameObject::GetComponentType()
{
	return ComponentType::NONE;
}

#define REGISTER_COMPONENT_TYPE(T, component_type) template<> ComponentType GameObject::GetComponentType<T>() { return component_type; }
REGISTER_COMPONENT_TYPE(Transform, ComponentType::Transform);
REGISTER_COMPONENT_TYPE(Camera, ComponentType::Camera);
REGISTER_COMPONENT_TYPE(Renderer, ComponentType::Renderer);
REGISTER_COMPONENT_TYPE(Animator, ComponentType::Animator);
REGISTER_COMPONENT_TYPE(Script, ComponentType::Script);
REGISTER_COMPONENT_TYPE(Collider2D, ComponentType::Collider2D);

GameObject::GameObject(const GameObject& origin)
{
	for (auto& origin_component : origin.m_component_list)
	{
		AddComponent(origin_component.second->Clone());
	}

	for (auto& child : origin.m_p_child_vector)
	{
		AddChild(child->Clone());
	}
}

GameObject::~GameObject()
{
	for (auto& component : this->m_component_list)
		SAFE_DELETE(component.second);

	this->m_component_list.clear();

	this->m_p_parent = nullptr;

	for (auto& child : this->m_p_child_vector)
		SAFE_DELETE(child);

	this->m_p_child_vector.clear();
	this->m_p_child_vector.shrink_to_fit();
}

void GameObject::Update()
{
	if (this->m_dead_check)
		return;

	//������Ʈ ������Ʈ
	for (auto& component : this->m_component_list)
		component.second->Update();

	//�ڽ� ������Ʈ ������Ʈ
	for (auto& child : this->m_p_child_vector)
		child->Update();
}

void GameObject::LateUpdate()
{
	if (this->m_dead_check)
		return;

}

void GameObject::FinalUpdate()
{
	//������Ʈ ���� ������Ʈ
	for (auto& component : this->m_component_list)
		component.second->FinalUpdate();

	//�ڽ� ������Ʈ ���� ������Ʈ(transform)
	for (auto& child : this->m_p_child_vector)
		child->FinalUpdate();

	//Layer�� ���
	auto current_scene = SceneManager::GetInstance()->GetCurrentScene();
	auto layer = current_scene->GetLayer(static_cast<UINT>(this->m_object_layer_index));
	layer->RegisterObject(this);
}

void GameObject::Render()
{
	//Renderer
	auto renderer = GetComponent<Renderer>();
	if (renderer != nullptr)
		renderer->Render();

	//Collider
	auto collider2D = GetComponent<Collider2D>();
	if (collider2D != nullptr)
		collider2D->Render();
}

//������ �����ʹ� ������ �̸� �����ϰ� �ִ� ���� ������Ʈ�� �����ؾ� ��
void GameObject::AddComponent(IComponent* p_component)
{
	p_component->SetGameObject(this);
	this->m_component_list.push_back
	(
		std::make_pair(p_component->GetComponentType(), p_component)
	);
}

IComponent* GameObject::GetComponent(const ComponentType& component_type) const
{
	for (auto& component : this->m_component_list)
	{
		if (component.first == component_type)
			return component.second;
	}

	return nullptr;
}

//TODO: ��ũ��Ʈ�� �� �� �̻� ������ ��� ������ ��� ���� ���
void GameObject::RemoveComponent(const ComponentType& component_type)
{
	std::list<std::pair<ComponentType, IComponent*>>::iterator list_iter;

	for (list_iter = this->m_component_list.begin(); list_iter != this->m_component_list.end();)
	{
		if (list_iter->first == component_type)
		{
			list_iter = this->m_component_list.erase(list_iter);
		}

		else
			++list_iter;
	}
}

GameObject* GameObject::GetRoot()
{
	if (HasParent())
	{
		return this->m_p_parent->GetRoot();
	}

	return this;
}

void GameObject::SetParent(GameObject* p_parent_game_object)
{
	if (p_parent_game_object == nullptr)
	{
		return;
	}

	//���ڷ� ���� ���۳�Ʈ�� �ڱ� �ڽ��� ���
	if (this->GetObjectID() == p_parent_game_object->GetObjectID())
		return;

	if (HasParent())
	{
		//���� ��ϵ� �θ�� ���ڷ� ���� �θ� ���� ���� ���۳�Ʈ ID�� ������ ���(�Ȱ��� �θ��� ���)
		if (this->m_p_parent->GetObjectID() == p_parent_game_object->GetObjectID())
			return;
	}

	auto old_parenet = this->m_p_parent;
	this->m_p_parent = p_parent_game_object;

	if (old_parenet != nullptr)
		old_parenet->TachChild();

	if (HasParent())
		this->m_p_parent->TachChild();
}

GameObject* GameObject::GetChildFromIndex(const UINT& index) const
{
	if (this->m_p_child_vector[index] != nullptr)
		return this->m_p_child_vector[index];

	return nullptr;
}

GameObject* GameObject::GetChildFromObjectName(const std::string& object_name) const
{
	for (auto& child : this->m_p_child_vector)
	{
		if (child->m_object_name == object_name)
			return child;
	}

	return nullptr;
}

void GameObject::AddChild(GameObject* p_child_game_object)
{
	//��ȿ�� ���� �ƴ� ���
	if (p_child_game_object == nullptr)
		return;

	//���ڷ� ���� �ڽ� ������Ʈ�� ���� �ڱ� �ڽ��� ���
	if (this->GetObjectID() == p_child_game_object->GetObjectID())
		return;

	this->m_p_child_vector.emplace_back(p_child_game_object);

	p_child_game_object->m_p_parent = this;
}

void GameObject::DetachChild()
{
	//�θ� ���� ���
	if (!HasParent())
		return;

	//TODO
}

void GameObject::TachChild()
{
	//�ڽ� ������Ʈ�� transform�� ������ �ִ� ���
	//�ڽ� ������Ʈ�� transform vector �ʱ�ȭ
	if (HasChilds())
	{
		for (auto& child : this->m_p_child_vector)
		{
			SAFE_DELETE(child);
		}
		this->m_p_child_vector.clear();
		this->m_p_child_vector.shrink_to_fit();
	}

	auto current_scene = SceneManager::GetInstance()->GetCurrentScene();

	if (current_scene != nullptr)
	{
		//TODO
	}
}
