#include "stdafx.h"
#include "GUI_ParticleSystem.h"

#include "Helper/IconProvider.h"

GUI_ParticleSystem::GUI_ParticleSystem(const std::string& particle_system_gui_name)
	:GUI_Component(particle_system_gui_name)
{
}

void GUI_ParticleSystem::Render()
{
	if (BeginComponent(m_component_gui_name, ComponentType::ParticleSystem, IconType::Component_ParticleSystem))
	{

		DrawComponentEnd();
	}
}
