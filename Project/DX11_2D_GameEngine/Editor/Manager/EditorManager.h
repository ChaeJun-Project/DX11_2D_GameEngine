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
	//�������� ���Ǵ� ��� GUI�� ���� �� ������ ����
	//�� GUI ���� �� �������� ������ �� �ϳ��� ��ü�� ������ �־�� �ϹǷ�
	//unique pointer�� ����Ͽ� ��ü ���� �� ����
	std::map<GUIType, std::unique_ptr<IGUI>> m_gui_map;
};

