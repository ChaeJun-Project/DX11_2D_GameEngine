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
	LoadIconTexture("Texture/Icon/ToolBar/toolbar_play.png", IconType::ToolBar_Play);
	LoadIconTexture("Texture/Icon/ToolBar/toolbar_pause.png", IconType::ToolBar_Pause);
	LoadIconTexture("Texture/Icon/ToolBar/toolbar_stop.png", IconType::ToolBar_Stop);

	//Load GameObject Icon Texture
	LoadIconTexture("Texture/Icon/Inspector/GameObject.png", IconType::Inspector_GameObject);
	LoadIconTexture("Texture/Icon/Inspector/Prefab_GameObject.png", IconType::Inspector_Prefab_GameObject);

	//Load Component Icon Texture
	LoadIconTexture("Texture/Icon/Inspector/component_settings.png", IconType::Component_Settings);
	LoadIconTexture("Texture/Icon/Inspector/component_transform.png", IconType::Component_Transform);
	LoadIconTexture("Texture/Icon/Inspector/component_camera.png", IconType::Component_Camera);
	LoadIconTexture("Texture/Icon/Inspector/component_sprite_renderer.png", IconType::Component_SpriteRenderer);
	LoadIconTexture("Texture/Icon/Inspector/component_animator2D.png", IconType::Component_Animator2D);
	LoadIconTexture("Texture/Icon/Inspector/component_animator.png", IconType::Component_Animator);
	LoadIconTexture("Texture/Icon/Inspector/component_collider2d.png", IconType::Component_Collider2D);
	LoadIconTexture("Texture/Icon/Inspector/component_light2D.png", IconType::Component_Light2D);
	LoadIconTexture("Texture/Icon/Inspector/component_particle_system.png", IconType::Component_ParticleSystem);
	LoadIconTexture("Texture/Icon/Inspector/component_tile_map.png", IconType::Component_TileMap);
	LoadIconTexture("Texture/Icon/Inspector/component_rigid_body2D.png", IconType::Component_RigidBody2D);
	
	LoadIconTexture("Texture/Icon/Inspector/component_script.png", IconType::Component_Script);
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

	//�ش� ������ �ؽ�ó�� ã�� ������ ���
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

const bool IconProvider::CreateImageButton(const char* id, const IconType& icon_type, const ImVec2& button_size)
{
	bool is_add_image_button = ImGui::ImageButton
	(
		GetIconTexture(icon_type)->GetShaderResourceView(),
		button_size
	);

	return is_add_image_button;
}