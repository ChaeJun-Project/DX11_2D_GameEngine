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

void GUI_Scene::ShowGizmo()
{
	////선택된 Actor 포인터가 만료된 경우(참조 카운트가 0이 되어 해제된 경우)
	//if (Editor_Helper::GetInstance()->selected_actor.expired())
	//	return;

	//auto camera = renderer->GetCamera();
	//auto actor = Editor_Helper::GetInstance()->selected_actor.lock();

	////Scene에 카메라가 있고 선택된 actor에 카메라 컴퍼넌트가 없는 경우
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
	//	//D3D는 행기준 행렬을 사용하고 OpenGL은 열기준 행렬을 사용하는데
	//	//ImGuizmo는 OpenGL처럼 열기준 행렬을 사용하기 때문에
	//	//프로그램 내에서 사용하는 행렬 메트릭스를 모두 전치행렬로 바꿔 넘겨줘야 원하는 형태로 렌더링이 됨
	//	auto view = camera->GetViewMatrix().Transpose();		//뷰 메트릭스의 전치행렬
	//	auto proj = camera->GetProjectionMatrix().Transpose();	//투영 메트릭스의 전치행렬
	//	auto world = transform->GetWorldMatrix().Transpose();	//월드 메트릭스의 전치행렬

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

	//	//월드 메트릭스의 전치행렬를 다시 전치행렬로 변환 -> 프로그램 내에서 사용하는 행기준 행렬로 다시 변환한다는 의미
	//	world.Transpose();

	//	transform->SetTranslation(world.GetTranslation());
	//	transform->SetRotation(world.GetRotation());
	//	transform->SetScale(world.GetScale());
	//}
}
