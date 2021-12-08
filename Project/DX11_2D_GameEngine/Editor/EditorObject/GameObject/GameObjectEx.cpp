#include "stdafx.h"
#include "GameObjectEx.h"

#include <DX11_2D_GameEngine_Lib/IComponent.h>
#include <DX11_2D_GameEngine_Lib/Script.h>

void GameObjectEx::FinalUpdate()
{
	//������Ʈ ���� ������Ʈ
	for (auto& component : m_component_list)
		component.second->FinalUpdate();

	//��ũ��Ʈ ������Ʈ
     

	//�ڽ� ������Ʈ ���� ������Ʈ(transform)
	for (auto& child : m_p_child_vector)
		child->FinalUpdate();

}