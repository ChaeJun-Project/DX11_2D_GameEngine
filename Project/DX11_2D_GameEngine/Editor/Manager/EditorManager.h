#pragma once

#include <DX11_2D_GameEngine_Lib/Singleton.h>

#include "GUI/IGUI.h"

class IGUI;

class EditorManager : public Singleton<EditorManager>
{
	SINGLETON(EditorManager);

	EditorManager();
	~EditorManager();

public:
	void Initialize(const HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context);
	void Update();
	void Render();

public:
    void ResizeEditor(const UINT& width, const UINT& height);

private:
	//엔전에서 사용되는 모든 GUI를 저장 및 관리할 벡터
	//각 GUI 또한 각 종류마다 유일한 단 하나의 객체만 가지고 있어야 하므로
	//unique pointer를 사용하여 객체 생성 및 관리
	std::map<GUIType, std::unique_ptr<IGUI>> m_gui_map;
};

