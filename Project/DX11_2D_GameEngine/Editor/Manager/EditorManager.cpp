#include "stdafx.h"
#include "EditorManager.h"
#include <DX11_2D_GameEngine_Lib/GraphicsManager.h>

//Helper
#include "Helper/EditorHelper.h"
#include "Helper/IconProvider.h"

//GUI
#include "GUI/1. MenuBar/GUI_MenuBar.h"
#include "GUI/6. Inspector/GUI_Inspector.h"


EditorManager::EditorManager()
{
}

EditorManager::~EditorManager()
{
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
	//Initialize Editor Helper
	EditorHelper::GetInstance()->Initialize();

    //Initialize Icon Provider
	IconProvider::GetInstance()->Initialize();

	//<summary>
	//imgui_docking ������ DirectX11 Sample �ڵ带 ���� �ۼ���
	//</summary>

	//ImGui ���� üũ
	IMGUI_CHECKVERSION();

	//ImGui Context ����
	ImGui::CreateContext();

	//ImGui Context ����
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  //Ű���� �Է� ���
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      //��ŷ �ý��� ���
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    //��Ƽ ����Ʈ ���(������ �÷���)

	//Set GUI Style(Current: Dark Color)
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	//ImGui�� ������ �ڵ鰪 ����
	ImGui_ImplWin32_Init(hwnd);
	//ImGui�� D3D11 Device ��ü��` D3D11 DeviceContext ��ü ���� ����
	ImGui_ImplDX11_Init(device, device_context);

	//Add GUIs
	m_gui_map.insert(std::make_pair(GUIType::MenuBar, std::make_unique<GUI_MenuBar>("Main Menu Bar")));			//MenuBar
	//m_gui_map.insert(std::make_pair(GUIType::ToolBar, std::make_unique<>()));			//ToolBar
	//m_gui_map.insert(std::make_pair(GUIType::Hierarchy, std::make_unique<>()));		//Hierarchy
	//m_gui_map.insert(std::make_pair(GUIType::Scene, std::make_unique<>()));		    //Scene
	//m_gui_map.insert(std::make_pair(GUIType::Game, std::make_unique<>()));            //Game
	m_gui_map.insert(std::make_pair(GUIType::Inspector, std::make_unique<GUI_Inspector>("Inspector")));		//Inspector
	//m_gui_map.insert(std::make_pair(GUIType::Project, std::make_unique<>()));			//Project
	//m_gui_map.insert(std::make_pair(GUIType::Console, std::make_unique<>()));			//Console(Log)
}

void EditorManager::Update()
{
	for (auto& gui : m_gui_map)
	{
		if (gui.second != nullptr)
			gui.second->Update();
	}
}

void EditorManager::Render()
{
	//New Frame ImGui
	ImGuiNewFrame();

	for (auto& gui : m_gui_map)
	{
		if (gui.second != nullptr && gui.second->GetIsActive())
		{ 
			gui.second->Begin();
			gui.second->Render();
			gui.second->End();
		}
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

void EditorManager::ImGuiNewFrame()
{
    //DX11 ���� ȣ�� �� Win32 ȣ���ؾ� ������ ������ ����
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void EditorManager::ResizeEditor(const UINT& width, const UINT& height)
{
	auto graphics_manager = GraphicsManager::GetInstance();
	graphics_manager->ResizeWindowByUser(width, height);

	ImGui_ImplDX11_InvalidateDeviceObjects();
	ImGui_ImplDX11_CreateDeviceObjects();
}
