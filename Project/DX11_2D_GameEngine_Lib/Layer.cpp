#include "stdafx.h"
#include "Layer.h"

Layer::Layer(const UINT& layer_index)
	:m_layer_index(layer_index)
{
}

Layer::~Layer()
{
	this->m_p_parent_game_object_list.clear();

	this->m_p_game_object_list.clear();
}

void Layer::Update()
{
	this->m_p_game_object_list.clear();

	for (auto& game_object : this->m_p_parent_game_object_list)
		game_object->Update();
}

void Layer::LateUpdate()
{
	for (auto& game_object : this->m_p_parent_game_object_list)
		game_object->LateUpdate();
}

void Layer::FinalUpdate()
{
	std::list<std::shared_ptr<GameObject>>::iterator iter = this->m_p_parent_game_object_list.begin();

	for (; iter != this->m_p_parent_game_object_list.end(); )
	{
		(*iter)->FinalUpdate();

		if ((*iter)->IsDead())
			iter = this->m_p_parent_game_object_list.erase(iter);

		else
			++iter;
	}
}

void Layer::Render()
{
	for (auto& game_object : this->m_p_game_object_list)
		game_object.lock()->Render();
}

void Layer::AddGameObject(const std::shared_ptr<GameObject>& p_game_object, bool is_move)
{
	if (p_game_object == nullptr)
		return;

	this->m_p_parent_game_object_list.push_back(p_game_object);
	p_game_object->m_object_layer_index = m_layer_index;

	std::queue<std::shared_ptr<GameObject>> p_object_queue;
	p_object_queue.push(p_game_object);

	//자식 오브젝트 너비 탐색
	while (!p_object_queue.empty())
	{
	    auto p_current_object = p_object_queue.front();
		p_object_queue.pop();

		auto p_child_vector = p_current_object->GetChilds();
		
		for (auto& child : p_child_vector)
		{
			if(is_move)
				child.lock()->m_object_layer_index = m_layer_index;

			else
			{
				if(child.lock()->m_object_layer_index == -1)
					child.lock()->m_object_layer_index = m_layer_index;
			}

			p_object_queue.push(child.lock());
		}
	}
}
