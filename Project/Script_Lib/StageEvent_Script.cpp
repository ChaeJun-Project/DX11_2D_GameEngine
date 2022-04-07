#include "stdafx.h"
#include "StageEvent_Script.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>

StageEvent_Script::StageEvent_Script()
	:Script("StageEvent_Script")
{
}

StageEvent_Script::StageEvent_Script(const StageEvent_Script& origin)
	: Script(origin.m_script_name)
{
	m_is_active = origin.m_is_active;
}

StageEvent_Script::~StageEvent_Script()
{
	m_p_event_func = nullptr;
}

void StageEvent_Script::OnCollisionExit(GameObject* p_other_game_object)
{
	if (p_other_game_object->GetGameObjectTag()._Equal("Player"))
	{
		if (m_p_event_func == nullptr)
			return;

		m_p_event_func();

		m_p_owner_game_object->SetIsActive(false);
	}
}

void StageEvent_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script
}