#include "stdafx.h"
#include "GUI_ParticleRenderer.h"

#include "GUI/Module/ItemList/GUI_ItemList.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/Texture.h>
#include <DX11_2D_GameEngine_Lib/Mesh.h>
#include <DX11_2D_GameEngine_Lib/Material.h>
#include <DX11_2D_GameEngine_Lib/Particle.h>

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/ParticleRenderer.h>

GUI_ParticleRenderer::GUI_ParticleRenderer(const std::string& particle_renderer_gui_name)
	:GUI_Component(particle_renderer_gui_name)
{
	m_p_item_list = std::make_unique<GUI_ItemList>();
}

GUI_ParticleRenderer::~GUI_ParticleRenderer()
{
	m_p_item_list.reset();
}

void GUI_ParticleRenderer::Render()
{
	auto particle_system = m_p_selected_game_object->GetComponent<ParticleRenderer>();
	if (particle_system == nullptr)
		return;

	auto is_active = particle_system->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::ParticleRenderer, is_active, IconType::Component_ParticleRenderer))
	{
		auto particle_system = m_p_selected_game_object->GetComponent<ParticleRenderer>();
		if (particle_system == nullptr)
			return;

		particle_system->SetIsActive(is_active);

		//Particle Renderer Property
		//Particle
		ShowParticleCombo("Particle", particle_system);

		//Particle Property
		ShowParticleProperty(particle_system);

		//Particle Texture
		ShowTexture("Texture", particle_system);

		//Material
		ShowMaterialCombo("Material", particle_system);

		//Mesh
		ShowMeshCombo("Mesh", particle_system);

		DrawComponentEnd();
	}
}

