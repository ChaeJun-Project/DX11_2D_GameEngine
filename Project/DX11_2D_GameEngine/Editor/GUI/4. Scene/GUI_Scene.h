#pragma once
#include "GUI/IGUI.h"

class CameraEx;

class GUI_Scene final : public IGUI
{
public:
	GUI_Scene(const std::string& scene_title);
	~GUI_Scene();

public:
	void Initialize() override;
	void Update() override;

	void Begin() override;
	void Render() override;
	void End() override;

private:
    void ShowProjectionButton();
    void ShowScene();
	void ShowGizmo();

private:
	//Editor Camera
	CameraEx* m_p_editor_camera = nullptr;

	Vector2 m_gizmo_offset = Vector2::Zero;
};

