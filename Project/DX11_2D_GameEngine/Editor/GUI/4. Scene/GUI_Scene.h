#pragma once
#include "GUI/IGUI.h"

class GUI_Scene final : public IGUI
{
public:
	GUI_Scene(const std::string& scene_title);
	~GUI_Scene();

public:
	void Update() override;
	void Render() override;

private:
    void ShowProjectionButton();
    void ShowScene();
	void ShowGizmo();

private:
	//Editor Camera
	Camera* m_p_editor_camera = nullptr;
};

