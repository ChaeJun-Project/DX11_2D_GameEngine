#pragma once

class GUI_FileItem;

class GUI_FileDialog final
{
public:
	GUI_FileDialog() = default;
	~GUI_FileDialog();

	void Update(const std::string& folder_path);
	void Render(const ImGuiTextFilter& file_item_filter);

public:
	void SetCurrentFolderPath(const std::string& current_folder_path) { m_current_folder_path = current_folder_path; }
	void IsClearFileItem() { m_is_clear_file_item = true; }

private:
    void Clear();

public:
    std::string m_current_folder_path;

	std::vector<GUI_FileItem*> m_file_item_vector;

	Clicked_CallBack1 m_p_folder_double_clicked_func = nullptr;

	bool m_is_clear_file_item = false;

	friend class GUI_Project;
};

