#include "stdafx.h"
#include "GameObject.h"

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
#include "TileMapRenderer.h"
#include "RigidBody2D.h"
#include "AudioListener.h"
#include "AudioSource.h"
#include "Script.h"

template<typename T>
constexpr ComponentType GameObject::GetComponentType()
{
	return ComponentType::None;
}

//Script�� �ϳ� �̻��� ���� �� �ֱ� ������ �߰����� ����
#define REGISTER_COMPONENT_TYPE(T, component_type) template<> ComponentType GameObject::GetComponentType<T>() { return component_type; }
REGISTER_COMPONENT_TYPE(Transform, ComponentType::Transform);
REGISTER_COMPONENT_TYPE(Camera, ComponentType::Camera);
REGISTER_COMPONENT_TYPE(SpriteRenderer, ComponentType::SpriteRenderer);
REGISTER_COMPONENT_TYPE(Animator2D, ComponentType::Animator2D);
REGISTER_COMPONENT_TYPE(Animator, ComponentType::Animator);
REGISTER_COMPONENT_TYPE(Collider2D, ComponentType::Collider2D);
REGISTER_COMPONENT_TYPE(Light2D, ComponentType::Light2D);
REGISTER_COMPONENT_TYPE(ParticleSystem, ComponentType::ParticleSystem);
REGISTER_COMPONENT_TYPE(TileMapRenderer, ComponentType::TileMapRenderer);
REGISTER_COMPONENT_TYPE(RigidBody2D, ComponentType::RigidBody2D);
REGISTER_COMPONENT_TYPE(AudioListener, ComponentType::AudioListener);
REGISTER_COMPONENT_TYPE(AudioSource, ComponentType::AudioSource);

GameObject::GameObject(const GameObject& origin)
{
	//GameObject Active Check
	m_is_active = origin.m_is_active;
	//GameObject name
	m_object_name = origin.m_object_name;
	//GameObject Tag
	m_game_object_tag = origin.m_game_object_tag;
	//GameObject Layer
	m_game_object_layer = origin.m_game_object_layer;

	//Component
	for (const auto& origin_component : origin.m_p_component_map)
	{
		AddComponent(origin_component.second->Clone());
	}

	//Script
	for (const auto& origin_script : origin.m_p_script_un_map)
	{
		AddComponent(origin_script.second->Clone());
	}

	//Child GameObject
	for (const auto& child : origin.m_p_child_vector)
	{
		AddChild(child->Clone());
	}
}

GameObject::~GameObject()
{
	//Component
	for (auto& component : m_p_component_map)
		SAFE_DELETE(component.second);

	m_p_component_map.clear();

	//Script
	for (auto& script : m_p_script_un_map)
		SAFE_DELETE(script.second);

	m_p_script_un_map.clear();

	//Parent
	m_p_parent = nullptr;

	//Child
	for (auto& child : m_p_child_vector)
		SAFE_DELETE(child);

	m_p_child_vector.clear();
	m_p_child_vector.shrink_to_fit();
}

void GameObject::Start()
{
	//Component ����
	for (auto& component : m_p_component_map)
	{
		if (component.second->GetIsActive())
			component.second->Start();
	}

	//Script ����
	for (auto& script : m_p_script_un_map)
	{
		if (script.second->GetIsActive())
			script.second->Start();
	}

	//�ڽ� ������Ʈ ������Ʈ
	for (auto& child : m_p_child_vector)
		child->Start();
}

void GameObject::Update()
{
	if (m_dead_check)
		return;

	//Component ������Ʈ
	for (auto& component : m_p_component_map)
	{
		if (component.second->GetIsActive())
			component.second->Update();
	}

	//Script ������Ʈ
	for (auto& script : m_p_script_un_map)
	{
		if (script.second->GetIsActive())
			script.second->Update();
	}

	//�ڽ� ������Ʈ ������Ʈ
	for (auto& child : m_p_child_vector)
		child->Update();
}

