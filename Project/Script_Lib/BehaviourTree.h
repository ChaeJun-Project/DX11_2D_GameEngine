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
//Selector�� ���� ��� �� �ϳ��� true��� true ��ȯ(�ϳ��� �����Ѵٸ� ����)
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
//Sequence�� ���� ��� �� �ϳ��� false��� false��ȯ
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