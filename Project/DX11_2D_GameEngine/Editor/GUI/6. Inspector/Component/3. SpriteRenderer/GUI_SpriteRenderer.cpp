#include "stdafx.h"
#include "GUI_SpriteRenderer.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/FileManager.h>

#include <DX11_2D_GameEngine_Lib/Texture.h>
#include <DX11_2D_GameEngine_Lib/Mesh.h>
#include <DX11_2D_GameEngine_Lib/Material.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/SpriteRenderer.h>

GUI_SpriteRenderer::GUI_SpriteRenderer(const std::string& sprite_renderer_gui_name)
	:GUI_Component(sprite_renderer_gui_name)
{
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
		
		//람다 함수 show_texture_slot
		//오브젝트에 적용한 텍스처를 Inspector창에서 이미지로 보여주는 함수
		const auto ShowTexture = [](const char* label_name, const std::shared_ptr<Texture>& texture)
		{
			ImGui::Text(label_name);
			ImGui::SameLine(80.0f);

			ImGui::PushItemWidth(150.0f);
			std::string sprite_name_str = texture->GetResourceName();
			std::string sprite_name_label_str = "##" + sprite_name_str;
			ImGui::InputText(sprite_name_label_str.c_str(), &sprite_name_str, ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::Image
			(
				texture ? texture->GetShaderResourceView() : nullptr,
				ImVec2(80.0f, 80.0f),
				ImVec2(0.0f, 0.0f),
				ImVec2(1.0f, 1.0f),
				ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
				ImColor(1.0f, 1.0f, 1.0f, 1.0f)
			);
		};

		const auto ShowText = [](const char* label_name, const std::shared_ptr<IResource>& resource)
		{
			ImGui::Text(label_name);
			ImGui::SameLine(80.0f);

			ImGui::PushItemWidth(150.0f);
			std::string resource_name_str = resource->GetResourceName();
			std::string resource_name_label_str = "##" + resource_name_str;
			ImGui::InputText(resource_name_label_str.c_str(), &resource_name_str, ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();
		};

		//Texture
		ShowTexture("Sprite", sprite_texture);

		//Material
		ShowText("Material", material);

		//Mesh
		ShowText("Mesh", mesh);

		DrawComponentEnd();
	}

}
