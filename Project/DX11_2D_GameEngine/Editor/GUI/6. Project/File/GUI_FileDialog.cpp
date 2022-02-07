#include "stdafx.h"
#include "GUI_FileDialog.h"

#include "Helper/IconProvider.h"

#include "GUI_FileItem.h"

#include <DX11_2D_GameEngine_Lib/FileManager.h>

GUI_FileDialog::~GUI_FileDialog()
{
	m_file_item_vector.clear();
	m_file_item_vector.shrink_to_fit();
}

void GUI_FileDialog::Update(const std::string& directory_path)
{
	//���� ���丮 ��ο� ���� ������Ʈ�� ��ΰ� ���ٸ�
	if (m_current_directory_path._Equal(directory_path))
		return;

	//�ٸ� ���
	//���� ���丮 ��� ������Ʈ
	m_current_directory_path = directory_path;

	//FileItem ���Ͱ� ����ִٸ� �ʱ�ȭ
	if (!m_file_item_vector.empty())
	{
		m_file_item_vector.clear();
		m_file_item_vector.shrink_to_fit();
	}

	//Directory
	auto directory_path_vector = FILE_MANAGER->GetDirectoriesInDirectory(m_current_directory_path);
	for (const auto& directory_path : directory_path_vector)
	{
		PayLoad pay_load;
		pay_load.type = PayLoadType::Folder;
		pay_load.data = directory_path;

		GUI_FileItem file_item(FileThumbnailType::Folder, pay_load);

		m_file_item_vector.emplace_back(file_item);
	}

	//File
	auto file_path_vector = FILE_MANAGER->GetFilesInDirectory(m_current_directory_path);
	for (const auto& file_path : file_path_vector)
	{
		PayLoad pay_load;
		pay_load.data = file_path;

		//Animation
		if (FILE_MANAGER->IsSupportedAnimationFile(file_path))
		{
			pay_load.type = PayLoadType::Animation;

			GUI_FileItem file_item(FileThumbnailType::Animation, pay_load);
			m_file_item_vector.emplace_back(file_item);
		}
		//Audio
		else if (FILE_MANAGER->IsSupportedAudioFile(file_path))
		{
			pay_load.type = PayLoadType::Audio;

			GUI_FileItem file_item(FileThumbnailType::Audio, pay_load);
			m_file_item_vector.emplace_back(file_item);
		}

		//Material
		else if (FILE_MANAGER->IsSupportedMaterialFile(file_path))
		{
			pay_load.type = PayLoadType::Material;

			GUI_FileItem file_item(FileThumbnailType::Material, pay_load);
			m_file_item_vector.emplace_back(file_item);
		}

		//Mesh
		else if (FILE_MANAGER->IsSupportedMeshFile(file_path))
		{
			pay_load.type = PayLoadType::Mesh;

			GUI_FileItem file_item(FileThumbnailType::Mesh, pay_load);
			m_file_item_vector.emplace_back(file_item);
		}

		//Prefab
		else if (FILE_MANAGER->IsSupportedPrefabFile(file_path))
		{
			pay_load.type = PayLoadType::Prefab;

			GUI_FileItem file_item(FileThumbnailType::Prefab, pay_load);
			m_file_item_vector.emplace_back(file_item);
		}

		//Scene
		else if (FILE_MANAGER->IsSupportedSceneFile(file_path))
		{
			pay_load.type = PayLoadType::Scene;

			GUI_FileItem file_item(FileThumbnailType::Scene, pay_load);
			m_file_item_vector.emplace_back(file_item);
		}

		//TileMap
		else if (FILE_MANAGER->IsSupportedTileMapFile(file_path))
		{
			pay_load.type = PayLoadType::TileMap;

			GUI_FileItem file_item(FileThumbnailType::TileMap, pay_load);
			m_file_item_vector.emplace_back(file_item);
		}

		//Texture
		else if (FILE_MANAGER->IsSupportedTextureFile(file_path))
		{
			pay_load.type = PayLoadType::Texture;

			GUI_FileItem file_item(FileThumbnailType::Texture, pay_load);
			m_file_item_vector.emplace_back(file_item);
		}

		//None
		else
		{
			pay_load.type = PayLoadType::None;

			GUI_FileItem file_item(FileThumbnailType::None, pay_load);
			m_file_item_vector.emplace_back(file_item);
		}
	}
}

void GUI_FileDialog::Render(const ImGuiTextFilter& file_item_filter)
{
	const auto& style = ImGui::GetStyle();
	const auto& file_dialog_width = ImGui::GetContentRegionAvail().x;
	float same_line_range = style.ItemSpacing.x;

	for (auto& file_item : m_file_item_vector)
	{
		//Filter�� ���� �˻����� �� ��ġ���� �ʴٸ� �������� ����
		if (!file_item_filter.PassFilter(file_item.GetFileName().c_str()))
			continue;

	    //Render File Item
		file_item.Render();

		//File Item�� �׸� ������ File Item�� �ʺ�� ���� �ʺ� ���� ���� ����
		same_line_range += (file_item.GetFileItemSize().x + style.ItemSpacing.x);
		//������ �׸� File Item�� ���� ���ο� �׸��� �Ǻ�
		if (same_line_range <= file_dialog_width - (file_item.GetFileItemSize().x + style.ItemSpacing.x))
			ImGui::SameLine();
		else
			same_line_range = 0.0f;
	}
}