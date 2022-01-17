#include "stdafx.h"
#include "GUI_ParticleSystem.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/ParticleSystem.h>

GUI_ParticleSystem::GUI_ParticleSystem(const std::string& particle_system_gui_name)
	:GUI_Component(particle_system_gui_name)
{
}

void GUI_ParticleSystem::Render()
{
	if (BeginComponent(m_component_gui_name, ComponentType::ParticleSystem, IconType::Component_ParticleSystem))
	{
		auto particle_system = m_select_game_object->GetComponent<ParticleSystem>();
		if (particle_system == nullptr)
			return;

		DrawComponentEnd();
	}
}
