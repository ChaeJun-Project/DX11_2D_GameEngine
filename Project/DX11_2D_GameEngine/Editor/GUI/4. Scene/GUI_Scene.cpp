#include "stdafx.h"
#include "GUI_Scene.h"

#include "Editor/ImGuizmo.h"
#include "Helper/IconProvider.h"
#include "Helper/EditorHelper.h"

#include "EditorObject/Camera/CameraEx.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/RenderManager.h>
#include <DX11_2D_GameEngine_Lib/Texture.h>

#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Camera.h>

GUI_Scene::GUI_Scene(const std::string& scene_title)
	:IGUI(scene_title)
{
	m_window_flags |= ImGuiWindowFlags_NoScrollbar;
}

GUI_Scene::~GUI_Scene()
{
}

void GUI_Scene::Initialize()
{

}

void GUI_Scene::Update()
{
	if (m_p_editor_camera == nullptr)
		m_p_editor_camera = dynamic_cast<CameraEx*>(RenderManager::GetInstance()->GetEditorCamera());
}

void GUI_Scene::Render()
{
	ShowProjectionButton();

	ShowScene();

	ShowGizmo();
}

void GUI_Scene::ShowProjectionButton()
{
	if (m_p_editor_camera != nullptr)
	{
		auto is_window_hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenBlockedByPopup);

		m_p_editor_camera->SetIsActvie(is_window_hovered);

		ImGui::PushStyleColor
		(
			ImGuiCol_Button,
			ImGui::GetStyle().Colors[m_p_editor_camera->GetProjectionType() == ProjectionType::Orthographic ? ImGuiCol_ButtonActive : ImGuiCol_Button]
		);

		if (ImGui::Button("2D", ImVec2(50.0f, 0.0f)))
		{
			if (SceneManager::GetInstance()->GetEditorState() == EditorState::EditorState_Stop)
			{
				//직교투영 -> 원근투영
				if (m_p_editor_camera->GetProjectionType() == ProjectionType::Orthographic)
				{
					m_p_editor_camera->SetProjectionType(ProjectionType::Perspective);
				}
				//원근투영 -> 직교투영
				else
				{
					m_p_editor_camera->SetProjectionType(ProjectionType::Orthographic);
				}
			}
		}

		ImGui::PopStyleColor();

		ImGui::Separator();
	}
}

void GUI_Scene::ShowScene()
{
	auto render_manager = RenderManager::GetInstance();

	//Scene 윈도우창을 그릴 위치를 받아옴
	auto window_position_x = (ImGui::GetCursorPos().x + ImGui::GetWindowPos().x);
	auto window_position_y = (ImGui::GetCursorPos().y + ImGui::GetWindowPos().y);

	render_manager->SetScreenOffset(window_position_x, window_position_y);

	//Scene 윈도우창의 크기를 받아옴
	auto scene_window_width = static_cast<UINT>(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x);
	auto scene_window_height = static_cast<UINT>(ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y - 28.0f);

	//Scene 윈도우창의 크기를 항상 짝수로 설정
	scene_window_width -= scene_window_width % 2 != 0 ? 1 : 0;
	scene_window_height -= scene_window_height % 2 != 0 ? 1 : 0;

	//현재 윈도우 사이즈가 변경된 경우
	render_manager->SetResolution(RenderTextureType::EditorScene, scene_window_width, scene_window_height); //RTV, SRV, DSV 재생성

	auto render_texture_srv = render_manager->GetRenderTexture(RenderTextureType::EditorScene)->GetShaderResourceView();
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

//<summary>
//https://github.com/CedricGuillemet/ImGuizmo 튜토리얼 참고
//</summary>
void GUI_Scene::ShowGizmo()
{
	if (SceneManager::GetInstance()->GetEditorState() != EditorState::EditorState_Stop)
		return;

	auto render_manager = RenderManager::GetInstance();
	auto p_editor_camera = render_manager->GetEditorCamera();

	//Editor Camera가 없고 선택된 GameObject가 nullptr인 경우
	if (p_editor_camera == nullptr || EditorHelper::GetInstance()->GetSelectedGameObject() == nullptr)
		return;

	auto p_game_object = EditorHelper::GetInstance()->GetSelectedGameObject();

	auto transform = p_game_object->GetComponent<Transform>();

	static ImGuizmo::OPERATION operation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mode(ImGuizmo::WORLD);

	//Change Gizmo Mode
	if (KEY_PRESS(KeyCode::KEY_1))
		operation = ImGuizmo::TRANSLATE; //Transform Mode

	if (KEY_PRESS(KeyCode::KEY_2))
		operation = ImGuizmo::ROTATE; //Rotation Mode

	if (KEY_PRESS(KeyCode::KEY_3))
		operation = ImGuizmo::SCALE; //Scale Mode

	if (KEY_PRESS(KeyCode::KEY_4))
		operation = ImGuizmo::UNIVERSAL; //Universal Mode(Transform + Rotation + Scale)

	//선택된 GameObject에 부모 GameObject가 있는 경우 Gizmo Mode를 Local로 변경
	if (p_game_object->HasParent())
		mode = ImGuizmo::LOCAL;

	else
		mode = ImGuizmo::WORLD;

	auto offset = render_manager->GetScreenOffset();
	auto size = render_manager->GetResolution();
	auto view = p_editor_camera->GetViewMatrix();
	auto proj = p_editor_camera->GetProjectionMatrix();
	auto world = transform->GetOriginWorldMatrix();

	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(offset.x, offset.y, size.x, size.y);
	ImGuizmo::Manipulate
	(
		view,
		proj,
		operation,
		mode,
		world
	);

	transform->SetTranslation(world.GetTranslation());
	transform->SetRotation(world.GetRotation());
	transform->SetScale(world.GetScale());
}
