#pragma once

#include "../GUI_Component.h"

class GUI_ItemList;

class ParticleRenderer;

class GUI_ParticleRenderer final : public GUI_Component
{
public:
	GUI_ParticleRenderer(const std::string& particle_system_gui_name);
	~GUI_ParticleRenderer();

	void Render() override;

private:
    void ShowParticleCombo(const std::string& label_name, ParticleRenderer* p_particle_renderer);
	void ShowParticleProperty(ParticleRenderer* p_particle_renderer);
	void ShowTexture(const std::string& label_name, ParticleRenderer* p_particle_renderer);
	void ShowMaterialCombo(const std::string& label_name, ParticleRenderer* p_particle_renderer);
	void ShowMeshCombo(const std::string& label_name, ParticleRenderer* p_particle_renderer);

private:
   std::unique_ptr<GUI_ItemList> m_p_item_list = nullptr;
};
