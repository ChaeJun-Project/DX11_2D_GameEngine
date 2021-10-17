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

GameObject::GameObject(const GameObject& origin)
{
	for (auto& origin_component : origin.m_component_list)
	{
		CreateSharedFromRaw(origin_component.second.get());
	}

	for (auto& child : origin.m_p_child_vector)
	{
		AddChild(std::make_shared<GameObject>(*(child.lock().get())));
	}
}

GameObject::~GameObject()
{
	for (auto& component : this->m_component_list)
		component.second.reset();

	m_component_list.clear();

	m_p_parent.reset();

	for (auto& child : this->m_p_child_vector)
		child.reset();
}

void GameObject::Update()
{
	if (this->m_dead_check)
		return;

	//컴포넌트 업데이트
	for (auto& component : this->m_component_list)
		component.second->Update();

	//자식 오브젝트 업데이트
	for (auto& child : this->m_p_child_vector)
		child.lock()->Update();
}

void GameObject::LateUpdate()
{
	if (this->m_dead_check)
		return;

}

void GameObject::FinalUpdate()
{
	//컴포넌트 최종 업데이트
	for (auto& component : this->m_component_list)
		component.second->FinalUpdate();

	//자식 오브젝트 최종 업데이트(transform)
	for (auto& child : this->m_p_child_vector)
		child.lock()->FinalUpdate();

	//Layer에 등록
	auto current_scene = SceneManager::GetInstance()->GetCurrentScene();
	auto layer = current_scene->GetLayer(static_cast<UINT>(this->m_object_layer_index));
	layer->RegisterObject(shared_from_this());
}

void GameObject::Render()
{
	auto renderer = std::dynamic_pointer_cast<Renderer>(GetComponent(ComponentType::Renderer));

	if (renderer == nullptr)
		return;

	renderer->Render();
}

const std::shared_ptr<GameObject>& GameObject::operator=(const GameObject& origin)
{
	for (auto& origin_component : origin.m_component_list)
	{
		CreateSharedFromRaw(origin_component.second.get());
	}

	for (auto& child : origin.m_p_child_vector)
	{
		AddChild(std::make_shared<GameObject>(*(child.lock().get())));
	}

	return shared_from_this();
}

const std::shared_ptr<IComponent>& GameObject::CreateSharedFromRaw(IComponent* p_component_raw)
{
	if (p_component_raw == nullptr)
		return nullptr;

	std::shared_ptr<IComponent> p_component = nullptr;

	switch (p_component_raw->GetComponentType())
	{
	case ComponentType::Transform:
	   
		p_component = std::make_shared<Transform>(*(dynamic_cast<Transform*>(p_component_raw)));
		break;
	case ComponentType::Camera:
		p_component = std::make_shared<Camera>(*(dynamic_cast<Camera*>(p_component_raw)));
		break;
	case ComponentType::Renderer:
		p_component = std::make_shared<Renderer>(*(dynamic_cast<Renderer*>(p_component_raw)));
		break;
	case ComponentType::Animator:
		p_component = std::make_shared<Animator>(*(dynamic_cast<Animator*>(p_component_raw)));
		break;
	/*case ComponentType::Script:
		p_component = std::make_shared<Script>(*p_component_raw);
		break;*/
	//case ComponentType::RigidBody2D:
	//	break;
	//case ComponentType::BoxCollider2D:
	//	break;
	}

	AddComponent(p_component);
}

//들어오는 데이터는 사전에 미리 소유하고 있는 게임 오브젝트를 설정해야 함
void GameObject::AddComponent(const std::shared_ptr<IComponent>& p_component)
{
	p_component->SetGameObject(shared_from_this());
	this->m_component_list.push_back
	(
		std::make_pair(p_component->GetComponentType(), p_component)
	);
}

const std::shared_ptr<IComponent>& GameObject::GetComponent(const ComponentType& component_type) const
{
	for (auto& component : this->m_component_list)
	{
		if (component.first == component_type)
			return component.second;
	}

	return nullptr;
}

//TODO: 스크립트가 두 개 이상 존재할 경우 삭제를 어떻게 할지 고민
void GameObject::RemoveComponent(const ComponentType& component_type)
{
	std::list<std::pair<ComponentType, std::shared_ptr<IComponent>>>::iterator list_iter;

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

const std::shared_ptr<GameObject>& GameObject::GetRoot()
{
	if (HasParent())
	{
		return this->m_p_parent->GetRoot();
	}

	return shared_from_this();
}

void GameObject::SetParent(const std::shared_ptr<GameObject>& p_parent_game_object)
{
	if (p_parent_game_object == nullptr)
	{
		return;
	}

	//인자로 들어온 컴퍼넌트가 자기 자신인 경우
	if (this->GetObjectID() == p_parent_game_object->GetObjectID())
		return;

	if (HasParent())
	{
		//현재 등록된 부모와 인자로 들어온 부모가 서로 같은 컴퍼넌트 ID를 소유한 경우(똑같은 부모인 경우)
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

const std::shared_ptr<GameObject>& GameObject::GetChildFromIndex(const UINT& index)
{
	if (this->m_p_child_vector[index].lock() != nullptr)
		return this->m_p_child_vector[index].lock();

	return nullptr;
}

const std::shared_ptr<GameObject>& GameObject::GetChildFromObjectName(const std::string& object_name)
{
	for (auto& child : this->m_p_child_vector)
	{
		if (child.lock()->m_object_name == object_name)
			return child.lock();
	}

	return nullptr;
}

void GameObject::AddChild(const std::shared_ptr<GameObject>& p_child_game_object)
{
	//유효한 값이 아닌 경우
	if (p_child_game_object == nullptr)
		return;

	//인자로 들어온 자식 오브젝트가 현재 자기 자신인 경우
	if (this->GetObjectID() == p_child_game_object->GetObjectID())
		return;

	this->m_p_child_vector.emplace_back(p_child_game_object);

	p_child_game_object->m_p_parent = shared_from_this();
}

void GameObject::DetachChild()
{
	//부모가 없는 경우
	if (!HasParent())
		return;

	//TODO
}

void GameObject::TachChild()
{
	//자식 오브젝트의 transform을 가지고 있는 경우
	//자식 오브젝트의 transform vector 초기화
	if (HasChilds())
	{
		for (auto& child : this->m_p_child_vector)
		{
			child.reset();
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