void GameObject::FinalUpdate()
{
	if (m_dead_check)
		return;

	//������Ʈ ���� ������Ʈ
	for (auto& component : m_p_component_map)
	{
		if (component.second->GetIsActive())
			component.second->FinalUpdate();
	}

	//�ڽ� ������Ʈ ���� ������Ʈ(transform)
	for (auto& child : m_p_child_vector)
		child->FinalUpdate();
}

void GameObject::Render()
{
	if (m_dead_check)
		return;

	//SpriteRenderer
	auto renderer = GetComponent<SpriteRenderer>();
	if (renderer != nullptr && renderer->GetIsActive())
		renderer->Render();

	//Particle System
	auto particle_system = GetComponent<ParticleSystem>();
	if (particle_system != nullptr && particle_system->GetIsActive())
		particle_system->Render();

	//Collider
	auto collider2D = GetComponent<Collider2D>();
	if (collider2D != nullptr && collider2D->GetIsActive())
		collider2D->Render();

	//TileMapRenderer
	auto tile_map = GetComponent<TileMapRenderer>();
	if (tile_map != nullptr && tile_map->GetIsActive())
		tile_map->Render();
}

void GameObject::AddComponent(const ComponentType& component_type)
{
	switch (component_type)
	{
	case ComponentType::Transform:
		AddComponent(new Transform());
		break;
	case ComponentType::Camera:
		AddComponent(new Camera());
		break;
	case ComponentType::SpriteRenderer:
		AddComponent(new SpriteRenderer());
		break;
	case ComponentType::Animator2D:
		AddComponent(new Animator2D());
		break;
	case ComponentType::Animator:
		//TODO
		break;
	case ComponentType::Collider2D:
		AddComponent(new Collider2D());
		break;
	case ComponentType::Light2D:
		AddComponent(new Light2D());
		break;
	case ComponentType::ParticleSystem:
		AddComponent(new ParticleSystem());
		break;
	case ComponentType::TileMapRenderer:
		AddComponent(new TileMapRenderer());
		break;
	case ComponentType::RigidBody2D:
		AddComponent(new RigidBody2D());
		break;
	case ComponentType::AudioListener:
		AddComponent(new AudioListener());
		break;
	case ComponentType::AudioSource:
		AddComponent(new AudioSource());
		break;
	}
}

void GameObject::AddComponent(IComponent* p_component)
{
	//�ش� Component�� �����ϰ� �ִ� ���� ������Ʈ�� ����
	p_component->SetGameObject(this);

	//�ش� Component�� Script�� ���
	if (p_component->GetComponentType() == ComponentType::Script)
	{
		auto p_script = dynamic_cast<Script*>(p_component); //�ٿ� ĳ����
		auto script_iter = m_p_script_un_map.insert(std::make_pair(p_script->GetScriptName(), p_script));

		//�̹� �߰��� ��ũ��Ʈ���(��ũ��Ʈ�� �ߺ����� �߰��� ���)
		//�ٿ� ĳ���õ� ������ �Ǵ� �Ű����� ������ ���� �θ� ������ �޸� ����(���� ������ �޸� ���� �߻�)
		if (!script_iter.second)
			SAFE_DELETE(p_script);
	}

	//�� ���� ���
	else
		m_p_component_map.insert(std::make_pair(p_component->GetComponentType(), p_component));
}

IComponent* GameObject::GetComponent(const ComponentType& component_type) const
{
	auto component_iter = m_p_component_map.find(component_type);

	if (component_iter == m_p_component_map.end())
		return nullptr;

	return component_iter->second;
}

void GameObject::RemoveComponent(const ComponentType& component_type)
{
	auto component_iter = m_p_component_map.find(component_type);

	if (component_iter == m_p_component_map.end())
		return;

	//�ش� Component �޸� ����
	SAFE_DELETE(component_iter->second);

	//�ش� Component ����
	m_p_component_map.erase(component_type);
}

Script* GameObject::GetScript(const std::string& script_name)
{
	auto script_iter = m_p_script_un_map.find(script_name);

	if (script_iter == m_p_script_un_map.end())
		return nullptr;

	return script_iter->second;
}

