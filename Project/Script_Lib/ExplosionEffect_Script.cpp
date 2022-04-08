#include "stdafx.h"
#include "ExplosionEffect_Script.h"

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Animator2D.h>

ExplosionEffect_Script::ExplosionEffect_Script()
	:Script("ExplosionEffect_Script")
{
}

ExplosionEffect_Script::ExplosionEffect_Script(const ExplosionEffect_Script& origin)
	: Script(origin.m_script_name)
{
	m_is_active = origin.m_is_active;
}

ExplosionEffect_Script::~ExplosionEffect_Script()
{
	m_p_animator2D = nullptr;
}

void ExplosionEffect_Script::Awake()
{
	m_p_animator2D = m_p_owner_game_object->GetComponent<Animator2D>();
}

void ExplosionEffect_Script::OnEnable()
{
	m_p_animator2D->SetIsLoop(true);
	m_p_animator2D->Play();
}

void ExplosionEffect_Script::OnDisable()
{
	m_p_animator2D->Stop();
}

void ExplosionEffect_Script::Update()
{
	if (m_p_animator2D == nullptr || !m_p_animator2D->GetIsPlaying())
		return;

	if(m_p_animator2D->GetCurrentAnimationIsFinished())
		m_p_owner_game_object->SetIsActive(false);
}

void ExplosionEffect_Script::SaveToScene(FILE* p_file)
{
	Script::SaveToScene(p_file); //Script
}
