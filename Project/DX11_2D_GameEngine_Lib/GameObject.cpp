#include "stdafx.h"
#include "GameObject.h"

#include "Layer.h"

#include "SceneManager.h"
#include "Scene.h"

GameObject::GameObject(const GameObject& origin_game_object)
{
	for (auto& origin_component : origin_game_object.m_component_list)
	{
		AddComponent(origin_component.second->Clone());
	}

	for (auto& child : origin_game_object.m_p_child_vector)
	{
		AddChild(std::make_shared<GameObject>(child.lock()->Clone()));
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

	//������Ʈ ������Ʈ
	for (auto& component : this->m_component_list)
		component.second->Update();

    //�ڽ� ������Ʈ ������Ʈ
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
	//������Ʈ ���� ������Ʈ
	for (auto& component : this->m_component_list)
		component.second->FinalUpdate();

	//�ڽ� ������Ʈ ���� ������Ʈ(transform)
	for (auto& child : this->m_p_child_vector)
		child.lock()->FinalUpdate();

	//Layer�� ���
	auto current_scene = SceneManager::GetInstance()->GetCurrentScene();
	auto layer = current_scene->GetLayer(static_cast<UINT>(this->m_object_layer_index));
	layer->RegisterObject(std::make_shared<GameObject>(this));
}

void GameObject::Render()
{
	auto renderer = std::dynamic_pointer_cast<Renderer>(GetComponent(ComponentType::Renderer));

	if (renderer == nullptr)
		return;

	renderer->Render();
}

//������ �����ʹ� ������ �̸� �����ϰ� �ִ� ���� ������Ʈ�� �����ؾ� ��
void GameObject::AddComponent(const std::shared_ptr<IComponent>& p_component)
{
	auto this_game_object = std::make_shared<GameObject>(this);
	p_component->SetGameObject(this_game_object);
	this->m_component_list.push_back
	(
		std::make_pair(p_component->GetComponentType(), p_component)
	);
	std::cout << m_object_name <<"���� Ƚ��" << this_game_object.use_count() << std::endl;
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

//TODO: ��ũ��Ʈ�� �� �� �̻� ������ ��� ������ ��� ���� ���
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
		return this->m_p_parent.lock()->GetRoot();
	}

	auto this_game_object = std::make_shared<GameObject>(this);
	return this_game_object;
}

void GameObject::SetParent(const std::shared_ptr<GameObject>& p_parent_game_object)
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
		if (this->m_p_parent.lock()->GetObjectID() == p_parent_game_object->GetObjectID())
			return;
	}

	auto old_parenet = this->m_p_parent.lock();
	this->m_p_parent = p_parent_game_object;

	if (old_parenet != nullptr)
		old_parenet->TachChild();

	if (HasParent())
		this->m_p_parent.lock()->TachChild();
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
	//��ȿ�� ���� �ƴ� ���
	if (p_child_game_object == nullptr)
		return;

	//���ڷ� ���� �ڽ� ������Ʈ�� ���� �ڱ� �ڽ��� ���
	if (this->GetObjectID() == p_child_game_object->GetObjectID())
		return;

	this->m_p_child_vector.emplace_back(p_child_game_object);

	auto this_game_object = std::make_shared<GameObject>(this);
	p_child_game_object->m_p_parent = this_game_object;
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
