#pragma once

#include "stdafx.h" 
#include "BehaviourTree.h"

class Colonel_Script;
class Transform;

class Property
{
protected:
	Colonel_Script* m_p_colonel_script = nullptr;
	float m_col_time = 0.0f;
};

class IdleNode final : public Node, Property
{
public:
	IdleNode(Colonel_Script* p_colonel_script, const float& col_time);
	~IdleNode();

public:
	const bool Run() override;
};