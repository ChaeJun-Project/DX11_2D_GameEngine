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

//Script는 하나 이상을 가질 수 있기 때문에 추가하지 않음
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
//Rect Transform은 Transform을 상속받아 m_p_component_map에서 ComponentType::Transform의 위치에 저장 및 사용
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
	//Component 시작
	for (auto& component : m_p_component_map)
	{
		if (component.second->GetIsActive())
			component.second->Start();
	}

	//Script 시작
	for (auto& script : m_p_script_un_map)
	{
		if (script.second->GetIsActive())
			script.second->Start();
	}

	//자식 오브젝트 업데이트
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

	//Component 업데이트
	for (auto& component : m_p_component_map)
	{
		if (component.second->GetIsActive())
			component.second->Update();
	}

	//Script 업데이트
	for (auto& script : m_p_script_un_map)
	{
		if (script.second->GetIsActive())
			script.second->Update();
	}

	//자식 오브젝트 업데이트
	for (auto& child : m_p_child_vector)
		child->Update();
}

void GameObject::FinalUpdate()
{
	if (m_dead_check)
		return;

	//컴포넌트 최종 업데이트
	for (auto& component : m_p_component_map)
	{
		if (component.second->GetIsActive())
			component.second->FinalUpdate();
	}

	//자식 오브젝트 최종 업데이트(transform)
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
	//해당 Component를 소유하고 있는 게임 오브젝트를 설정
	p_component->SetGameObject(this);

	//해당 Component가 Script인 경우
	if (p_component->GetComponentType() == ComponentType::Script)
	{
		auto p_script = dynamic_cast<Script*>(p_component); //다운 캐스팅
		auto script_iter = m_p_script_un_map.insert(std::make_pair(p_script->GetScriptName(), p_script));

		//이미 추가된 스크립트라면(스크립트를 중복으로 추가할 경우)
		//다운 캐스팅된 포인터 또는 매개변수 값으로 들어온 부모 포인터 메모리 해제(하지 않으면 메모리 누수 발생)
		if (!script_iter.second)
			SAFE_DELETE(p_script);
	}

	//그 외의 경우
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

	RemoveAllComponent(); //현재 GameObject가 포함하고 있는 모든 Component 삭제 => 해당 GameObject를 UI로 사용

	AddComponent(p_rect_transform); //RectTransform Component 추가
}

void GameObject::AddImageRendererComponent()
{
	//현재 GameObject가 Rect Transform을 가지고 있지 않은 경우
	if (GetComponent(ComponentType::RectTransform) == nullptr)
	{
		//Rect Transform 추가
		AddComponent(ComponentType::RectTransform);
	}

	//Image Renderer 추가
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

	//삭제하려는 Component가 RectTransform인 경우
	if (component_type == ComponentType::RectTransform)
	{
		if (component_iter->second->GetComponentType() != ComponentType::RectTransform)
			return;

		auto p_rect_transform = dynamic_cast<RectTransform*>(component_iter->second); //업 캐스팅

		auto p_transform = new Transform(*p_rect_transform); //새로운 Transform Component 생성

		//모든 Component 삭제
		RemoveAllComponent();

		//새로 생성한 Transform Component 추가
		AddComponent(p_transform);
	}

	else
	{
		//해당 Component 메모리 해제
		SAFE_DELETE(component_iter->second);

		//해당 Component 삭제
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

	//해당 Script 메모리 해제
	SAFE_DELETE(script_iter->second);

	//해당 Script 삭제
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
	//유효한 값이 아닌 경우
	if (p_child_game_object == nullptr)
		return;

	//인자로 들어온 자식 오브젝트가 현재 자기 자신인 경우
	if (GetObjectID() == p_child_game_object->GetObjectID())
		return;

	m_p_child_vector.emplace_back(p_child_game_object);

	p_child_game_object->m_p_parent = this;
}

void GameObject::DetachFromParent()
{
	//부모가 없는 경우
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