void GameObject::RemoveScript(const std::string& script_name)
{
	auto script_iter = m_p_script_un_map.find(script_name);

	if (script_iter == m_p_script_un_map.end())
		return;

	//�ش� Script �޸� ����
	SAFE_DELETE(script_iter->second);

	//�ش� Script ����
	m_p_script_un_map.erase(script_name);
}

void GameObject::SetGameObjectLayer(const UINT& layer_index)
{
	if (m_game_object_layer != layer_index)
	{
		//Current Scene
		auto p_current_scene = SCENE_MANAGER->GetCurrentScene();

		//Deregister from Pre Layer
		auto p_pre_layer = p_current_scene->GetLayer(m_game_object_layer);
		p_pre_layer->DeregisterGameObject(this);

		//Set New Layer Index
		m_game_object_layer = layer_index;

		//Register to Current Layer
		auto p_current_layer = p_current_scene->GetLayer(m_game_object_layer);
		p_current_layer->RegisterGameObject(this);
	}
}

//=====================================================================
// [Hierarchy]
//=====================================================================
GameObject* GameObject::GetRoot()
{
	if (HasParent())
	{
		return m_p_parent->GetRoot();
	}

	return this;
}

GameObject* GameObject::GetChildFromIndex(const UINT& index) const
{
	if (m_p_child_vector[index] != nullptr)
		return m_p_child_vector[index];

	return nullptr;
}

GameObject* GameObject::GetChildFromObjectName(const std::string& object_name) const
{
	for (auto child : m_p_child_vector)
	{
		if (child->m_object_name == object_name)
			return child;
	}

	return nullptr;
}

const bool GameObject::GetHasChild(GameObject* p_game_object)
{
	std::vector<GameObject*>::iterator iter = m_p_child_vector.begin();

	for (; iter != m_p_child_vector.end();)
	{
		if ((*iter) == p_game_object)
		{
			return true;
		}

		else
			++iter;
	}

	return false;
}

void GameObject::AddChild(GameObject* p_child_game_object)
{
	//��ȿ�� ���� �ƴ� ���
	if (p_child_game_object == nullptr)
		return;

	//���ڷ� ���� �ڽ� ������Ʈ�� ���� �ڱ� �ڽ��� ���
	if (GetObjectID() == p_child_game_object->GetObjectID())
		return;

	m_p_child_vector.emplace_back(p_child_game_object);

	p_child_game_object->m_p_parent = this;
}

void GameObject::DetachFromParent()
{
	//�θ� ���� ���
	if (!HasParent())
		return;

	std::vector<GameObject*>::iterator iter = m_p_parent->m_p_child_vector.begin();

	for (; iter != m_p_parent->m_p_child_vector.end();)
	{
		if ((*iter) == this)
		{
			iter = m_p_parent->m_p_child_vector.erase(iter);
			this->m_p_parent = nullptr;
			GetComponent<Transform>()->InitialzieProperty();
			return;
		}

		else
			++iter;
	}
}

void GameObject::SaveToScene(FILE* p_file)
{
	//GameObject Name
	__super::SaveToScene(p_file); //DX11Obejct

	//Active 
	fprintf(p_file, "[Active]\n");
	fprintf(p_file, "%d\n", m_is_active);

	//Tag
	fprintf(p_file, "[Tag]\n");
	fprintf(p_file, "%s\n", m_game_object_tag.c_str());

	//Layer
	fprintf(p_file, "[Layer]\n");
	fprintf(p_file, "%d\n", m_game_object_layer);
}

void GameObject::LoadFromScene(FILE* p_file)
{
	//GameObject Name
	__super::LoadFromScene(p_file); //DX11Obejct

	char char_buffer[256] = {};

	//Active 
	FILE_MANAGER->FScanf(char_buffer, p_file);
	int boolen_num = -1;
	fscanf_s(p_file, "%d\n", &boolen_num);
	m_is_active = boolen_num;
	
	//Tag
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf(char_buffer, p_file);
	m_game_object_tag = std::string(char_buffer);

	//Layer
	FILE_MANAGER->FScanf(char_buffer, p_file);
	fscanf_s(p_file, "%d\n", &m_game_object_layer);
}