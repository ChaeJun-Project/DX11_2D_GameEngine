#pragma once

class Node
{
public:
	virtual const bool Run() = 0;  
};

#include <vector>

class CompositeNode : public Node
{
public:
	void AddChild(Node* p_node)
	{
		if (p_node == nullptr)
			return;

		m_child_node_list.emplace_back(p_node);
	}

	const std::vector<Node*>& GetChildren() { return m_child_node_list; }

protected:
	std::vector<Node*> m_child_node_list;
};

//<summary>
//Selector의 하위 노드 중 하나라도 true라면 true 반환(하나라도 동작한다면 수행)
//</summary>
class Selector final : public CompositeNode
{
public:
	const bool Run() override
	{
		for (auto& p_node : m_child_node_list)
		{
			if (p_node->Run())
				return true;
		}
		return false;
	}
};

//<summary>
//Sequence의 하위 노드 중 하나라도 false라면 false반환
//</summary>
class Sequence final : public CompositeNode
{
public:
	const bool Run() override
	{
		for (auto& p_node : m_child_node_list)
		{
			if (!p_node->Run())
				return false;
		}
		return true;
	}
};