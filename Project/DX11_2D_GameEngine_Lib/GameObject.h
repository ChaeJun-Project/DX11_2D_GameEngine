#pragma once

#include "stdafx.h"

class IComponent;

class GameObject : public DX11Obejct
{
private:
	template<typename T>
	static constexpr ComponentType GetComponentType();

public:
	GameObject(const GameObjectType& game_object_type);
	~GameObject();

	void Update();
	void LateUpdate();
	void FinalUpdate();

	void Render();
public:
	template<typename T>
	void AddAndCreateComponent();

	template<typename T>
	std::shared_ptr<T> GetComponent();

	template<typename T>
	void RemoveComponent();

public:
	//Object Type
	const GameObjectType& GetGameObjectType() const { return this->m_game_object_type; }
	//Object Tag
	const std::string& GetObjectTag() const { return this->m_object_tag; }
	void SetObjectTag(const std::string& object_tag) { this->m_object_tag = object_tag; }
	//Object name
	const std::string& GetObjectName() const { return this->m_object_name; }
	void SetObjectName(const std::string& object_name) { this->m_object_name = object_name; }
	//Object Side
	const GameObjectSideState& GetObjectSideState() const { return this->m_game_object_side_state; }
	void SetObjectSideState(const GameObjectSideState& object_side_state) { this->m_game_object_side_state = object_side_state; }

protected:
	//GameObject Type
	const GameObjectType m_game_object_type;
	//Object Tag
	std::string m_object_tag;
	//Object name
	std::string m_object_name;
	//Object Side
	GameObjectSideState m_game_object_side_state = GameObjectSideState::Right;
	//Component Unordered Map
	std::unordered_map<ComponentType, std::shared_ptr<IComponent>> m_component_un_map;
};


template<typename T>
void GameObject::AddAndCreateComponent()
{
	//Class T가 IComponent를 상속받는 클래스인지 확인
	auto result = std::is_base_of<IComponent, T>::value;
	assert(result);
	if (!result)
	{
		return;
	}

	//타입 T에 해당하는 Component Type 반환
	auto component_type = GetComponentType<T>();
	//un_map에 새로운 Component 추가
	//insert에 성공하면 pair_ib<iterator, bool> 반환
	//iterator는 map 데이터를 참조하는 반복자, bool은 map에 데이터 삽입 성공여부
	auto component_iter = this->m_component_un_map.insert(std::make_pair(component_type, std::make_shared<T>(this)));

	//해당하는 Type의 map에 성공적으로 데이터가 추가가 되었는지 확인
	assert(component_iter.second);
}

template<typename T>
std::shared_ptr<T> GameObject::GetComponent()
{
	//Class T가 IComponent를 상속받는 클래스인지 확인
	auto result = std::is_base_of<IComponent, T>::value;
	assert(result);
	if (!result)
	{
		return nullptr;
	}
	//타입 T에 해당하는 Component Type 반환
	auto component_type = GetComponentType<T>();
	auto component_iter = this->m_component_un_map.find(component_type);

	//해당 Component Type의 Component 데이터가 존재하는 경우
	assert(component_iter != this->m_component_un_map.end());
	if (component_iter->second != nullptr)
		return std::dynamic_pointer_cast<T>(component_iter->second);

	return nullptr;
}

template<typename T>
void GameObject::RemoveComponent()
{
	//Class T가 IComponent를 상속받는 클래스인지 확인
	auto result = std::is_base_of<IComponent, T>::value;
	assert(result);
	if (!result)
	{
		return;
	}
	//타입 T에 해당하는 Component Type 반환
	auto component_type = GetComponentType<T>();
	this->m_component_un_map.erase(component_type);
}