void GUI_ParticleRenderer::ShowParticleCombo(const std::string& label_name, ParticleRenderer* p_particle_renderer)
{
	ImGui::Text(label_name.c_str());
	ImGui::SameLine(100.0f);

	ImGui::PushItemWidth(200.0f);
	auto p_particle = p_particle_renderer->GetCurrentParticle();
	std::string particle_name;
	if (p_particle != nullptr)
		particle_name = p_particle->GetResourceName();

	int index = 0;

	const auto particle_map = RESOURCE_MANAGER->GetResourceMap(ResourceType::Particle);

	for (const auto& particle : particle_map)
	{
		m_p_item_list->AddItem(particle.second->GetResourceName());

		if (particle.second->GetResourceName()._Equal(particle_name))
			m_p_item_list->SetCurrentListID(index);

		else
			++index;
	}

	const auto& item_list_vector = m_p_item_list->GetItemList();

	if (ImGui::BeginCombo("##Particle Comobo", particle_name.c_str()))
	{
		for (UINT i = 0; i < static_cast<UINT>(item_list_vector.size()); ++i)
		{
			const bool is_selected = (m_p_item_list->GetCurrentListID() == i);
			if (ImGui::Selectable(item_list_vector[i].c_str(), is_selected))
			{
				if (CAN_EDIT)
				{
					p_particle_renderer->SetCurrentParticle(RESOURCE_MANAGER->GetResource<Particle>(item_list_vector[i]));
				}
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	m_p_item_list->ClearItemList();

	if (auto pay_load = DragDropEvent::ReceiveDragDropPayLoad(PayLoadType::Particle))
	{
		p_particle_renderer->SetCurrentParticle(RESOURCE_MANAGER->LoadFromFile<Particle>(std::get<std::string>(pay_load->data)));
	}
}

void GUI_ParticleRenderer::ShowParticleProperty(ParticleRenderer* p_particle_renderer)
{
	if (p_particle_renderer->GetCurrentParticle() != nullptr)
	{
		//Particle Activable Count
		auto activable_count = static_cast<int>(p_particle_renderer->GetParticleActivableCount());
		ShowInt("Particle", "Activable Count", activable_count, 80.0f, 110.0f);

		//Particle Max Count
		auto max_count = static_cast<int>(p_particle_renderer->GetParticleMaxCount());
		ShowInt("Particle", "Max Count", max_count, 80.0f, 110.0f);

		//Particle Spawn Range
		auto spawn_range = p_particle_renderer->GetParticleSpawnRange();
		ShowFloat3("Spawn Range", spawn_range, 60.0f, 100.0f);
		
		//Particle Scale
		auto start_scale = p_particle_renderer->GetParticleStartScale();
		ShowFloat3("Start Scale", start_scale, 60.0f, 100.0f);

		auto end_scale = p_particle_renderer->GetParticleEndScale();
		ShowFloat3("End Scale", end_scale, 60.0f, 100.0f);

		//Particle Color
		auto start_color = p_particle_renderer->GetParticleStartColor();
		ShowColorPicker4("Start Color", start_color, ImGuiColorEditFlags_AlphaPreview);

		auto end_color = p_particle_renderer->GetParticleEndColor();
		ShowColorPicker4("End Color", end_color, ImGuiColorEditFlags_AlphaPreview);

		//Particle Speed
		auto min_speed = p_particle_renderer->GetParticleMinSpeed();
		ShowFloat("Particle", "Min Speed", min_speed, 80.0f, 110.0f);

		auto max_speed = p_particle_renderer->GetParticleMaxSpeed();
		ShowFloat("Particle", "Max Speed", max_speed, 80.0f, 110.0f);

		//Particle Life
		auto min_life = p_particle_renderer->GetParticleMinLife();
		ShowFloat("Particle", "Min Life", min_life, 80.0f, 110.0f);

		auto max_life = p_particle_renderer->GetParticleMaxLife();
		ShowFloat("Particle", "Max Life", max_life, 80.0f, 110.0f);

		//Particle Spawn Frequency
		auto spawn_frequency = p_particle_renderer->GetParticleSpawnFrequency();
		ShowFloat("Particle", "Spawn Frequency", spawn_frequency, 80.0f, 110.0f);


		if (CAN_EDIT)
		{
			//Particle Activable Count
			p_particle_renderer->SetParticleActivableCount(static_cast<UINT>(activable_count));
			//Particle Max Count
			p_particle_renderer->SetParticleMaxCount(static_cast<UINT>(max_count));
		
			//Particle Spawn Range
			p_particle_renderer->SetParticleSpawnRange(spawn_range);

			//Particle Scale
			p_particle_renderer->SetParticleStartScale(start_scale);
			p_particle_renderer->SetParticleEndScale(end_scale);

			//Particle Color
			p_particle_renderer->SetParticleStartColor(start_color);
			p_particle_renderer->SetParticleEndColor(end_color);
		
			//Particle Speed
			p_particle_renderer->SetParticleMinSpeed(min_speed);
			p_particle_renderer->SetParticleMaxSpeed(max_speed);

			//Particle Life
			p_particle_renderer->SetParticleMinLife(min_life);
			p_particle_renderer->SetParticleMaxLife(max_life);
		
			//Particle Spawn Frequency
			p_particle_renderer->SetParticleSpawnFrequency(spawn_frequency);
		}
	}
}

void GUI_ParticleRenderer::ShowTexture(const std::string& label_name, ParticleRenderer* p_particle_renderer)
{
	ImGui::Text(label_name.c_str());
	ImGui::SameLine(100.0f);

	ImGui::PushItemWidth(200.0f);
	auto p_texture = p_particle_renderer->GetCurrentParticleTexture();
	std::string texture_name;
	if (p_texture != nullptr)
		texture_name = p_texture->GetResourceName();
	std::string texture_name_label = "##" + texture_name;
	ImGui::InputText(texture_name_label.c_str(), &texture_name, ImGuiInputTextFlags_ReadOnly);
	ImGui::PopItemWidth();

	if (auto pay_load = DragDropEvent::ReceiveDragDropPayLoad(PayLoadType::Texture))
	{
		p_particle_renderer->SetCurrentParticleTexture(RESOURCE_MANAGER->LoadFromFile<Texture>(std::get<std::string>(pay_load->data)));
	}

	ImGui::Image
	(
		p_texture ? p_texture->GetShaderResourceView() : nullptr,
		ImVec2(80.0f, 80.0f),
		ImVec2(0.0f, 0.0f),
		ImVec2(1.0f, 1.0f),
		ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
		ImColor(1.0f, 1.0f, 1.0f, 1.0f)
	);
}

void GUI_ParticleRenderer::ShowMaterialCombo(const std::string& label_name, ParticleRenderer* p_particle_renderer)
{
	ImGui::Text(label_name.c_str());
	ImGui::SameLine(100.0f);

	ImGui::PushItemWidth(200.0f);
	auto p_material = p_particle_renderer->GetMaterial();
	std::string material_name;
	if (p_material != nullptr)
		material_name = p_material->GetResourceName();

	int index = 0;

	const auto material_map = RESOURCE_MANAGER->GetResourceMap(ResourceType::Material);

	for (const auto& material : material_map)
	{
		m_p_item_list->AddItem(material.second->GetResourceName());

		if (material.second->GetResourceName()._Equal(material_name))
			m_p_item_list->SetCurrentListID(index);

		else
			++index;
	}

	const auto& item_list_vector = m_p_item_list->GetItemList();

	if (ImGui::BeginCombo("##Material Comobo", material_name.c_str()))
	{
		for (UINT i = 0; i < static_cast<UINT>(item_list_vector.size()); ++i)
		{
			const bool is_selected = (m_p_item_list->GetCurrentListID() == i);
			if (ImGui::Selectable(item_list_vector[i].c_str(), is_selected))
			{
				if (CAN_EDIT)
				{
					p_particle_renderer->SetMaterial(RESOURCE_MANAGER->GetResource<Material>(item_list_vector[i]));
				}
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	m_p_item_list->ClearItemList();
}

void GUI_ParticleRenderer::ShowMeshCombo(const std::string& label_name, ParticleRenderer* p_particle_renderer)
{
	ImGui::Text(label_name.c_str());
	ImGui::SameLine(100.0f);

	ImGui::PushItemWidth(200.0f);
	auto p_mesh = p_particle_renderer->GetMesh();
	std::string mesh_name;
	if (p_mesh != nullptr)
		mesh_name = p_mesh->GetResourceName();

	int index = 0;

	const auto& mesh_map = RESOURCE_MANAGER->GetResourceMap(ResourceType::Mesh);

	for (const auto& mesh : mesh_map)
	{
		m_p_item_list->AddItem(mesh.second->GetResourceName());

		if (mesh.second->GetResourceName()._Equal(mesh_name))
			m_p_item_list->SetCurrentListID(index);

		else
			++index;
	}

	const auto& item_list_vector = m_p_item_list->GetItemList();

	if (ImGui::BeginCombo("##Mesh Combo", mesh_name.c_str()))
	{
		for (UINT i = 0; i < static_cast<UINT>(item_list_vector.size()); ++i)
		{
			const bool is_selected = (m_p_item_list->GetCurrentListID() == i);
			if (ImGui::Selectable(item_list_vector[i].c_str(), is_selected))
			{
				if (CAN_EDIT)
				{
					p_particle_renderer->SetMesh(RESOURCE_MANAGER->GetResource<Mesh>(item_list_vector[i]));
				}
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	m_p_item_list->ClearItemList();
}
