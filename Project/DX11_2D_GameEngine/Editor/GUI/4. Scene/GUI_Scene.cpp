#include "stdafx.h"
#include "GUI_Scene.h"

#include "Editor/ImGuizmo.h"
#include "Helper/IconProvider.h"

#include "EditorObject/Camera/CameraEx.h"

#include <DX11_2D_GameEngine_Lib/RenderManager.h>
#include <DX11_2D_GameEngine_Lib/Texture.h>

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

		ImGui::PopStyleColor();

		ImGui::Separator();
	}
}

void GUI_Scene::ShowScene()
{
	auto render_manager = RenderManager::GetInstance();

	//Scene ������â�� �׸� ��ġ�� �޾ƿ�
	auto window_position_x = (ImGui::GetCursorPos().x + ImGui::GetWindowPos().x);
	auto window_position_y = (ImGui::GetCursorPos().y + ImGui::GetWindowPos().y);

	render_manager->SetScreenOffset(window_position_x, window_position_y);

	//Scene ������â�� ũ�⸦ �޾ƿ�
	auto scene_window_width = static_cast<UINT>(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x);
	auto scene_window_height = static_cast<UINT>(ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y - 28.0f);

	//Scene ������â�� ũ�⸦ �׻� ¦���� ����
	scene_window_width -= scene_window_width % 2 != 0 ? 1 : 0;
	scene_window_height -= scene_window_height % 2 != 0 ? 1 : 0;

	//���� ������ ����� ����� ���
	render_manager->SetResolution(RenderTextureType::EditorScene, scene_window_width, scene_window_height); //RTV, SRV, DSV �����

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

void GUI_Scene::ShowGizmo()
{
	////���õ� Actor �����Ͱ� ����� ���(���� ī��Ʈ�� 0�� �Ǿ� ������ ���)
	//if (Editor_Helper::GetInstance()->selected_actor.expired())
	//	return;

	//auto camera = renderer->GetCamera();
	//auto actor = Editor_Helper::GetInstance()->selected_actor.lock();

	////Scene�� ī�޶� �ְ� ���õ� actor�� ī�޶� ���۳�Ʈ�� ���� ���
	//if (camera && !actor->GetComponent<Camera>())
	//{
	//	auto transform = actor->GetTransform();

	//	if (!transform)
	//		return;

	//	static ImGuizmo::OPERATION operation(ImGuizmo::TRANSLATE);
	//	static ImGuizmo::MODE mode(ImGuizmo::WORLD);

	//	if (ImGui::IsKeyPressed(87)) //w
	//		operation = ImGuizmo::TRANSLATE;
	//	if (ImGui::IsKeyPressed(69)) //e
	//		operation = ImGuizmo::ROTATE;
	//	if (ImGui::IsKeyPressed(82)) //r
	//		operation = ImGuizmo::SCALE;

	//	auto offset = renderer->GetEditorOffset();
	//	auto size = renderer->GetResolution();
	//	//D3D�� ����� ����� ����ϰ� OpenGL�� ������ ����� ����ϴµ�
	//	//ImGuizmo�� OpenGLó�� ������ ����� ����ϱ� ������
	//	//���α׷� ������ ����ϴ� ��� ��Ʈ������ ��� ��ġ��ķ� �ٲ� �Ѱ���� ���ϴ� ���·� �������� ��
	//	auto view = camera->GetViewMatrix().Transpose();		//�� ��Ʈ������ ��ġ���
	//	auto proj = camera->GetProjectionMatrix().Transpose();	//���� ��Ʈ������ ��ġ���
	//	auto world = transform->GetWorldMatrix().Transpose();	//���� ��Ʈ������ ��ġ���

	//	ImGuizmo::SetDrawlist();
	//	ImGuizmo::SetRect(offset.x, offset.y, size.x, size.y);
	//	ImGuizmo::Manipulate
	//	(
	//		view,
	//		proj,
	//		operation,
	//		mode,
	//		world
	//	);

	//	//���� ��Ʈ������ ��ġ��ĸ� �ٽ� ��ġ��ķ� ��ȯ -> ���α׷� ������ ����ϴ� ����� ��ķ� �ٽ� ��ȯ�Ѵٴ� �ǹ�
	//	world.Transpose();

	//	transform->SetTranslation(world.GetTranslation());
	//	transform->SetRotation(world.GetRotation());
	//	transform->SetScale(world.GetScale());
	//}
}
