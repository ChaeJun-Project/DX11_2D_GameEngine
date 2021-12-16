#include "stdafx.h"
#include "RenderManager.h"

#include "Core.h"
#include "Settings.h"

#include "GraphicsManager.h"

#include "ResourceManager.h"
#include "Material.h"
#include "Texture.h"

#include "SceneManager.h"

#include "Camera.h"
#include "Light2D.h"

RenderManager::RenderManager()
{
	m_camera_vector.resize(1);
}

RenderManager::~RenderManager()
{
	//Render Texture Map
	for (auto& p_render_texture : m_p_render_texture_map)
	{
		p_render_texture.second.reset();
	}
	m_p_render_texture_map.clear();

	//Camera
	m_camera_vector.clear();
	m_camera_vector.shrink_to_fit();

	//Light2D
	m_light2D_vector.clear();
	m_light2D_vector.shrink_to_fit();

	//Post Effect Texture
	m_p_post_effect_target_texture.reset();
}

void RenderManager::Initialize()
{
	CreateRenderTexture(RenderTextureType::GameScene, g_cbuffer_program.resolution.x, g_cbuffer_program.resolution.y);
	CreateRenderTexture(RenderTextureType::EditorScene, g_cbuffer_program.resolution.x, g_cbuffer_program.resolution.y);

	//Post Effect Material�� Texture ����
	//auto post_effect_material = ResourceManager::GetInstance()->GetMaterial("PostEffect");
	//post_effect_material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, m_p_post_effect_target_texture);

	auto water_material = ResourceManager::GetInstance()->GetMaterial("WaterEffect");
	water_material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, m_p_post_effect_target_texture);
}

void RenderManager::Render()
{
	//Program, Light2D ������ ������Ʈ
	UpdateConstantBuffer();

	auto scene_manager = SceneManager::GetInstance();

	//<summary>
	//ClientState
	//Title = 0
	//Game = 1
	//Editor = 2

	switch (scene_manager->GetClientState())
	{
	case 0:
		RenderTitle();
		break;
	case 1:
		RenderPlay();
		break;
	case 2:
	{
		RenderEditor();
		RenderPlay();
	}
	break;
	}

	//ī�޶� ���� �ʱ�ȭ
	m_camera_vector.clear();
	m_camera_vector.resize(1);

	//Light2D ���� �ʱ�ȭ
	m_light2D_vector.clear();

	//Graphics Clear Target
	GraphicsManager::GetInstance()->BeginScene();
}

void RenderManager::RenderTitle()
{
	auto settings = Core::GetInstance()->GetSettings();
	m_resolution_size.x = static_cast<float>(settings->GetWindowWidth());
	m_resolution_size.y = static_cast<float>(settings->GetWindowHeight());
}

void RenderManager::RenderPlay()
{
	SetRenderTexture(RenderTextureType::GameScene);

	//���� ī�޶�(index 0) �������� ȭ�� �׸���
	if (m_camera_vector[0] != nullptr)
	{
		m_camera_vector[0]->UpdateMatrix();

		m_camera_vector[0]->SortObjects();
		m_camera_vector[0]->RenderForwardObjects();
		m_camera_vector[0]->RenderParticleObjects();
		m_camera_vector[0]->RenderPostEffectObjects();
	}

	//���� ī�޶� ȭ�� �׸���(index 1����)
	for (UINT i = 1; i < static_cast<UINT>(m_camera_vector.size()); ++i)
	{
		if (m_camera_vector[i] == nullptr)
			continue;

		m_camera_vector[i]->SortObjects();
		m_camera_vector[i]->RenderForwardObjects();
	}
}

void RenderManager::RenderEditor()
{
	SetRenderTexture(RenderTextureType::EditorScene);

	//Editor Camera �������� ȭ�� �׸���
	if (m_p_editor_camera != nullptr)
	{
		m_p_editor_camera->UpdateMatrix();

		m_p_editor_camera->SortObjects();
		m_p_editor_camera->RenderForwardObjects();
		m_p_editor_camera->RenderParticleObjects();
		m_p_editor_camera->RenderPostEffectObjects();
	}
}

