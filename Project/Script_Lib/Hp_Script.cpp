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
	p_hp_gage_image_renderer = nullptr;

	m_p_game_object_contorller = nullptr;
}

void Hp_Script::Awake()
{
	p_hp_gage_image_renderer = m_p_owner_game_object->GetChildFromIndex(0)->GetComponent<ImageRenderer>();
}

void Hp_Script::Update()
{
	if (m_p_game_object_contorller == nullptr)
		return;

	m_game_object_current_hp = static_cast<UINT>(m_p_game_object_contorller->GetHp());

	float current_hp = static_cast<float>(m_game_object_current_hp);
	float total_hp = static_cast<float>(m_game_object_total_hp);

	auto ratio = static_cast<float>(current_hp / total_hp);
	p_hp_gage_image_renderer->SetFillAmount(ratio);
}

void Hp_Script::RegisterScriptParamData()
{
	AddScriptParamData(ScriptParamStruct("Total Hp", ScriptParamType::Int, reinterpret_cast<void*>(&m_game_object_total_hp), 100.0f));
	AddScriptParamData(ScriptParamStruct("Current Hp", ScriptParamType::Int, reinterpret_cast<void*>(&m_game_object_current_hp), 100.0f));
}

void Hp_Script::SetTargetGameObject(GameObject* p_target_game_object)
{
	if (p_target_game_object == nullptr)
		return;

	m_p_target_game_object = p_target_game_object;

	if (m_p_target_game_object->GetGameObjectName()._Equal("Z"))
	{
		auto p_script = m_p_target_game_object->GetScript("Z_Script");
		m_p_game_object_contorller = dynamic_cast<Z_Script*>(p_script); //다운 캐스팅 후 업 캐스팅
	}

	else if (m_p_target_game_object->GetGameObjectName()._Equal("X"))
	{
		//TODO
	}

	else if (m_p_target_game_object->GetGameObjectName()._Equal("Colonel"))
	{
		auto p_script = m_p_target_game_object->GetScript("Colonel_Script");
		m_p_game_object_contorller = dynamic_cast<Colonel_Script*>(p_script); //다운 캐스팅 후 업 캐스팅
	}

	//Hp 설정
	if (m_p_game_object_contorller != nullptr)
	{
		m_game_object_total_hp = static_cast<UINT>(m_p_game_object_contorller->GetHp());
		m_game_object_current_hp = m_game_object_total_hp;

		p_hp_gage_image_renderer->SetFillAmount(1.0f);
	}
}

void Hp_Script::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //Script
}