#include "stdafx.h"
#include "StateMachine.h"

void StateMachine::SetState(IState* p_state)
{
	//새로 변경할 상태가 유효하지 않거나 이전 상태와 같다면
	if (p_state == nullptr || m_p_current_state == p_state)
		return;

	m_p_current_state->Exit();

	m_p_current_state = p_state;

	m_p_current_state->Enter();
}

void StateMachine::StateUpdate()
{
	if (m_p_current_state == nullptr)
		return;

	m_p_current_state->Update();
}