void RenderManager::SetRenderTexture(const RenderTextureType& render_texture_type)
{
	auto render_texture = m_p_render_texture_map[render_texture_type];

	if (render_texture != nullptr)
	{
		auto p_render_target_view = render_texture->GetRenderTargetView();
		auto p_depth_stencil_view = render_texture->GetDepthStencilView();
		auto view_port = render_texture->GetViewPort();

		//�� ���ۿ� �׷��� ����(render_target_view)�� Output_Merger�� ����Ÿ������ ����
		DEVICE_CONTEXT->OMSetRenderTargets(1, &p_render_target_view, p_depth_stencil_view);
		//������ ����Ʈ ���
		DEVICE_CONTEXT->RSSetViewports(1, &view_port);
		//�� ����(render_target_view)�� �׷��� ���� �����
		DEVICE_CONTEXT->ClearRenderTargetView(p_render_target_view, Color4::Black);
		//���� ���� ���� �����
		DEVICE_CONTEXT->ClearDepthStencilView(p_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		m_resolution_size = Vector2(view_port.Width, view_port.Height);
	}
}

//Camera Component�� RenderPostEffectObjects���� ȣ��
void RenderManager::CopyPostEffect()
{
	//Render Target Texture
	auto render_target_textre = ResourceManager::GetInstance()->GetTexture("RenderTargetView");

	//Render Target Texture�� �̹����� ī��
	DEVICE_CONTEXT->CopyResource(m_p_post_effect_target_texture->GetTexture(), render_target_textre->GetTexture());
}

void RenderManager::ResizePostEffectTexture()
{
	auto settings = Core::GetInstance()->GetSettings();
	auto resource_manager = ResourceManager::GetInstance();

	//���� ����    
	if (m_p_post_effect_target_texture == nullptr)
	{
		//���� �ػ󵵿� �°� Post Effect Target�ؽ�ó ����
		m_p_post_effect_target_texture = resource_manager->CreateTexture
		(
			"PostEffectTarget",
			settings->GetWindowWidth(),
			settings->GetWindowHeight(),
			DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_SHADER_RESOURCE
		);
	}
	//���� ������ Post Effect Target Texture�� �ִٸ�
	else
	{
		//Post Effect Target Texture ����
		m_p_post_effect_target_texture->ReleaseTexture();

		//���� ����
		m_p_post_effect_target_texture->Create
		(
			settings->GetWindowWidth(),
			settings->GetWindowHeight(),
			DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_SHADER_RESOURCE
		);
	}
}

void RenderManager::RegisterCamera(Camera* p_camera, int& camera_index)
{
	//index�� �������� ���� ī�޶���
	if (camera_index == -1)
	{
		m_camera_vector.emplace_back(p_camera);
		camera_index = static_cast<int>(m_camera_vector.size() - 1);
		return;
	}

	else
	{
		//���ڷ� ���� ī�޶� �ε����� 
		//���� ī�޶� ���� ������� ũ�ų� ���� ���
		if (m_camera_vector.size() <= camera_index)
		{
			//ī�޶� ���� ����� ����
			m_camera_vector.resize(camera_index);
			m_camera_vector[camera_index] = p_camera;
			return;
		}

		//���ڷ� ���� ī�޶� �ε�����
		//���� ī�޶� ���� ������� ���� ���
		else
		{
			//�ش� ī�޶� �ε����� �̹� ī�޶� ��ϵǾ� �ִ� ���
			if (m_camera_vector[camera_index] != nullptr)
			{
				//�̹� ���� ī�޶� ��ϵǾ� �ִµ� ���ο� ���� ī�޶� ���� ���
				if (camera_index == 0)
				{
					assert(false); //����üũ
				}

				m_camera_vector.emplace_back(p_camera);
				camera_index = static_cast<int>(m_camera_vector.size() - 1);
				return;
			}

			else
				m_camera_vector[camera_index] = p_camera;

		}
	}
}

Camera* RenderManager::GetMainCamera()
{
	if (m_camera_vector[0] == nullptr)
		return nullptr;

	return m_camera_vector[0];
}

void RenderManager::RegisterLight2D(Light2D* p_light2D, int& light2D_index)
{
	m_light2D_vector.emplace_back(p_light2D);
	light2D_index = static_cast<int>(m_light2D_vector.size() - 1);
}

void RenderManager::UpdateConstantBuffer()
{
	//=============================================
	//Program
	//=============================================
	auto constant_buffer = GraphicsManager::GetInstance()->GetConstantBuffer(CBuffer_BindSlot::Program);
	constant_buffer->SetConstantBufferData(&g_cbuffer_program, sizeof(CBuffer_Program));
	constant_buffer->SetBufferBindStage(PipelineStage::Graphics_ALL | PipelineStage::CS);
	constant_buffer->BindPipeline();

	//=============================================
	//Light2D
	//=============================================
	CBuffer_Light2D m_light2Ds_data;
	ZeroMemory(&m_light2Ds_data, sizeof(CBuffer_Light2D));

	//Light2D ������ ������Ʈ
	for (UINT i = 0; i < static_cast<UINT>(m_light2D_vector.size()); ++i)
	{
		m_light2Ds_data.light2D_array[i] = m_light2D_vector[i]->GetLight2DInfo();
	}
	//Light2D ���� ������Ʈ
	m_light2Ds_data.light_count = static_cast<UINT>(m_light2D_vector.size());

	constant_buffer = GraphicsManager::GetInstance()->GetConstantBuffer(CBuffer_BindSlot::Light2D);
	constant_buffer->SetConstantBufferData(&m_light2Ds_data, sizeof(CBuffer_Light2D));
	constant_buffer->SetBufferBindStage(PipelineStage::Graphics_ALL);
	constant_buffer->BindPipeline();
}

void RenderManager::SetResolution(const RenderTextureType& render_texture_type, const UINT& width, const UINT& height)
{
	if (width == 0 || height == 0)
	{
		return;
	}

	CreateRenderTexture(render_texture_type, width, height);
}

void RenderManager::CreateRenderTexture(const RenderTextureType& render_texture_type, const UINT& width, const UINT& height)
{
	auto map_iter = m_p_render_texture_map.find(render_texture_type);

	//�߰��� �� �Ǿ��ִ� ���
	if (map_iter == m_p_render_texture_map.end())
	{
		//Create Render Target View
		auto render_texture = std::make_shared<Texture>("");

		render_texture->Create
		(
			width,
			height,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		);

		//Create Depth Stencil View
		render_texture->Create
		(
			width,
			height,
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			D3D11_BIND_DEPTH_STENCIL
		);

		//������ Render Texture�� Map�� �߰�
		auto render_texture_iter = m_p_render_texture_map.insert(std::make_pair(render_texture_type, render_texture));
		auto result = render_texture_iter.second;
		assert(result);
	}

	//�̹� �߰��� �� ���
	else
	{
		auto render_texture = m_p_render_texture_map[render_texture_type];

		//Create Render Target View
		render_texture->Create
		(
			width,
			height,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		);

		//Create Depth Stencil View
		render_texture->Create
		(
			width,
			height,
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			D3D11_BIND_DEPTH_STENCIL
		);
	}
}

const std::shared_ptr<Texture>& RenderManager::GetRenderTexture(const RenderTextureType& render_texture_type)
{
	auto render_texture_iter = m_p_render_texture_map.find(render_texture_type);

	//�ش� Render Texture�� ã�� ������ ���
	if (render_texture_iter == m_p_render_texture_map.end())
		return nullptr;

	return render_texture_iter->second;
}

