#pragma once

#include "stdafx.h"

class IComponent;

class GameObject : public DX11Obejct, public std::enable_shared_from_this<GameObject>
{
public:
	GameObject() = default;
	GameObject(const GameObject& origin_game_object); //복사생성자
	~GameObject();

	void Update();
	void LateUpdate();
	void FinalUpdate();

	void Render();

private:
	template<typename T>
	static constexpr ComponentType GetComponentType();

public:
	void AddComponent(const std::shared_ptr<IComponent>& p_component);

	template<typename T>
	const std::shared_ptr<T>& GetComponent();
	const std::shared_ptr<IComponent>& GetComponent(const ComponentType& component_type) const;

	void RemoveComponent(const ComponentType& component_type);

public:
	//Object name
	const std::string& GetObjectName() const { return this->m_object_name; }
	void SetObjectName(const std::string& object_name) { this->m_object_name = object_name; }

	//Object Tag
	const std::string& GetObjectTag() const { return this->m_object_tag; }
	void SetObjectTag(const std::string& object_tag) { this->m_object_tag = object_tag; }
	
	//Object Layer
	const int& GetObjectLayer() const { return this->m_object_layer_index; }

	//Dead Check
	const bool IsDead() { return this->m_dead_check; }

public:
	//=====================================================================
	// [Hierarchy]
	//=====================================================================
	//재귀적인 방법으로 최상위 오브젝트 찾기
	const std::shared_ptr<GameObject>& GetRoot();
	const bool& GetIsRoot() { return !HasParent(); }

	const std::shared_ptr<GameObject>& GetParent() const { return this->m_p_parent.lock(); }
	void SetParent(const std::shared_ptr<GameObject>& p_parent_game_object);
	
	const std::vector<std::weak_ptr<GameObject>>& GetChilds() const { return this->m_p_child_vector; }
	const std::shared_ptr<GameObject>& GetChildFromIndex(const UINT& index);
	const std::shared_ptr<GameObject>& GetChildFromObjectName(const std::string& object_name);
	const UINT& GetChildCount() const { return static_cast<UINT>(this->m_p_child_vector.size()); }

	void AddChild(const std::shared_ptr<GameObject>& p_child_game_object);
	void DetachChild();
	void TachChild();

	const bool& HasParent() { return !(this->m_p_parent.expired()); }
	const bool& HasChilds() { return !(this->m_p_child_vector.empty()); }

private:
    void SetDead() { this->m_dead_check = true; }

public:
    CLONE(GameObject);

private:
	//Object name
	std::string m_object_name;
	//Object Tag
	std::string m_object_tag;
	//Object Layer
	int m_object_layer_index = -1;
	//Component list
	std::list<std::pair<ComponentType, std::shared_ptr<IComponent>>> m_component_list;
	
	//Hierarchy
	//Parent Object
	std::weak_ptr<GameObject> m_p_parent;
	//Child Object
	std::vector<std::weak_ptr<GameObject>> m_p_child_vector;

	bool m_dead_check = false;

	friend class Layer;
	friend class EventManager;
};

template<typename T>
constexpr ComponentType GameObject::GetComponentType()
{
	return ComponentType::NONE;
}

#define REGISTER_COMPONENT_TYPE(T, component_type) template<> ComponentType GameObject::GetComponentType<T>() { return component_type; }
REGISTER_COMPONENT_TYPE(Transform, ComponentType::Transform);
REGISTER_COMPONENT_TYPE(Camera, ComponentType::Camera);
REGISTER_COMPONENT_TYPE(Renderer, ComponentType::Transform);
REGISTER_COMPONENT_TYPE(Animator, ComponentType::Animator);
REGISTER_COMPONENT_TYPE(Script, ComponentType::Script);

template<typename T>
inline const std::shared_ptr<T>& GameObject::GetComponent()
{
	//Class T가 IComponent를 상속받는 클래스인지 확인
	auto result = std::is_base_of<IComponent, T>::value;
	assert(result);
	if (!result)
	{
		return nullptr;
	}

	//타입 T에 해당하는 Shader Type 반환
	auto component_type = GetComponentType<T>();
	auto component = GetComponent(component_type);

	if (component != nullptr)
	{
		return std::dynamic_pointer_cast<T>(component);
	}

	return nullptr;
}
