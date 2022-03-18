#include "stdafx.h"
#include "GUI_Scene.h"

#include "Editor/ImGuizmo.h"
#include "Helper/IconProvider.h"
#include "Helper/EditorHelper.h"

#include "EditorObject/Camera/CameraEx.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/GraphicsManager.h>
#include <DX11_2D_GameEngine_Lib/RenderManager.h>
#include <DX11_2D_GameEngine_Lib/Texture.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/Transform.h>
#include <DX11_2D_GameEngine_Lib/Camera.h>
#include <DX11_2D_GameEngine_Lib/RectTransform.h>

GUI_Scene::GUI_Scene(const std::string& scene_title)
	:IGUI(scene_title)
{
	m_window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar;
}

GUI_Scene::~GUI_Scene()
{
	m_p_editor_camera = nullptr;
}

void GUI_Scene::Update()
{
	if (m_p_editor_camera == nullptr)
		m_p_editor_camera = dynamic_cast<CameraEx*>(RENDER_MANAGER->GetEditorCamera());
}

void GUI_Scene::Begin()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	__super::Begin();
}

void GUI_Scene::Render()
{
	ShowProjectionButton();
	ShowScene();
	ShowGizmo();
}

void GUI_Scene::End()
{
	__super::End();
	ImGui::PopStyleVar();
}

void GUI_Scene::ShowProjectionButton()
{
	if (ImGui::BeginMenuBar())
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
				if (SCENE_MANAGER->GetEditorState() == EditorState::EditorState_Stop)
				{
					//�������� -> ��������
					if (m_p_editor_camera->GetProjectionType() == ProjectionType::Orthographic)
					{
						m_p_editor_camera->SetProjectionType(ProjectionType::Perspective);
					}
					//�������� -> ��������
					else
					{
						m_p_editor_camera->SetProjectionType(ProjectionType::Orthographic);
					}
				}
			}

			ImGui::PopStyleColor();
		}
		ImGui::EndMenuBar();
	}
}

void GUI_Scene::ShowScene()
{
	m_gizmo_offset.x = (ImGui::GetWindowPos().x + ImGui::GetCursorPos().x);
	m_gizmo_offset.y = (ImGui::GetWindowPos().y + ImGui::GetCursorPos().y);

	auto viewport_position = ImGui::GetCurrentWindow()->ViewportPos;

	//Scene ������â�� �׸� ��ġ�� �޾ƿ�
	auto window_position_x = (ImGui::GetWindowPos().x - viewport_position.x);
	auto window_position_y = (ImGui::GetWindowPos().y - viewport_position.y + ImGui::GetWindowContentRegionMin().y);

	RENDER_MANAGER->SetScreenOffset(window_position_x, window_position_y);

	//Scene ������â�� ũ�⸦ �޾ƿ�
	auto scene_window_width = static_cast<UINT>(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x);
	auto scene_window_height = static_cast<UINT>(ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);

	//Scene ������â�� ũ�⸦ �׻� ¦���� ����
	scene_window_width -= scene_window_width % 2 != 0 ? 1 : 0;
	scene_window_height -= scene_window_height % 2 != 0 ? 1 : 0;

	//���� ������ ����� ����� ���
	RENDER_MANAGER->SetResolution(scene_window_width, scene_window_height); //RTV, SRV, DSV �����

	auto render_texture = RENDER_MANAGER->GetRenderTexture();

	ImGui::Image
	(
		render_texture ? render_texture->GetShaderResourceView() : nullptr,
		ImVec2(static_cast<float>(scene_window_width), static_cast<float>(scene_window_height)),
		ImVec2(0.0f, 0.0f),
		ImVec2(1.0f, 1.0f)
	);

	//Make Prefab -> GameObject
	if (auto pay_load = DragDropEvent::ReceiveDragDropPayLoad(PayLoadType::Prefab))
	{
		FileFunction::CreatePrefabGameObject(std::get<std::string>(pay_load->data));
	}
}

//<summary>
//https://github.com/CedricGuillemet/ImGuizmo Ʃ�丮�� ����
//</summary>
void GUI_Scene::ShowGizmo()
{
	if (SCENE_MANAGER->GetEditorState() != EditorState::EditorState_Stop)
		return;

	auto p_editor_camera = RENDER_MANAGER->GetEditorCamera();

	auto p_game_object = EDITOR_HELPER->GetSelectedGameObject();

	//Editor Camera�� ���� ���õ� GameObject�� nullptr�� ���
	if (p_editor_camera == nullptr || p_game_object == nullptr)
		return;

	auto p_transform = p_game_object->GetComponent<Transform>();

	static ImGuizmo::OPERATION operation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mode(ImGuizmo::WORLD);

	if (ImGui::IsWindowFocused())
	{
		//Change Gizmo Mode
		if (KEY_PRESS(Key::KEY_1))
			operation = ImGuizmo::TRANSLATE; //Transform Mode

		if (KEY_PRESS(Key::KEY_2))
			operation = ImGuizmo::ROTATE; //Rotation Mode

		if (KEY_PRESS(Key::KEY_3))
			operation = ImGuizmo::SCALE; //Scale Mode

		if (KEY_PRESS(Key::KEY_4))
			operation = ImGuizmo::UNIVERSAL; //Universal Mode(Transform + Rotation + Scale)
	}

	//���õ� GameObject�� �θ� GameObject�� �ִ� ��� Gizmo Mode�� Local�� ����
	if (p_game_object->HasParent())
		mode = ImGuizmo::LOCAL;

	else
		mode = ImGuizmo::WORLD;

	auto size = RENDER_MANAGER->GetResolution();
	auto view = p_editor_camera->GetViewMatrix();
	auto proj = p_editor_camera->GetProjectionMatrix();
	auto world = p_transform->GetOriginWorldMatrix(); //�ڽ� GameObject�� ��� �θ� GameObject�� World Matrix�� ���� ����

	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(m_gizmo_offset.x, m_gizmo_offset.y, size.x, size.y);
	ImGuizmo::Manipulate
	(
		view,
		proj,
		operation,
		mode,
		world
	);

	//�θ� GameObject�� �����ϴ� ���
	//Gizmo�� ���� ���� ���� World�� �θ� GameObject�� OriginWorldMatrix�� ������� �����ָ�
	//�����ϰ� �ڽ� GameObject�� Local ��ȯ���� ���� �� ����
	if (p_game_object->HasParent())
	{
		auto parent_world = p_game_object->GetParent()->GetComponent<Transform>()->GetOriginWorldMatrix();
		world = world * parent_world.Inverse();
	}

	p_transform->SetTranslation(world.GetTranslation());
	p_transform->SetRotation(world.GetRotation());
	p_transform->SetScale(world.GetScale());
}
