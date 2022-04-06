#include "stdafx.h"
#include "Layer.h"

#include "GameObject.h"

Layer::Layer(const UINT& layer_index)
	:m_layer_index()
{
}

Layer::~Layer()
{
	//Clear Parent GameObject Vector
	m_p_parent_game_object_vector.clear();
	m_p_parent_game_object_vector.shrink_to_fit();

	//Clear GameObject Vector
	m_p_game_object_vector.clear();
	m_p_game_object_vector.shrink_to_fit();
}

void Layer::Awake()
{
	for (const auto& p_parent_game_object : m_p_parent_game_object_vector)
		p_parent_game_object->Awake();
}

void Layer::Start()
{
	for (const auto& p_parent_game_object : m_p_parent_game_object_vector)
	{
		if (p_parent_game_object->m_is_active)
			p_parent_game_object->Start();
	}
}

void Layer::Update()
{
	for (const auto& p_parent_game_object : m_p_parent_game_object_vector)
	{
		if (p_parent_game_object->m_is_active)
			p_parent_game_object->Update();
	}
}

void Layer::FinalUpdate()
{
	for (const auto& p_parent_game_object : m_p_parent_game_object_vector)
	{
		if (p_parent_game_object->m_is_active)
			p_parent_game_object->FinalUpdate();
	}
}

void Layer::AddGameObject(GameObject* p_game_object, bool is_move)
{
	if (p_game_object == nullptr)
		return;

	m_p_parent_game_object_vector.emplace_back(p_game_object);
	p_game_object->m_game_object_layer = m_layer_index;

	std::queue<GameObject*> p_object_queue;
	p_object_queue.push(p_game_object);

	//자식 오브젝트 너비 탐색
	while (!p_object_queue.empty())
	{
		auto p_current_object = p_object_queue.front();
		p_object_queue.pop();

		auto p_child_vector = p_current_object->GetChilds();

		for (auto& child : p_child_vector)
		{
			if (is_move)
				child->m_game_object_layer = m_layer_index;

			else
			{
				if (child->m_game_object_layer == -1)
					child->m_game_object_layer = m_layer_index;
			}

			p_object_queue.push(child);
		}
	}
}

void Layer::RegisterGameObject(GameObject* p_game_object)
{
	//해당 GameObject 부모가 없는 최상위 오브젝트라면
	if (!p_game_object->HasParent())
		m_p_parent_game_object_vector.emplace_back(p_game_object);

	m_p_game_object_vector.emplace_back(p_game_object);
}

void Layer::DeregisterGameObject(GameObject* p_game_object)
{
	//해당 GameObject 부모가 없는 최상위 오브젝트라면
	if (!p_game_object->HasParent())
		DeregisterFromParentGameObject(p_game_object);

	std::vector<GameObject*>::iterator iter = m_p_game_object_vector.begin();

	for (; iter != m_p_game_object_vector.end();)
	{
		if ((*iter) == p_game_object)
		{
			iter = m_p_game_object_vector.erase(iter);
			return;
		}

		else
			++iter;
	}
}

void Layer::DeregisterFromParentGameObject(GameObject* p_game_object)
{
	std::vector<GameObject*>::iterator iter = m_p_parent_game_object_vector.begin();

	for (; iter != m_p_parent_game_object_vector.end();)
	{
		if ((*iter) == p_game_object)
		{
			iter = m_p_parent_game_object_vector.erase(iter);
			return;
		}

		else
			++iter;
	}
}

const bool Layer::GetLayerEmpty()
{
	if (m_p_parent_game_object_vector.empty() && m_p_game_object_vector.empty())
		return true;

	return false;
}
