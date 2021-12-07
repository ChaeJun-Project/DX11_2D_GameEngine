#include "stdafx.h"
#include "GameObject.h"

#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"

#include "Transform.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "Animator2D.h"
#include "Animator.h"
#include "Collider2D.h"
#include "Light2D.h"
#include "ParticleSystem.h"
#include "TileMap.h"

#include "Script.h"

template<typename T>
constexpr ComponentType GameObject::GetComponentType()
{
	return ComponentType::NONE;
}

#define REGISTER_COMPONENT_TYPE(T, component_type) template<> ComponentType GameObject::GetComponentType<T>() { return component_type; }
REGISTER_COMPONENT_TYPE(Transform, ComponentType::Transform);
REGISTER_COMPONENT_TYPE(Camera, ComponentType::Camera);
REGISTER_COMPONENT_TYPE(SpriteRenderer, ComponentType::SpriteRenderer);
REGISTER_COMPONENT_TYPE(Animator2D, ComponentType::Animator2D);
REGISTER_COMPONENT_TYPE(Animator, ComponentType::Animator);
REGISTER_COMPONENT_TYPE(Collider2D, ComponentType::Collider2D);
REGISTER_COMPONENT_TYPE(Light2D, ComponentType::Light2D);
REGISTER_COMPONENT_TYPE(ParticleSystem, ComponentType::ParticleSystem);
REGISTER_COMPONENT_TYPE(TileMap, ComponentType::TileMap);

REGISTER_COMPONENT_TYPE(Script, ComponentType::Script);


GameObject::GameObject(const GameObject& origin)
{
	//Object name
	m_object_name = origin.m_object_name;
	//Object Tag
	m_object_tag = origin.m_object_tag;
	//Object Layer
	m_object_layer_index = origin.m_object_layer_index;

	m_dead_check = false;

	//해당 오브젝트로 프리팹을 만든 횟수
	m_prefab_count = origin.m_prefab_count;

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
	for (auto& component : m_component_list)
		SAFE_DELETE(component.second);

	m_component_list.clear();

	m_p_parent = nullptr;

	m_p_child_vector.clear();
	m_p_child_vector.shrink_to_fit();
}

void GameObject::Start()
{
	//컴포넌트 업데이트
	for (auto& component : m_component_list)
		component.second->Start();

	//자식 오브젝트 업데이트
	for (auto& child : m_p_child_vector)
		child->Start();
}

void GameObject::Update()
{
	if (m_dead_check)
		return;

	//컴포넌트 업데이트
	for (auto& component : m_component_list)
		component.second->Update();

	//자식 오브젝트 업데이트
	for (auto& child : m_p_child_vector)
		child->Update();
}

void GameObject::LateUpdate()
{
	if (m_dead_check)
		return;

}

void GameObject::FinalUpdate()
{
	//컴포넌트 최종 업데이트
	for (auto& component : m_component_list)
		component.second->FinalUpdate();

	//자식 오브젝트 최종 업데이트(transform)
	for (auto& child : m_p_child_vector)
		child->FinalUpdate();

	RegisterLayer();
}

void GameObject::RegisterLayer()
{
	//Layer에 등록
	auto current_scene = SceneManager::GetInstance()->GetCurrentScene();
	auto layer = current_scene->GetLayer(static_cast<UINT>(m_object_layer_index));
	layer->RegisterObject(this);
}

void GameObject::Render()
{
	if (!m_active_check)
		return;

	//SpriteRenderer
	auto renderer = GetComponent<SpriteRenderer>();
	if (renderer != nullptr)
		renderer->Render();

	//Particle System
	auto particle_system = GetComponent<ParticleSystem>();
	if (particle_system != nullptr)
		particle_system->Render();

	//Collider
	auto collider2D = GetComponent<Collider2D>();
	if (collider2D != nullptr)
		collider2D->Render();

	//TileMap
	auto tile_map = GetComponent<TileMap>();
	if (tile_map != nullptr)
		tile_map->Render();
}

