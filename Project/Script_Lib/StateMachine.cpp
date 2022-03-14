#include "stdafx.h"
#include "StateMachine.h"

void StateMachine::SetState(IState* p_state)
{
	//���� ������ ���°� ��ȿ���� �ʰų� ���� ���¿� ���ٸ�
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
