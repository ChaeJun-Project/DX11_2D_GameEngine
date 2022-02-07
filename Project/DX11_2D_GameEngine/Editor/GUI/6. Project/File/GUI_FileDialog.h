#pragma once

class GUI_FileItem;

class GUI_FileDialog final
{
public:
	GUI_FileDialog() = default;
	~GUI_FileDialog();

	void Update(const std::string& directory_path);
	void Render(const ImGuiTextFilter& file_item_filter);

public:
    std::string m_current_directory_path;

	std::vector<GUI_FileItem> m_file_item_vector;

	friend class GUI_Project;
};

