#pragma once
#include "GUI/IGUI_Popup.h"

//<summary>
//���� �ػ󵵸� �����ϰ� ������ �� �ִ� GUI
//</summary>
class GUI_SetResolution final : public IGUI_Popup
{
public:
	GUI_SetResolution();
	virtual ~GUI_SetResolution() = default;

public:
	virtual void Render() override;

private:
	void InitializeResolution();
	void SaveResolution();
	void LoadResolution();

private:
	std::string m_folder_path = "Engine/";
	std::string m_resolution_file_path = "Resolution.txt";

	Vector2 m_resolution = Vector2::Zero;

	friend class GUI_MenuBar;
};

