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
	//Class T�� IComponent�� ��ӹ޴� Ŭ�������� Ȯ��
	auto result = std::is_base_of<IComponent, T>::value;
	assert(result);
	if (!result)
	{
		return;
	}

	//Ÿ�� T�� �ش��ϴ� Component Type ��ȯ
	auto component_type = GetComponentType<T>();
	//un_map�� ���ο� Component �߰�
	//insert�� �����ϸ� pair_ib<iterator, bool> ��ȯ
	//iterator�� map �����͸� �����ϴ� �ݺ���, bool�� map�� ������ ���� ��������
	auto component_iter = this->m_component_un_map.insert(std::make_pair(component_type, std::make_shared<T>(this)));

	//�ش��ϴ� Type�� map�� ���������� �����Ͱ� �߰��� �Ǿ����� Ȯ��
	assert(component_iter.second);
}

template<typename T>
std::shared_ptr<T> GameObject::GetComponent()
{
	//Class T�� IComponent�� ��ӹ޴� Ŭ�������� Ȯ��
	auto result = std::is_base_of<IComponent, T>::value;
	assert(result);
	if (!result)
	{
		return nullptr;
	}
	//Ÿ�� T�� �ش��ϴ� Component Type ��ȯ
	auto component_type = GetComponentType<T>();
	auto component_iter = this->m_component_un_map.find(component_type);

	//�ش� Component Type�� Component �����Ͱ� �����ϴ� ���
	assert(component_iter != this->m_component_un_map.end());
	if (component_iter->second != nullptr)
		return std::dynamic_pointer_cast<T>(component_iter->second);

	return nullptr;
}

template<typename T>
void GameObject::RemoveComponent()
{
	//Class T�� IComponent�� ��ӹ޴� Ŭ�������� Ȯ��
	auto result = std::is_base_of<IComponent, T>::value;
	assert(result);
	if (!result)
	{
		return;
	}
	//Ÿ�� T�� �ش��ϴ� Component Type ��ȯ
	auto component_type = GetComponentType<T>();
	this->m_component_un_map.erase(component_type);
}
