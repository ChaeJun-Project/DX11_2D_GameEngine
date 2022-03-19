#include "stdafx.h"
#include "Canvas.h"

#include "ConstantBuffer.h"

#include "Material.h"
#include "Mesh.h"

#include "Scene.h"

#include "GameObject.h"
#include "Transform.h"

Canvas::Canvas()
	:IComponent(ComponentType::Canvas)
{
	m_p_mesh = RESOURCE_MANAGER->GetResource<Mesh>("Rectangle_Mesh");
	auto clone_material = RESOURCE_MANAGER->GetResource<Material>("Canvas_Material")->Clone();
	m_p_material = std::shared_ptr<Material>(clone_material);
}

Canvas::Canvas(const Canvas& origin)
	: IComponent(origin.m_component_type)
{
	m_is_active = origin.m_is_active;

	m_canvas_world_matrix = origin.m_canvas_world_matrix;

	//Material
	auto p_clone_material_raw = origin.m_p_material->Clone();
	m_p_material = std::shared_ptr<Material>(p_clone_material_raw);
	//Mesh
	m_p_mesh = origin.m_p_mesh;

	m_canvas_resolution = origin.m_canvas_resolution;
}

Canvas::~Canvas()
{
	m_p_ui_camera_object = nullptr;
}

void Canvas::Start()
{
    //Set UI Camera Object
	auto p_ui_camera_object = SCENE_MANAGER->GetCurrentScene()->FindGameObjectWithTag(m_ui_camera_object_name);
	if (p_ui_camera_object != nullptr)
		m_p_ui_camera_object = p_ui_camera_object;

	else
		return;

	//Set Canvas Local Position
	auto ui_camera_position = m_p_ui_camera_object->GetComponent<Transform>()->GetLocalTranslation();
	m_p_owner_game_object->GetComponent<Transform>()->SetLocalTranslation(ui_camera_position);

	m_canvas_resolution = RENDER_MANAGER->GetResolution();
}

void Canvas::FinalUpdate()
{
	UpdateCanvasWorldMatrix();
}

void Canvas::Render()
{
	if (m_p_mesh == nullptr || m_p_material == nullptr || m_p_material->GetShader() == nullptr)
		return;

	UpdateConstantBuffer();

	if (SCENE_MANAGER->GetEditorState() == EditorState::EditorState_Stop
		|| SCENE_MANAGER->GetClientState() == 1)
	{
		m_p_material->BindPipeline();

		m_p_mesh->Render();
	}
}

void Canvas::UpdateConstantBuffer()
{
	g_cbuffer_wvpmatrix.world = m_canvas_world_matrix;

	auto constant_buffer = GRAPHICS_MANAGER->GetConstantBuffer(CBuffer_BindSlot::WVPMatrix);
	constant_buffer->SetConstantBufferData(&g_cbuffer_wvpmatrix, sizeof(CBuffer_WVPMatrix));
	constant_buffer->SetBufferBindStage(PipelineStage::VS);
	constant_buffer->BindPipeline();
}

void Canvas::UpdateCanvasWorldMatrix()
{
	auto p_transform = m_p_owner_game_object->GetComponent<Transform>();
	auto world_matrix = p_transform->GetWorldMatrix();

	m_canvas_resolution = RENDER_MANAGER->GetResolution();
	auto scale = Matrix::Scaling(Vector3(m_canvas_resolution.x, m_canvas_resolution.y, 1.0f));

	m_canvas_world_matrix = scale * world_matrix;
}

void Canvas::SaveToScene(FILE* p_file)
{
	__super::SaveToScene(p_file); //IComponent

	//UI Camera
	fprintf(p_file, "[UI Camera]\n");
	fprintf(p_file, "%s\n", m_ui_camera_object_name.c_str());
}

void Canvas::LoadFromScene(FILE* p_file)
{
	__super::LoadFromScene(p_file); //IComponent

	char char_buffer[256] = { 0 };

	//UI Camera
	FILE_MANAGER->FScanf(char_buffer, p_file);
	FILE_MANAGER->FScanf(char_buffer, p_file);
	m_ui_camera_object_name = std::string(char_buffer);
}
