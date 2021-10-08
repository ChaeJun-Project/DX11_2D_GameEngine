#include "stdafx.h"
#include "IScript.h"

IScript::IScript(GameObject* p_game_object)
	:IComponent(ComponentType::Script, p_game_object)
{
}

void IScript::Update()
{
}

void IScript::FinalUpdate()
{
}
