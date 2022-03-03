#include "stdafx.h"
#include "IconProvider.h"

IconProvider::~IconProvider()
{
	for (auto& icon : m_icon_map)
	{
		if (icon.second != nullptr)
			icon.second.reset();
	}

	m_icon_map.clear();
}

void IconProvider::Initialize()
{
	//Load Tool Bar Texture
	LoadToolBarIcons();

	//Load GameObject Icon Texture
	LoadGameObjectIcons();

	//Load Component Icon Texture
	LoadComponentIcons();

	//Load Console Icon Texture
	LoadConsoleIcons();

	//Load FileThumbnail Texture
	LoadFileThumbnails();
}

void IconProvider::LoadToolBarIcons()
{
	LoadIconTexture("Icon/ToolBar/toolbar_play.png", IconType::ToolBar_Play);
	LoadIconTexture("Icon/ToolBar/toolbar_pause.png", IconType::ToolBar_Pause);
	LoadIconTexture("Icon/ToolBar/toolbar_stop.png", IconType::ToolBar_Stop);
}

void IconProvider::LoadGameObjectIcons()
{
	LoadIconTexture("Icon/Inspector/GameObject.png", IconType::Inspector_GameObject);
	LoadIconTexture("Icon/Inspector/Prefab_GameObject.png", IconType::Inspector_Prefab_GameObject);
}

void IconProvider::LoadComponentIcons()
{
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
	LoadIconTexture("Icon/Inspector/component_tile_map.png", IconType::Component_TileMapRenderer);
	LoadIconTexture("Icon/Inspector/component_rigid_body2D.png", IconType::Component_RigidBody2D);
	LoadIconTexture("Icon/Inspector/component_audio_listener.png", IconType::Component_Audio_Listener);
	LoadIconTexture("Icon/Inspector/component_audio_source.png", IconType::Component_Audio_Source);

	LoadIconTexture("Icon/Inspector/component_script.png", IconType::Component_Script);
}

void IconProvider::LoadConsoleIcons()
{
	LoadIconTexture("Icon/Console/console_log_info.png", IconType::Console_Info);
	LoadIconTexture("Icon/Console/console_log_warning.png", IconType::Console_Warning);
	LoadIconTexture("Icon/Console/console_log_error.png", IconType::Console_Error);
}

void IconProvider::LoadFileThumbnails()
{
	LoadFileThumbnailTexture("Icon/FileDialog/filedialog_none.png", FileThumbnailType::None);
	LoadFileThumbnailTexture("Icon/FileDialog/filedialog_folder.png", FileThumbnailType::Folder);
	LoadFileThumbnailTexture("Icon/FileDialog/filedialog_animation.png", FileThumbnailType::Animation);
	LoadFileThumbnailTexture("Icon/FileDialog/filedialog_audio.png", FileThumbnailType::Audio);
	LoadFileThumbnailTexture("Icon/FileDialog/filedialog_material.png", FileThumbnailType::Material);
	LoadFileThumbnailTexture("Icon/FileDialog/filedialog_mesh.png", FileThumbnailType::Mesh);
	LoadFileThumbnailTexture("Icon/FileDialog/filedialog_prefab.png", FileThumbnailType::Prefab);
	LoadFileThumbnailTexture("Icon/FileDialog/filedialog_scene.png", FileThumbnailType::Scene);
	LoadFileThumbnailTexture("Icon/FileDialog/filedialog_tilemap.png", FileThumbnailType::TileMap);
}

void IconProvider::LoadIconTexture(const std::string& texture_path, const IconType& icon_type)
{
	auto resource_manager = RESOURCE_MANAGER;

	auto texture = resource_manager->CreateIconTexture(texture_path);

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

void IconProvider::LoadFileThumbnailTexture(const std::string& texture_path, const FileThumbnailType& file_thumbnail_type)
{
	auto resource_manager = RESOURCE_MANAGER;

	auto texture = resource_manager->CreateIconTexture(texture_path);

	if (texture != nullptr)
	{
		auto file_thumbnail_iter = m_file_thumbnail_map.insert(std::make_pair(file_thumbnail_type, texture));
		auto result = file_thumbnail_iter.second;
		assert(result);
	}

	else
		assert(false);
}

const std::shared_ptr<Texture>& IconProvider::GetFileThumbnail(const FileThumbnailType& file_thumbnail_type)
{
	auto file_thumbnail_iter = m_file_thumbnail_map.find(file_thumbnail_type);

	if (file_thumbnail_iter == m_file_thumbnail_map.end())
		return nullptr;

	return file_thumbnail_iter->second;
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
