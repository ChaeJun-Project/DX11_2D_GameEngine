#include "stdafx.h"
#include "GameObjectEx.h"

#include <DX11_2D_GameEngine_Lib/IComponent.h>
#include <DX11_2D_GameEngine_Lib/Script.h>

void GameObjectEx::FinalUpdate()
{
	//컴포넌트 최종 업데이트
	for (auto& component : m_p_component_map)
		component.second->FinalUpdate();

	//스크립트 업데이트
     

	//자식 오브젝트 최종 업데이트(transform)
	for (auto& child : m_p_child_vector)
		child->FinalUpdate();

}
