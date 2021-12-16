#pragma once

#include <DX11_2D_GameEngine_Lib/Camera.h>

class CameraEx final : public Camera
{
public:
    void FinalUpdate();

private:
    void ControlEditorCamera();

    void Control2DViewMode();
    void Control3DViewMode();

public:
    void SetIsActvie(const bool& is_active) { m_is_active = is_active; }

private:
    bool m_is_active = false;

    float m_speed = 100.0f;
    float m_drag = m_speed * 0.8f;
};

