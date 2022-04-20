#pragma once

class Node
{
public:
    Node() = default;
	virtual ~Node() = default;

public:
    virtual void Tick() {}
	virtual const bool Update() = 0;
};

#include <vector>

class CompositeNode : public Node
{
public:
	CompositeNode() = default;
	virtual ~CompositeNode()
	{
		for (auto& p_child_node : m_child_node_vector)
		{
		    SAFE_DELETE(p_child_node);
	    }
		m_child_node_vector.clear();
		m_child_node_vector.shrink_to_fit();
	}

public:
	void Tick() override
	{
		for (auto& p_node : m_child_node_vector)
		{
			p_node->Tick();
		}
	}

	void AddChild(Node* p_node)
	{
		if (p_node == nullptr)
			return;

		m_child_node_vector.emplace_back(p_node);
	}

	const std::vector<Node*>& GetChildren() { return m_child_node_vector; }

protected:
	std::vector<Node*> m_child_node_vector;
};

//<summary>
//Selector의 하위 노드 중 하나라도 true라면 true 반환(하나라도 동작한다면 수행)
//</summary>
class Selector final : public CompositeNode
{
public:
	Selector() = default;
	virtual ~Selector() = default;

public:
	const bool Update() override
	{
		for (auto& p_node : m_child_node_vector)
		{
			if (p_node->Update())
				return true;
		}
		return false;
	}
};

#include <random>
#include <algorithm>
class RandomSelector final : public CompositeNode
{
public:
	RandomSelector() = default;
	virtual ~RandomSelector() = default;

public:
	const bool Update() override
	{
		ShuffleVector();

		for (auto& p_node : m_child_node_vector)
		{
			if (p_node->Update())
				return true;
		}
		return false;
	}

private:
	void ShuffleVector() //자식 노드 무작위로 섞기
	{
		std::random_device random_device;
		std::default_random_engine random_engine(random_device());
		std::shuffle(std::begin(m_child_node_vector), std::end(m_child_node_vector), random_engine);
	}
};

//<summary>
//Sequence의 하위 노드 중 하나라도 false라면 false반환
//</summary>
class Sequence final : public CompositeNode
{
public:
	Sequence() = default;
	~Sequence() = default;

public:
	const bool Update() override
	{
		for (auto& p_node : m_child_node_vector)
		{
			if (!p_node->Update())
				return false;
		}
		return true;
	}
};

//<summary>
//Decorator는 하나의 자식 노드만 가질 수 있음. 조건을 판별하여 자식 노드를 수행할지 말지 결정
//</summary>
class Decorator : public Node
{
public:
	Decorator() = default;
	virtual ~Decorator()
	{
		SAFE_DELETE(m_p_child_node);
	}

public:
	virtual const bool Update() = 0;
	
public:
	void AddChild(Node* p_node)
	{
		if (p_node == nullptr)
			return;

		m_p_child_node = p_node;
	}

protected:
	Node* m_p_child_node = nullptr;
};