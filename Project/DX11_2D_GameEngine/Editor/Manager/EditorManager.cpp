#include "stdafx.h"
#include "EditorManager.h"
#include <DX11_2D_GameEngine_Lib/GraphicsManager.h>

//Helper
#include "Helper/EditorHelper.h"
#include "Helper/IconProvider.h"

//GUI
#include "GUI/1. MenuBar/GUI_MenuBar.h"
#include "GUI/2. ToolBar/GUI_ToolBar.h"
#include "GUI/3. Hierarchy/GUI_Hierarchy.h"
#include "GUI/4. Scene/GUI_Scene.h"
#include "GUI/5. Game/GUI_Game.h"
#include "GUI/6. Inspector/GUI_Inspector.h"
#include "GUI/7. Project/GUI_Project.h"
#include "GUI/8. Console/GUI_Console.h"

#include <DX11_2D_GameEngine_Lib/FileManager.h>

EditorManager::~EditorManager()
{
	//<summary>
	//imgui ShutDown 직전에 imgui.ini가 저장될 위치를 프로그램 시작했을 때 경로로 맞춤
	//</summary>
	std::string init_directory_path = init_current_path + "Imgui/imgui.ini";
	ImGui::GetIO().IniFilename = init_directory_path.c_str();

	//ImGui Clear 
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	for (auto& gui : m_gui_map)
	{
		if (gui.second != nullptr)
			gui.second.reset();
	}

	m_gui_map.clear();
}

void EditorManager::Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context)
{
	//Initialize Icon Provider
	IconProvider::GetInstance()->Initialize();

	init_current_path = FileManager::GetWorkingDirectory();

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
	
	//Initialize GUI
	InitializeGUI();
}

void EditorManager::Update()
{
	for (auto& gui : m_gui_map)
	{
		if (gui.second != nullptr && gui.second->GetIsActive())
			gui.second->Update();
	}
}

void EditorManager::Render()
{
	//New Frame ImGui
	ImGuiNewFrame();

	//도킹을 사용하는 경우
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		BeginDockWindow();
	}

	for (auto& gui : m_gui_map)
	{
		if (gui.second != nullptr && gui.second->GetIsActive())
		{
			gui.second->Begin();
			gui.second->Render();
			gui.second->End();
		}
	}

	//도킹을 사용하는 경우
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		EndDockWindow();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void EditorManager::InitializeGUI()
{
	//Add GUI
	m_gui_map.insert(std::make_pair(GUIType::MenuBar, std::make_unique<GUI_MenuBar>("Main Menu Bar")));		//MenuBar
	m_gui_map.insert(std::make_pair(GUIType::ToolBar, std::make_unique<GUI_ToolBar>("Tool Bar")));			//ToolBar
	m_gui_map.insert(std::make_pair(GUIType::Hierarchy, std::make_unique<GUI_Hierarchy>("Hierarchy")));		//Hierarchy
	m_gui_map.insert(std::make_pair(GUIType::Scene, std::make_unique<GUI_Scene>("Scene")));					//Scene
	m_gui_map.insert(std::make_pair(GUIType::Game, std::make_unique<GUI_Game>("Game")));					//Game
	m_gui_map.insert(std::make_pair(GUIType::Inspector, std::make_unique<GUI_Inspector>("Inspector")));		//Inspector
	m_gui_map.insert(std::make_pair(GUIType::Project, std::make_unique<GUI_Project>("Project")));			//Project
	m_gui_map.insert(std::make_pair(GUIType::Console, std::make_unique<GUI_Console>("Console")));			//Console(Log)

	//Initialize Each GUI
	for (auto& gui : m_gui_map)
	{
		if (gui.second != nullptr)
		{
			gui.second->Initialize();
		}
	}
}

void EditorManager::ImGuiNewFrame()
{
	//DX11 먼저 호출 후 Win32 호출해야 오류가 생기지 않음
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void EditorManager::BeginDockWindow()
{
	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus;

	//도킹 윈도우는 전체 윈도우 영역에서 MenuBar와 ToolBar의 높이만큼 뺀 부분부터 화면에 Draw
	//도킹 윈도우의 사이즈는 전체 윈도우 영역에서 MenuBar와 ToolBar의 높이만큼 빼서 나온 영역
	ImVec2 offset = ImVec2(0, m_gui_map[GUIType::MenuBar]->m_gui_size.y + m_gui_map[GUIType::ToolBar]->m_gui_size.y);

	ImGuiViewport* viewport = ImGui::GetMainViewport();

	//NextWindow : 도킹 윈도우
	ImGui::SetNextWindowPos(viewport->Pos + offset); //도킹 윈도우를 그릴 커서 위치 설정
	ImGui::SetNextWindowSize(viewport->Size - offset); //도킹 윈도우 사이즈 설정
	ImGui::SetNextWindowViewport(viewport->ID); //현재 ViewPort에 도킹 윈도우 고정
	ImGui::SetNextWindowBgAlpha(0.0f); //도킹 윈도우의 배경 알파값은 0 (도킹 윈도우의 영역은 보이지 않음)

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockWindow", nullptr, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dock_window_id = ImGui::GetID("DockWindow");
	if (!ImGui::DockBuilderGetNode(dock_window_id)) //이전에 해당 id로 도킹빌드를 하지 않은 경우 추가
	{
		ImGui::DockBuilderRemoveNode(dock_window_id);
		ImGui::DockBuilderAddNode(dock_window_id, ImGuiDockNodeFlags_None);

		//도킹 윈도우에 GUI 레이아웃 배치
		ImGuiID main = dock_window_id;
		ImGuiID down = ImGui::DockBuilderSplitNode(main, ImGuiDir_Down, 0.2f, nullptr, &main);
		ImGuiID down_right = ImGui::DockBuilderSplitNode(down, ImGuiDir_Right, 0.2f, nullptr, &down);
		ImGuiID right = ImGui::DockBuilderSplitNode(main, ImGuiDir_Right, 0.2f, nullptr, &main);
		ImGuiID right2 = ImGui::DockBuilderSplitNode(right, ImGuiDir_Right, 0.2f, nullptr, &right);
		ImGuiID right3 = ImGui::DockBuilderSplitNode(right2, ImGuiDir_Right, 0.2f, nullptr, &right2);

		ImGui::DockBuilderDockWindow(m_gui_map[GUIType::Hierarchy]->m_gui_title.c_str(), main);
		ImGui::DockBuilderDockWindow(m_gui_map[GUIType::Scene]->m_gui_title.c_str(), right);
		ImGui::DockBuilderDockWindow(m_gui_map[GUIType::Game]->m_gui_title.c_str(), right2);
		ImGui::DockBuilderDockWindow(m_gui_map[GUIType::Inspector]->m_gui_title.c_str(), right3);
		ImGui::DockBuilderDockWindow(m_gui_map[GUIType::Project]->m_gui_title.c_str(), down);
		ImGui::DockBuilderDockWindow(m_gui_map[GUIType::Console]->m_gui_title.c_str(), down_right);
		ImGui::DockBuilderFinish(dock_window_id);
	}
	ImGui::DockSpace(dock_window_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
}

void EditorManager::EndDockWindow()
{
	ImGui::End();
}

void EditorManager::ResizeEditor(const UINT& width, const UINT& height)
{
	auto graphics_manager = GraphicsManager::GetInstance();
	graphics_manager->ResizeWindowByUser(width, height);

	ImGui_ImplDX11_InvalidateDeviceObjects();
	ImGui_ImplDX11_CreateDeviceObjects();
}
