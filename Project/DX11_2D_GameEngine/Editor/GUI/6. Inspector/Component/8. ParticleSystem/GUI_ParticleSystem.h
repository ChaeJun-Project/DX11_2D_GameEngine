#pragma once

#include "../GUI_Component.h"

class GUI_ParticleSystem final : public GUI_Component
{
public:
	GUI_ParticleSystem(const std::string& particle_system_gui_name);
	~GUI_ParticleSystem() = default;

	void Render() override;
};
