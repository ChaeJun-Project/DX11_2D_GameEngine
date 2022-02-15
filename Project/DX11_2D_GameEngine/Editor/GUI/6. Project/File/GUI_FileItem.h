#pragma once

#include "Helper/IconProvider.h"

class GUI_FileDialog;
class Texture;

class GUI_FileItem final
{
public:
	GUI_FileItem() = default;
	GUI_FileItem
	(
		const FileThumbnailType& file_thumbnail_type,
		const PayLoad& pay_load
	);
	~GUI_FileItem() = default;

	void Render();

private:
	void CheckClickedFileItem();
	void DragFileItem();
	void ShowMenuPopup();

public:
    void SetFileDialog(GUI_FileDialog* p_owner_file_dialog) { m_p_owner_file_dialog = p_owner_file_dialog; }

	const FileThumbnailType& GetFileThumbnailType() { return m_file_thumbnail_type; }
	const std::string& GetFilePath() { return std::get<std::string>(m_pay_load.data); }
	const std::string& GetFileName() { return m_file_name; }
	const ImVec2& GetFileItemSize() { return m_file_item_size; }

private:
	GUI_FileDialog* m_p_owner_file_dialog = nullptr;

	const FileThumbnailType m_file_thumbnail_type;
	std::shared_ptr<Texture> m_p_thumbnail_texture = nullptr;
	PayLoad m_pay_load; //Type, File Path
	std::string m_folder_path;
	std::string m_file_name;
	std::string m_file_extension;

	ImVec2 m_file_item_size = ImVec2(70.0f, 70.0f);

	bool m_is_clicked = false;

	int m_file_item_padding = 5;
};

