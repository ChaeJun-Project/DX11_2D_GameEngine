#include "stdafx.h"
#include "IconProvider.h"

#include <DX11_2D_GameEngine_Lib/ResourceManager.h>

IconProvider::~IconProvider()
{
	for (auto& icon : m_icon_map)
	{
		if(icon.second != nullptr)
			icon.second.reset();
	}

	m_icon_map.clear();
}

void IconProvider::Initialize()
{
    //Load Tool Bar Texture
	LoadIconTexture("Icon/ToolBar/toolbar_play.png", IconType::ToolBar_Play);
	LoadIconTexture("Icon/ToolBar/toolbar_pause.png", IconType::ToolBar_Pause);
	LoadIconTexture("Icon/ToolBar/toolbar_stop.png", IconType::ToolBar_Stop);

	//Load GameObject Icon Texture
	LoadIconTexture("Icon/Inspector/GameObject.png", IconType::Inspector_GameObject);
	LoadIconTexture("Icon/Inspector/Prefab_GameObject.png", IconType::Inspector_Prefab_GameObject);

	//Load Component Icon Texture
	LoadIconTexture("Icon/Inspector/component_settings.png", IconType::Component_Settings);
	LoadIconTexture("Icon/Inspector/component_plus.png", IconType::Component_Plus);
	LoadIconTexture("Icon/Inspector/component_back_arrow.png", IconType::Component_Back);
	LoadIconTexture("Icon/Inspector/component_transform.png", IconType::Component_Transform);
	LoadIconTexture("Icon/Inspector/component_camera.png", IconType::Component_Camera);
	LoadIconTexture("Icon/Inspector/component_sprite_renderer.png", IconType::Component_SpriteRenderer);
	LoadIconTexture("Icon/Inspector/component_animator2D.png", IconType::Component_Animator2D);
	LoadIconTexture("Icon/Inspector/component_animator.png", IconType::Component_Animator);
	LoadIconTexture("Icon/Inspector/component_collider2d.png", IconType::Component_Collider2D);
	LoadIconTexture("Icon/Inspector/component_light2D.png", IconType::Component_Light2D);
	LoadIconTexture("Icon/Inspector/component_particle_system.png", IconType::Component_ParticleSystem);
	LoadIconTexture("Icon/Inspector/component_tile_map.png", IconType::Component_TileMap);
	LoadIconTexture("Icon/Inspector/component_rigid_body2D.png", IconType::Component_RigidBody2D);
	
	LoadIconTexture("Icon/Inspector/component_script.png", IconType::Component_Script);

	//Load Console Icon
	LoadIconTexture("Icon/Console/console_log_info.png", IconType::Console_Info);
	LoadIconTexture("Icon/Console/console_log_warning.png", IconType::Console_Warning);
	LoadIconTexture("Icon/Console/console_log_error.png", IconType::Console_Error);
}

void IconProvider::LoadIconTexture(const std::string& texture_path, const IconType& icon_type)
{
	auto resource_manager = ResourceManager::GetInstance();

	auto texture = resource_manager->LoadTexture(texture_path, TextureType::Standard);

	if (texture != nullptr)
	{
		auto icon_iter = m_icon_map.insert(std::make_pair(icon_type, texture));
		auto result = icon_iter.second;
		assert(result);
	}
	
	else
		assert(false);
}

const std::shared_ptr<Texture>& IconProvider::GetIconTexture(const IconType& icon_type)
{
	auto icon_iter = m_icon_map.find(icon_type);

	//해당 아이콘 텍스처를 찾지 못했을 경우
	if (icon_iter == m_icon_map.end())
		return nullptr;

	return icon_iter->second;
}

void IconProvider::CreateImage(const IconType& icon_type, const ImVec2& button_size)
{
	ImGui::Image
	(
		GetIconTexture(icon_type)->GetShaderResourceView(),
		button_size
	);
}

const bool IconProvider::CreateImageButton(const IconType& icon_type, const ImVec2& button_size)
{
	bool is_add_image_button = ImGui::ImageButton
	(
		GetIconTexture(icon_type)->GetShaderResourceView(),
		button_size
	);

	return is_add_image_button;
}
