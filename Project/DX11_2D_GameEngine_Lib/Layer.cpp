#include "stdafx.h"
#include "Layer.h"

Layer::Layer(const UINT& layer_index)
	:m_layer_index(layer_index)
{
}

Layer::~Layer()
{
	for (auto& p_parent_game_object : m_p_parent_game_object_vector)
		SAFE_DELETE(p_parent_game_object);

	m_p_parent_game_object_vector.clear();
	m_p_parent_game_object_vector.shrink_to_fit();

	m_p_game_object_vector.clear();
	m_p_game_object_vector.shrink_to_fit();
}

void Layer::Start()
{
	for (auto& game_object : m_p_parent_game_object_vector)
		game_object->Start();
}

void Layer::Update()
{
	for (auto& game_object : m_p_parent_game_object_vector)
		game_object->Update();
}

void Layer::FinalUpdate()
{
	std::vector<GameObject*>::iterator iter = m_p_parent_game_object_vector.begin();

	for (; iter != m_p_parent_game_object_vector.end(); )
	{
		(*iter)->FinalUpdate();

		if ((*iter)->IsDead())
			iter = m_p_parent_game_object_vector.erase(iter);

		else
			++iter;
	}
}

void Layer::Render()
{
	for (auto& game_object : m_p_game_object_vector)
		game_object->Render();
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
