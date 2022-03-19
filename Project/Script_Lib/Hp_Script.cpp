#include "stdafx.h"
#include "Hp_Script.h"

#include "Z_Script.h"
#include "Colonel_Script.h"
#include "GameObjectController.h"

#include <DX11_2D_GameEngine_Lib/Scene.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/ImageRenderer.h>

Hp_Script::Hp_Script()
	:Script("Hp_Script")
{
	RegisterScriptParamData();
}

Hp_Script::Hp_Script(const Hp_Script& origin)
	: Script(origin.m_script_name)
{
	RegisterScriptParamData();

	m_is_active = origin.m_is_active;
}

Hp_Script::~Hp_Script()
{
	p_image_renderer = nullptr;

	m_p_game_object_contorller = nullptr;
}

void Hp_Script::Start()
{
	p_image_renderer = m_p_owner_game_object->GetComponent<ImageRenderer>();

	auto p_game_object = SCENE_MANAGER->GetCurrentScene()->FindGameObjectWithName(m_game_object_name);

	if (p_game_object == nullptr)
		return;

	if (p_game_object->GetGameObjectName()._Equal("Z"))
	{
		auto p_script = p_game_object->GetScript("Z_Script");
		m_p_game_object_contorller = dynamic_cast<Z_Script*>(p_script); //다운 캐스팅 후 업 캐스팅
	}

	else if (p_game_object->GetGameObjectName()._Equal("X"))
	{
		//TODO
	}

	else if (p_game_object->GetGameObjectName()._Equal("Colonel"))
	{
		auto p_script = p_game_object->GetScript("Colonel_Script");
		m_p_game_object_contorller = dynamic_cast<Colonel_Script*>(p_script); //다운 캐스팅 후 업 캐스팅
	}

	//Hp 설정
	if(m_p_game_object_contorller != nullptr)
	{
		m_game_object_total_hp = static_cast<UINT>(m_p_game_object_contorller->GetHp());
		m_game_object_current_hp = m_game_object_total_hp;
	}
}

void Hp_Script::Update()
{
	if (m_p_game_object_contorller == nullptr)
		return;

	m_game_object_current_hp = static_cast<UINT>(m_p_game_object_contorller->GetHp());

	float current_hp = static_cast<float>(m_game_object_current_hp);
	float total_hp = static_cast<float>(m_game_object_total_hp);

	auto ratio = static_cast<float>(current_hp / total_hp);
	p_image_renderer->SetFillAmount(ratio);
}

void Hp_Script::RegisterScriptParamData()
{
	AddScriptParamData(ScriptParamStruct("GameObject", ScriptParamType::String, reinterpret_cast<void*>(&m_game_object_name), 100.0f));
	AddScriptParamData(ScriptParamStruct("Script", ScriptParamType::String, reinterpret_cast<void*>(&m_script_name), 100.0f));

	AddScriptParamData(ScriptParamStruct("Total Hp", ScriptParamType::Int, reinterpret_cast<void*>(&m_game_object_total_hp), 100.0f));
	AddScriptParamData(ScriptParamStruct("Current Hp", ScriptParamType::Int, reinterpret_cast<void*>(&m_game_object_current_hp), 100.0f));
}

void Hp_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script

	//GameObject Name
	fprintf(p_file, "[GameObject Name]\n");
	fprintf(p_file, "%s\n", m_game_object_name.c_str());

	//Script Name
	fprintf(p_file, "[Script Name]\n");
	fprintf(p_file, "%s\n", m_script_name.c_str());
}

void Hp_Script::LoadFromScene(FILE* p_file)
{
	char char_buffer[256] = { 0 };

	//GameObject Name
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf(char_buffer, p_file);
	m_game_object_name = std::string(char_buffer);

	//Script Name
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf(char_buffer, p_file);
	m_script_name = std::string(char_buffer);
}
