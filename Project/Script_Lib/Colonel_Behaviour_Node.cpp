#include "stdafx.h"
#include "Colonel_Behaviour_Node.h"

IdleNode::IdleNode(Colonel_Script* p_colonel_script, const float& col_time)
{
	m_p_colonel_script = p_colonel_script;
	m_col_time = col_time;
}

IdleNode::~IdleNode()
{
	m_p_colonel_script = nullptr;
}

const bool IdleNode::Run()
{
	
	
	
	return false;
}
