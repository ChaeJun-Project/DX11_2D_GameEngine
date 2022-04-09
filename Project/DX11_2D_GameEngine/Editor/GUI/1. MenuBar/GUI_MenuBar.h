#pragma once
#include "GUI/IGUI.h"

class GUI_SceneRename;
class GUI_StyleSelector;
class GUI_SpriteEditor;
class GUI_Physics;
class GUI_SetResolution;

class GUI_MenuBar : public IGUI
{
public:
	GUI_MenuBar(const std::string& menubar_title);
	~GUI_MenuBar();

public:
	void Initialize() override;
	void Update() override;

	void Begin() override {}
	void Render() override;
	void End() override {}

private:
	void NewScene();
	void LoadScene();
	void SaveScene();

	void InitializeRecentScene();
	void SaveRecentScene();
	void LoadRecentScene();

private:
	//=========================
	//Recent Scene
	//=========================
	std::string m_folder_path = "Engine/";
	std::string m_recent_scene_file_path = "RecentScene.txt";
	
	//=========================
	//File
	//=========================
	std::unique_ptr<GUI_SceneRename> m_p_gui_scene_rename = nullptr;

	//=========================
	//Edit
	//=========================
	//Style Selector
	std::unique_ptr<GUI_StyleSelector> m_p_gui_style_selector = nullptr;

	//SpriteAnimation
	std::unique_ptr<GUI_SpriteEditor> m_p_gui_sprite_editor = nullptr;

	//Physics
	std::unique_ptr<GUI_Physics> m_p_gui_physics = nullptr;

	//Set Resolution
	std::unique_ptr<GUI_SetResolution> m_p_gui_set_resolution = nullptr;
};

