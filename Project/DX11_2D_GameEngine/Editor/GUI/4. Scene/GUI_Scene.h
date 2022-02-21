#pragma once
#include "GUI/IGUI.h"

class CameraEx;

class GUI_Scene final : public IGUI
{
public:
	GUI_Scene(const std::string& scene_title);
	~GUI_Scene();

public:
	virtual void Update() override;

	virtual void Begin() override;
	virtual void Render() override;
	virtual void End() override;

private:
    void ShowProjectionButton();
    void ShowScene();
	void ShowGizmo();

private:
	//Editor Camera
	CameraEx* m_p_editor_camera = nullptr;

	Vector2 m_gizmo_offset = Vector2::Zero;
};

