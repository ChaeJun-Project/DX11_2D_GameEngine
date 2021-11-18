#include "stdafx.h"
#include "EditorManager.h"
#include <DX11_2D_GameEngine_Lib/GraphicsManager.h>

EditorManager::EditorManager()
{
}

EditorManager::~EditorManager()
{
	for (auto& gui : m_gui_map)
	{
		if (gui.second != nullptr)
			gui.second.reset();
	}

	m_gui_map.clear();
}

void EditorManager::Initialize(const HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context)
{
	//<summary>
	//imgui_docking 버전의 DirectX11 Sample 코드를 보고 작성함
	//</summary>

	//ImGui 버전 체크
	IMGUI_CHECKVERSION();

	//ImGui Context 생성
	ImGui::CreateContext();

	//ImGui Context 설정
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  //키보드 입력 사용
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      //도킹 시스템 사용
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    //멀티 뷰포트 사용(윈도우 플랫폼)

	//Set GUI Style(Current: Dark Color)
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	//ImGui에 윈도우 핸들값 전달
	ImGui_ImplWin32_Init(hwnd);
	//ImGui에 D3D11 Device 객체와` D3D11 DeviceContext 객체 정보 전달
	ImGui_ImplDX11_Init(device, device_context);

	//Add GUIs
	//m_gui_map.insert(std::make_pair(GUIType::MenuBar, std::make_unique<>()));			//MenuBar
	//m_gui_map.insert(std::make_pair(GUIType::ToolBar, std::make_unique<>()));			//ToolBar
	//m_gui_map.insert(std::make_pair(GUIType::Hierarchy, std::make_unique<>()));		//Hierarchy
	//m_gui_map.insert(std::make_pair(GUIType::Scene, std::make_unique<>()));		    //Scene
	//m_gui_map.insert(std::make_pair(GUIType::Game, std::make_unique<>()));            //Game
	//m_gui_map.insert(std::make_pair(GUIType::Inspector, std::make_unique<>()));		//Inspector
	//m_gui_map.insert(std::make_pair(GUIType::Project, std::make_unique<>()));			//Project
	//m_gui_map.insert(std::make_pair(GUIType::Console, std::make_unique<>()));			//Console(Log)
}

void EditorManager::Update()
{
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();


}

void EditorManager::Render()
{
	
}

void EditorManager::ResizeEditor(const UINT& width, const UINT& height)
{
    auto graphics_manager = GraphicsManager::GetInstance();
	graphics_manager->ResizeWindowByUser(width, height);

	ImGui_ImplDX11_InvalidateDeviceObjects();
	ImGui_ImplDX11_CreateDeviceObjects();
}
