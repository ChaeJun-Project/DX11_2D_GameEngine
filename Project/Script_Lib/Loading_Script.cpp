#include "stdafx.h"
#include "Loading_Script.h"

Loading_Script::Loading_Script()
	:Script("Loading_Script")
{
}

Loading_Script::Loading_Script(const Loading_Script& origin)
	: Script(origin.m_script_name)
{
	m_is_active = origin.m_is_active;
}

Loading_Script::~Loading_Script()
{
}

void Loading_Script::Awake()
{
}

void Loading_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script
}

void Loading_Script::LoadFromScene(FILE* p_file)
{
}