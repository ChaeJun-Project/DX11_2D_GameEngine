#pragma once

#include <DX11_2D_GameEngine_Lib/Singleton.h>

#include "GUI/IGUI.h"

class IGUI;

class EditorManager final : public Singleton<EditorManager>
{
	SINGLETON(EditorManager);

	EditorManager() = default;
	~EditorManager();

public:
	void Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context);
	void Update();
	void Render();

public:
    void ExcuteEventCallBack();

private:
    void InitializeGUI();
    void ImGuiNewFrame();
	void BeginDockWindow();
	void EndDockWindow();

public:
    void ResizeEditor(const UINT& width, const UINT& height);

private:
	//�������� ���Ǵ� ��� GUI�� ���� �� ������ ����
	//�� GUI ���� �� �������� ������ �� �ϳ��� ��ü�� ������ �־�� �ϹǷ�
	//unique pointer�� ����Ͽ� ��ü ���� �� ����
	std::map<GUIType, std::unique_ptr<IGUI>> m_gui_map;

	std::string init_current_path;

	std::vector<Event_CallBack> m_event_callBack_vector;
};

