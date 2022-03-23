#pragma once
#include "GUI/IGUI_Popup.h"

//<summary>
//게임 해상도를 설정하고 저장할 수 있는 GUI
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

