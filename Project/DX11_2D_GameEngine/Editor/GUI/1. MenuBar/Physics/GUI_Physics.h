#pragma once

#include "GUI/IGUI_Popup.h"

class GUI_Physics final : public IGUI_Popup
{
public:
	GUI_Physics();
	virtual ~GUI_Physics();

public:
	virtual void Render() override;

private:
    void ShowLayerCollisionMatrix();

private:
    void InitializePhysics();
    void SavePhysics();
    void LoadPhysics();

private:
	std::string m_folder_path = "Engine/";
	std::string m_physics_file_path = "Physics.txt";

	std::map<UINT, std::vector<bool>> m_collision_check_map;

	friend class GUI_MenuBar;
};

