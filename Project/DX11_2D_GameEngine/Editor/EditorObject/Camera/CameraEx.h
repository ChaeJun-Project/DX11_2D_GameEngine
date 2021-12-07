#pragma once

#include <DX11_2D_GameEngine_Lib/Camera.h>

class CameraEx final : public Camera
{
public:
    void FinalUpdate();

private:
    void ControlEditorCamera();

private:
    float m_speed = 100.0f;
    float m_drag = m_speed * 0.8f;
};

