#include "stdafx.h"
#include "GUI_Game.h"

#include <DX11_2D_GameEngine_Lib/RenderManager.h>

#include <DX11_2D_GameEngine_Lib/Texture.h>

GUI_Game::GUI_Game(const std::string& game_title)
	:IGUI(game_title)
{
	m_window_flags |= ImGuiWindowFlags_NoScrollbar;
}

GUI_Game::~GUI_Game()
{
}

void GUI_Game::Initialize()
{

}

void GUI_Game::Update()
{

}

void GUI_Game::Render()
{
	ShowGame();
}

void GUI_Game::ShowGame()
{
	//Game ������â�� ũ�⸦ �޾ƿ�
	auto scene_window_width = static_cast<UINT>(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x);
	auto scene_window_height = static_cast<UINT>(ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);

	//Game ������â�� ũ�⸦ �׻� ¦���� ����
	scene_window_width -= scene_window_width % 2 != 0 ? 1 : 0;
	scene_window_height -= scene_window_height % 2 != 0 ? 1 : 0;

	auto render_manager = RenderManager::GetInstance();

	//���� ������ ����� ����� ���
	render_manager->SetResolution(RenderTextureType::GameScene, scene_window_width, scene_window_height); //RTV, SRV, DSV �����

	auto render_texture_srv = render_manager->GetRenderTexture(RenderTextureType::GameScene)->GetShaderResourceView();
	ImGui::Image
	(
		render_texture_srv ? render_texture_srv : nullptr,
		ImVec2(static_cast<float>(scene_window_width), static_cast<float>(scene_window_height)),
		ImVec2(0.0f, 0.0f),
		ImVec2(1.0f, 1.0f),
		ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
		ImVec4(1.0f, 1.0f, 1.0f, 0.5f)
	);
}
