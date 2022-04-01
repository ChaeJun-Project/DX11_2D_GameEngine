#include "stdafx.h"
#include "GUI_FileDialog.h"

#include "Helper/IconProvider.h"

#include "GUI_FileItem.h"

#include <DX11_2D_GameEngine_Lib/FileManager.h>

GUI_FileDialog::~GUI_FileDialog()
{
	Clear();

	m_p_update_file_dialog_func = nullptr;
}

void GUI_FileDialog::Update(const std::string& folder_path)
{
	//현재 폴더 경로 업데이트
	m_current_folder_path = folder_path;

	//FileItem 벡터가 비어있다면 초기화
	if (!m_file_item_vector.empty())
	{
		Clear();
	}

	//Directory
	auto directory_path_vector = FILE_MANAGER->GetDirectoriesInDirectory(m_current_folder_path);
	for (const auto& directory_path : directory_path_vector)
	{
		PayLoad pay_load;
		pay_load.type = PayLoadType::Folder;
		pay_load.data = directory_path;

		GUI_FileItem* p_file_item = nullptr;

		p_file_item = new GUI_FileItem(FileThumbnailType::Folder, pay_load);
		p_file_item->SetFileDialog(this);

		m_file_item_vector.emplace_back(p_file_item);
	}

	//File
	auto file_path_vector = FILE_MANAGER->GetFilesInDirectory(m_current_folder_path);
	for (const auto& file_path : file_path_vector)
	{
		PayLoad pay_load;
		pay_load.data = file_path;

		GUI_FileItem* p_file_item = nullptr;

		//Animation
		if (FILE_MANAGER->IsSupportedAnimationFile(file_path))
		{
			pay_load.type = PayLoadType::Animation;

			p_file_item = new GUI_FileItem(FileThumbnailType::Animation, pay_load);
		}
		//Audio
		else if (FILE_MANAGER->IsSupportedAudioFile(file_path))
		{
			pay_load.type = PayLoadType::Audio;

			p_file_item = new GUI_FileItem(FileThumbnailType::Audio, pay_load);
		}

		//Material
		else if (FILE_MANAGER->IsSupportedMaterialFile(file_path))
		{
			pay_load.type = PayLoadType::Material;

			p_file_item = new GUI_FileItem(FileThumbnailType::Material, pay_load);
		}

		//Mesh
		else if (FILE_MANAGER->IsSupportedMeshFile(file_path))
		{
			pay_load.type = PayLoadType::Mesh;

			p_file_item = new GUI_FileItem(FileThumbnailType::Mesh, pay_load);
		}

		//Scene
		else if (FILE_MANAGER->IsSupportedSceneFile(file_path))
		{
			pay_load.type = PayLoadType::Scene;

			p_file_item = new GUI_FileItem(FileThumbnailType::Scene, pay_load);
		}

		//Texture
		else if (FILE_MANAGER->IsSupportedTextureFile(file_path))
		{
			pay_load.type = PayLoadType::Texture;

			p_file_item = new GUI_FileItem(FileThumbnailType::Texture, pay_load);
		}

		//TileMap
		else if (FILE_MANAGER->IsSupportedTileMapFile(file_path))
		{
			pay_load.type = PayLoadType::TileMap;

			p_file_item = new GUI_FileItem(FileThumbnailType::TileMap, pay_load);
		}

		//Prefab
		else if (FILE_MANAGER->IsSupportedPrefabFile(file_path))
		{
			pay_load.type = PayLoadType::Prefab;

			p_file_item = new GUI_FileItem(FileThumbnailType::Prefab, pay_load);
		}

		//Particle
		else if (FILE_MANAGER->IsSupportedParticleFile(file_path))
		{
			pay_load.type = PayLoadType::Particle;

			p_file_item = new GUI_FileItem(FileThumbnailType::Particle, pay_load);
		}

		//None
		else
		{
			pay_load.type = PayLoadType::None;

			p_file_item = new GUI_FileItem(FileThumbnailType::None, pay_load);
		}

		p_file_item->SetFileDialog(this);
		m_file_item_vector.emplace_back(p_file_item);
	}
}

void GUI_FileDialog::Render(const ImGuiTextFilter& file_item_filter)
{
	const auto& style = ImGui::GetStyle();
	const auto& file_dialog_width = ImGui::GetContentRegionAvail().x;
	float same_line_range = style.ItemSpacing.x;

	for (auto& file_item : m_file_item_vector)
	{
		//Filter를 통해 검색했을 때 일치하지 않다면 보여주지 않음
		if (!file_item_filter.PassFilter(file_item->GetFileName().c_str()))
			continue;

	    //Render File Item
		file_item->Render();

		//File Item을 그릴 때마다 File Item의 너비와 정렬 너비를 더한 값을 누적
		same_line_range += (file_item->GetFileItemSize().x + style.ItemSpacing.x);
		//다음에 그릴 File Item을 같은 라인에 그릴지 판별
		if (same_line_range <= file_dialog_width - (file_item->GetFileItemSize().x + style.ItemSpacing.x))
			ImGui::SameLine();
		else
			same_line_range = 0.0f;
	}

	if (m_is_clear_file_item)
	{
		m_p_update_file_dialog_func(m_current_folder_path);
		m_is_clear_file_item = false;
	}
}

void GUI_FileDialog::Clear()
{
	for (auto& p_file_item : m_file_item_vector)
	{
		SAFE_DELETE(p_file_item);
	}
	
	m_file_item_vector.clear();
	m_file_item_vector.shrink_to_fit();
}
