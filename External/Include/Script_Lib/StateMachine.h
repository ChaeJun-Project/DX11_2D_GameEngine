#pragma once

#include "IState.h"

class StateMachine final
{
public:
	StateMachine(IState* p_start_state)
	:m_p_current_state(p_start_state)
	{
	}
	~StateMachine()
	{
		m_p_current_state = nullptr;
	}

public:
    void SetState(IState* p_state);

	void StateUpdate();

public:
    IState* m_p_current_state = nullptr;
};