//들어오는 데이터는 사전에 미리 소유하고 있는 게임 오브젝트를 설정해야 함
void GameObject::AddComponent(IComponent* p_component)
{
	p_component->SetGameObject(this);
	m_component_list.push_back
	(
		std::make_pair(p_component->GetComponentType(), p_component)
	);
}

IComponent* GameObject::GetComponent(const ComponentType& component_type) const
{
	for (auto& component : m_component_list)
	{
		if (component.first == component_type)
			return component.second;
	}

	return nullptr;
}

//TODO: 스크립트가 두 개 이상 존재할 경우 삭제를 어떻게 할지 고민
void GameObject::RemoveComponent(const ComponentType& component_type)
{
	std::list<std::pair<ComponentType, IComponent*>>::iterator list_iter;

	for (list_iter = m_component_list.begin(); list_iter != m_component_list.end();)
	{
		if (list_iter->first == component_type)
		{
			SAFE_DELETE(list_iter->second);
			list_iter = m_component_list.erase(list_iter);
		}

		else
			++list_iter;
	}
}

GameObject* GameObject::GetRoot()
{
	if (HasParent())
	{
		return m_p_parent->GetRoot();
	}

	return this;
}

void GameObject::SetParent(GameObject* p_parent_game_object)
{
	if (p_parent_game_object == nullptr)
	{
		return;
	}

	//인자로 들어온 컴퍼넌트가 자기 자신인 경우
	if (GetObjectID() == p_parent_game_object->GetObjectID())
		return;

	if (HasParent())
	{
		//현재 등록된 부모와 인자로 들어온 부모가 서로 같은 컴퍼넌트 ID를 소유한 경우(똑같은 부모인 경우)
		if (m_p_parent->GetObjectID() == p_parent_game_object->GetObjectID())
			return;
	}

	auto old_parenet = m_p_parent;
	m_p_parent = p_parent_game_object;

	if (old_parenet != nullptr)
		old_parenet->TachChild();

	if (HasParent())
		m_p_parent->TachChild();
}

GameObject* GameObject::GetChildFromIndex(const UINT& index) const
{
	if (m_p_child_vector[index] != nullptr)
		return m_p_child_vector[index];

	return nullptr;
}

GameObject* GameObject::GetChildFromObjectName(const std::string& object_name) const
{
	for (auto& child : m_p_child_vector)
	{
		if (child->m_object_name == object_name)
			return child;
	}

	return nullptr;
}

void GameObject::AddChild(GameObject* p_child_game_object)
{
	//유효한 값이 아닌 경우
	if (p_child_game_object == nullptr)
		return;

	//인자로 들어온 자식 오브젝트가 현재 자기 자신인 경우
	if (GetObjectID() == p_child_game_object->GetObjectID())
		return;

	m_p_child_vector.emplace_back(p_child_game_object);

	p_child_game_object->m_p_parent = this;
	p_child_game_object->m_object_layer_index = m_object_layer_index;
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
		for (auto& child : m_p_child_vector)
		{
			SAFE_DELETE(child);
		}
		m_p_child_vector.clear();
		m_p_child_vector.shrink_to_fit();
	}

	auto current_scene = SceneManager::GetInstance()->GetCurrentScene();

	if (current_scene != nullptr)
	{
		//TODO
	}
}

#include "Prefab.h"

void GameObject::RegisterPrefab()
{
	auto resource_manager = ResourceManager::GetInstance();

	std::string prefab_name = m_object_name;
	assert(!prefab_name.empty());

	//이미 해당 이름으로 프리팹 오브젝트가 존재하는 경우
	if (resource_manager->GetPrefab(prefab_name) != nullptr)
	{
		prefab_name += std::to_string(m_prefab_count);
	}

	resource_manager->AddPrefab(prefab_name, this);
	++m_prefab_count;
}
