#include "stdafx.h"
#include "RenderManager.h"

#include "Core.h"
#include "Settings.h"

#include "GraphicsManager.h"

#include "Camera.h"
#include "Light2D.h"

#include "ResourceManager.h"
#include "Material.h"
#include "Texture.h"

RenderManager::RenderManager()
{
	m_camera_vector.resize(1);
}

RenderManager::~RenderManager()
{
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

	//Graphics Manager
	auto graphis_manager = GraphicsManager::GetInstance();

	//Graphics Clear Target
	graphis_manager->BeginScene();

	//���� ī�޶�(index 0) �������� ȭ�� �׸���
	if (m_camera_vector[0] != nullptr)
	{
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

	//Graphics Swap
	graphis_manager->EndScene();

	//ī�޶� ���� �ʱ�ȭ
	m_camera_vector.clear();
	m_camera_vector.resize(1);

	//Light2D ���� �ʱ�ȭ
	m_light2D_vector.clear();
}

//Camera Component�� RenderPostEffectObjects���� ȣ��
void RenderManager::CopyPostEffect()
{
	//Render Target Texture
	auto render_target_textre = ResourceManager::GetInstance()->GetTexture("RenderTargetView");

	//Render Target Texture�� �̹����� ī��
	auto device_context = GraphicsManager::GetInstance()->GetDeviceContext();
	device_context->CopyResource(m_p_post_effect_target_texture->GetTexture(), render_target_textre->GetTexture());
}

void RenderManager::ResizePostEffectTexture()
{
	auto settings = Core::GetInstance()->GetSettings();
	auto resource_manager = ResourceManager::GetInstance();

    //���� ����    
    if(m_p_post_effect_target_texture == nullptr)
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