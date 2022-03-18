#include "stdafx.h"
#include "GUI_ImageRenderer.h"

#include "Helper/IconProvider.h"

#include <DX11_2D_GameEngine_Lib/FileManager.h>

#include <DX11_2D_GameEngine_Lib/ResourceManager.h>
#include <DX11_2D_GameEngine_Lib/Texture.h>
#include <DX11_2D_GameEngine_Lib/Mesh.h>
#include <DX11_2D_GameEngine_Lib/Material.h>

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/ImageRenderer.h>

GUI_ImageRenderer::GUI_ImageRenderer(const std::string& image_renderer_gui_name)
	:GUI_Component(image_renderer_gui_name)
{
}

GUI_ImageRenderer::~GUI_ImageRenderer()
{
}

void GUI_ImageRenderer::Render()
{
	auto p_image_renderer = m_p_selected_game_object->GetComponent<ImageRenderer>();
	if (p_image_renderer == nullptr)
		return;

	auto is_active = p_image_renderer->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::ImageRenderer, is_active, IconType::Component_Image_Renderer))
	{
		auto p_image_renderer = m_p_selected_game_object->GetComponent<ImageRenderer>();
		if (p_image_renderer == nullptr)
			return;

		p_image_renderer->SetIsActive(is_active);


		DrawComponentEnd();
	}
}
