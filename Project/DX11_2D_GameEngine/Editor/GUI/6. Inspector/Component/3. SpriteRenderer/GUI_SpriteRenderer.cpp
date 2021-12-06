#include "stdafx.h"
#include "GUI_SpriteRenderer.h"

#include "GUI/GUI_ItemList.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/FileManager.h>

#include <DX11_2D_GameEngine_Lib/ResourceManager.h>
#include <DX11_2D_GameEngine_Lib/Texture.h>
#include <DX11_2D_GameEngine_Lib/Mesh.h>
#include <DX11_2D_GameEngine_Lib/Material.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/SpriteRenderer.h>

GUI_SpriteRenderer::GUI_SpriteRenderer(const std::string& sprite_renderer_gui_name)
	:GUI_Component(sprite_renderer_gui_name)
{
	m_p_item_list = new GUI_ItemList;
}

GUI_SpriteRenderer::~GUI_SpriteRenderer()
{
	SAFE_DELETE(m_p_item_list);
}

void GUI_SpriteRenderer::Render()
{
	auto sprite_renderer = m_select_game_object->GetComponent<SpriteRenderer>();
	if (sprite_renderer == nullptr)
		return;

	if (BeginComponent(m_component_gui_name, ComponentType::SpriteRenderer, IconType::Component_SpriteRenderer))
	{
		auto sprite_renderer = m_select_game_object->GetComponent<SpriteRenderer>();
		auto sprite_texture = sprite_renderer->GetSpriteTexture();
		auto material = sprite_renderer->GetMaterial();
		auto mesh = sprite_renderer->GetMesh();

		auto resource_manager = ResourceManager::GetInstance();

		//���� �Լ� show_texture_slot
		//������Ʈ�� ������ �ؽ�ó�� Inspectorâ���� �̹����� �����ִ� �Լ�
		const auto ShowTexture = [](const char* label_name, const std::shared_ptr<Texture>& p_texture)
		{
			ImGui::Text(label_name);
			ImGui::SameLine(80.0f);

			ImGui::PushItemWidth(200.0f);
			std::string sprite_name_str;
			if(p_texture != nullptr)
				sprite_name_str = p_texture->GetResourceName();
			std::string sprite_name_label_str = "##" + sprite_name_str;
			ImGui::InputText(sprite_name_label_str.c_str(), &sprite_name_str, ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::Image
			(
				p_texture ? p_texture->GetShaderResourceView() : nullptr,
				ImVec2(80.0f, 80.0f),
				ImVec2(0.0f, 0.0f),
				ImVec2(1.0f, 1.0f),
				ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
				ImColor(1.0f, 1.0f, 1.0f, 1.0f)
			);
		};

		const auto ShowCombo = [&resource_manager, &sprite_renderer, this](const char* label_name, const std::shared_ptr<IResource>& p_resource)
		{
			ImGui::Text(label_name);
			ImGui::SameLine(80.0f);

			ImGui::PushItemWidth(200.0f);
			std::string resource_name_str = p_resource->GetResourceName();
			std::string resource_name_label_str = "##" + resource_name_str;

			int index = 0;

			switch (p_resource->GetResourceType())
			{
			case ResourceType::Material:
			{
				const auto material_map = resource_manager->GetMaterialMap();

				for (auto& material : material_map)
				{
					m_p_item_list->AddItem(material.second->GetResourceName());

					if (material.second->GetResourceName() == resource_name_str)
						m_p_item_list->SetCurrentListID(index);

					else
						++index;
				}
			}
			break;
			case ResourceType::Mesh:
			{
				const auto& mesh_map = resource_manager->GetMeshMap();

				for (auto& mesh : mesh_map)
				{
					m_p_item_list->AddItem(mesh.second->GetResourceName());

					if (mesh.second->GetResourceName() == resource_name_str)
						m_p_item_list->SetCurrentListID(index);

					else
						++index;
				}
			}
			break;
			}

			const auto& item_list_vector = m_p_item_list->GetItemList();

			if (ImGui::BeginCombo(resource_name_label_str.c_str(), resource_name_str.c_str()))
			{
				for (UINT i = 0; i < static_cast<UINT>(item_list_vector.size()); ++i)
				{
					const bool is_selected = (*(m_p_item_list->GetCurrentListID()) == i);
					if (ImGui::Selectable(item_list_vector[i].c_str(), is_selected))
					{
						SelectResource(sprite_renderer, p_resource, item_list_vector[i]);
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			m_p_item_list->ClearItemList();
		};

		//Texture
		ShowTexture("Sprite", sprite_texture);

		//Material
		ShowCombo("Material", material);

		//Mesh
		ShowCombo("Mesh", mesh);

		DrawComponentEnd();
	}

}

void GUI_SpriteRenderer::SelectResource(SpriteRenderer* p_sprite_renderer, const std::shared_ptr<IResource>& p_resource, const std::string& selected_item)
{
	auto resource_manager = ResourceManager::GetInstance();

	switch (p_resource->GetResourceType())
	{
	case ResourceType::Material:
	{
		const auto material_map = resource_manager->GetMaterialMap();

		for (auto& material : material_map)
		{
			if (material.second->GetResourceName() == selected_item)
			{
				p_sprite_renderer->SetMaterial(std::dynamic_pointer_cast<Material>(material.second));
				return;
			}
		}

	}
	break;
	case ResourceType::Mesh:
	{
		const auto& mesh_map = resource_manager->GetMeshMap();

		for (auto& mesh : mesh_map)
		{
			if (mesh.second->GetResourceName() == selected_item)
			{
				p_sprite_renderer->SetMesh(std::dynamic_pointer_cast<Mesh>(mesh.second));
				return;
			}
		}
	}
	break;
	}
}

