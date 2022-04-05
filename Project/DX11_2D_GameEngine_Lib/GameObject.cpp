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
#include "ParticleRenderer.h"
#include "TileMapRenderer.h"
#include "RigidBody2D.h"
#include "AudioListener.h"
#include "AudioSource.h"
#include "Canvas.h"
#include "RectTransform.h"
#include "ImageRenderer.h"
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
REGISTER_COMPONENT_TYPE(ParticleRenderer, ComponentType::ParticleRenderer);
REGISTER_COMPONENT_TYPE(TileMapRenderer, ComponentType::TileMapRenderer);
REGISTER_COMPONENT_TYPE(RigidBody2D, ComponentType::RigidBody2D);
REGISTER_COMPONENT_TYPE(AudioListener, ComponentType::AudioListener);
REGISTER_COMPONENT_TYPE(AudioSource, ComponentType::AudioSource);
REGISTER_COMPONENT_TYPE(Canvas, ComponentType::Canvas);
//Rect Transform�� Transform�� ��ӹ޾� m_p_component_map���� ComponentType::Transform�� ��ġ�� ���� �� ���
REGISTER_COMPONENT_TYPE(ImageRenderer, ComponentType::ImageRenderer);

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
	RemoveAllComponent();

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
	{
		if (child->m_is_active)
			child->Start();
	}
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
	/*if (m_dead_check)
		return;*/

	//Sprite Renderer
	auto p_sprite_renderer = GetComponent<SpriteRenderer>();
	if (p_sprite_renderer != nullptr && p_sprite_renderer->GetIsActive())
		p_sprite_renderer->Render();

	//Collider
	if (RENDER_MANAGER->GetDebugMode())
	{
		auto p_collider2D = GetComponent<Collider2D>();
		if (p_collider2D != nullptr && p_collider2D->GetIsActive())
			p_collider2D->Render();
	}

	//TileMap Renderer
	auto p_tile_map_renderer = GetComponent<TileMapRenderer>();
	if (p_tile_map_renderer != nullptr && p_tile_map_renderer->GetIsActive())
		p_tile_map_renderer->Render();

	//Particle Renderer
	auto p_particle_system = GetComponent<ParticleRenderer>();
	if (p_particle_system != nullptr && p_particle_system->GetIsActive())
		p_particle_system->Render();

	//Canvas
	auto p_canvas = GetComponent<Canvas>();
	if (p_canvas != nullptr && p_canvas->GetIsActive())
		p_canvas->Render();

	//Image Renderer
	auto p_image_renderer = GetComponent<ImageRenderer>();
	if (p_image_renderer != nullptr && p_image_renderer->GetIsActive())
		p_image_renderer->Render();
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
	case ComponentType::ParticleRenderer:
		AddComponent(new ParticleRenderer());
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
	case ComponentType::Canvas:
		AddComponent(new Canvas());
		break;
	case ComponentType::RectTransform:
		AddRectTransform();
		break;
	case ComponentType::ImageRenderer:
		AddImageRendererComponent();
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
	{
		if (p_component->GetComponentType() == ComponentType::RectTransform)
			m_p_component_map.insert(std::make_pair(ComponentType::Transform, p_component));

		else
			m_p_component_map.insert(std::make_pair(p_component->GetComponentType(), p_component));
	}
}

void GameObject::AddRectTransform()
{
	auto p_transform = GetComponent<Transform>();

	auto p_rect_transform = new RectTransform(*p_transform);

	RemoveAllComponent(); //���� GameObject�� �����ϰ� �ִ� ��� Component ���� => �ش� GameObject�� UI�� ���

	AddComponent(p_rect_transform); //RectTransform Component �߰�
}

void GameObject::AddImageRendererComponent()
{
	//���� GameObject�� Rect Transform�� ������ ���� ���� ���
	if (GetComponent(ComponentType::RectTransform) == nullptr)
	{
		//Rect Transform �߰�
		AddComponent(ComponentType::RectTransform);
	}

	//Image Renderer �߰�
	AddComponent(new ImageRenderer());
}

IComponent* GameObject::GetComponent(const ComponentType& component_type) const
{
	ComponentType type = ComponentType::None;

	if (component_type == ComponentType::RectTransform)
		type = ComponentType::Transform;

	else
		type = component_type;

	auto component_iter = m_p_component_map.find(type);

	if (component_iter == m_p_component_map.end())
		return nullptr;

	if (component_type == ComponentType::RectTransform)
	{
		if (component_iter->second->GetComponentType() != ComponentType::RectTransform)
			return nullptr;
	}

	return component_iter->second;
}

void GameObject::RemoveComponent(const ComponentType& component_type)
{
	ComponentType type = ComponentType::None;

	if (component_type == ComponentType::RectTransform)
		type = ComponentType::Transform;

	else
		type = component_type;

	auto component_iter = m_p_component_map.find(type);

	if (component_iter == m_p_component_map.end())
		return;

	//�����Ϸ��� Component�� RectTransform�� ���
	if (component_type == ComponentType::RectTransform)
	{
		if (component_iter->second->GetComponentType() != ComponentType::RectTransform)
			return;

		auto p_rect_transform = dynamic_cast<RectTransform*>(component_iter->second); //�� ĳ����

		auto p_transform = new Transform(*p_rect_transform); //���ο� Transform Component ����

		//��� Component ����
		RemoveAllComponent();

		//���� ������ Transform Component �߰�
		AddComponent(p_transform);
	}

	else
	{
		//�ش� Component �޸� ����
		SAFE_DELETE(component_iter->second);

		//�ش� Component ����
		m_p_component_map.erase(component_type);
	}
}

void GameObject::RemoveAllComponent()
{
	for (auto& p_component : m_p_component_map)
		SAFE_DELETE(p_component.second);

	m_p_component_map.clear();
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