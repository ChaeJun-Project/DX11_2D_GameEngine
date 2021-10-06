#pragma once

#include "stdafx.h"

class IComponent;

class IObject : public DX11Obejct
{
public:
	IObject(const GameObjectType& game_object_type)
		:m_game_object_type(game_object_type)
	{

	}
	virtual ~IObject() = default;

	virtual void Initialize() = 0;

	virtual void Update() = 0;
	virtual void LateUpdate() {}
	virtual void FinalUpdate() = 0;

	virtual void Render() = 0;

private:
	template<typename T>
	static constexpr ComponentType GetComponentType();

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
void IObject::AddAndCreateComponent()
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
std::shared_ptr<T> IObject::GetComponent()
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
void IObject::RemoveComponent()
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
