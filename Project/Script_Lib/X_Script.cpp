#include "stdafx.h"
#include "X_Script.h"

X_Script::X_Script()
	:Script("X_Script")
{
}

X_Script::X_Script(const X_Script& origin)
	: Script(origin.m_script_name)
{
	m_is_active = origin.m_is_active;
}

X_Script::~X_Script()
{
}

void X_Script::Start()
{
}

void X_Script::Update()
{
}

void X_Script::OnCollisionEnter(GameObject* other_game_object)
{
}

void X_Script::OnCollisionStay(GameObject* other_game_object)
{
}

void X_Script::OnCollisionExit(GameObject* other_game_object)
{
}

void X_Script::SaveToScene(FILE* p_file)
{
}

void X_Script::LoadFromScene(FILE* p_file)
{
}
