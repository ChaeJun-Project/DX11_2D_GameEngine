#include "stdafx.h"
#include "RenderManager.h"

#include "GraphicsManager.h"

#include "Camera.h"
#include "Light2D.h"

RenderManager::RenderManager()
{
	this->m_camera_vector.resize(1);
}

RenderManager::~RenderManager()
{
	//Camera
	this->m_camera_vector.clear();
	this->m_camera_vector.shrink_to_fit();

	//Light2D
	this->m_light2D_vector.clear();
	this->m_light2D_vector.shrink_to_fit();
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
	if (this->m_camera_vector[0] != nullptr)
		this->m_camera_vector[0]->Render();

	//���� ī�޶� ȭ�� �׸���(index 1����)
	for (UINT i = 1; i < static_cast<UINT>(this->m_camera_vector.size()); ++i)
	{
		if (this->m_camera_vector[i] == nullptr)
			continue;

		this->m_camera_vector[i]->Render();
	}

	//Graphics Swap
	graphis_manager->EndScene();

	//ī�޶� ���� �ʱ�ȭ
	this->m_camera_vector.clear();
	this->m_camera_vector.resize(1);

	//Light2D ���� �ʱ�ȭ
	this->m_light2D_vector.clear();
}

void RenderManager::RegisterCamera(Camera* p_camera, int& camera_index)
{
	//index�� �������� ���� ī�޶���
	if (camera_index == -1)
	{
		this->m_camera_vector.emplace_back(p_camera);
		camera_index = static_cast<int>(this->m_camera_vector.size() - 1);
		return;
	}

	else
	{
		//���ڷ� ���� ī�޶� �ε����� 
		//���� ī�޶� ���� ������� ũ�ų� ���� ���
		if (this->m_camera_vector.size() <= camera_index)
		{
			//ī�޶� ���� ����� ����
			this->m_camera_vector.resize(camera_index);
			this->m_camera_vector[camera_index] = p_camera;
			return;
		}

		//���ڷ� ���� ī�޶� �ε�����
		//���� ī�޶� ���� ������� ���� ���
		else
		{
			//�ش� ī�޶� �ε����� �̹� ī�޶� ��ϵǾ� �ִ� ���
			if (this->m_camera_vector[camera_index] != nullptr)
			{
				//�̹� ���� ī�޶� ��ϵǾ� �ִµ� ���ο� ���� ī�޶� ���� ���
				if (camera_index == 0)
				{
					assert(false); //����üũ
				}

				this->m_camera_vector.emplace_back(p_camera);
				camera_index = static_cast<int>(this->m_camera_vector.size() - 1);
				return;
			}

			else
				this->m_camera_vector[camera_index] = p_camera;

		}
	}
}

Camera* RenderManager::GetMainCamera()
{
	if (this->m_camera_vector[0] == nullptr)
		return nullptr;

	return this->m_camera_vector[0];
}

void RenderManager::RegisterLight2D(Light2D* p_light2D, int& light2D_index)
{
	this->m_light2D_vector.emplace_back(p_light2D);
	light2D_index = static_cast<int>(this->m_light2D_vector.size() - 1);
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
	for (UINT i = 0; i < static_cast<UINT>(this->m_light2D_vector.size()); ++i)
	{
		m_light2Ds_data.light2D_array[i] = this->m_light2D_vector[i]->GetLight2DInfo();
	}
	//Light2D ���� ������Ʈ
	m_light2Ds_data.light_count = static_cast<UINT>(this->m_light2D_vector.size());

	constant_buffer = GraphicsManager::GetInstance()->GetConstantBuffer(CBuffer_BindSlot::Light2D);
	constant_buffer->SetConstantBufferData(&m_light2Ds_data, sizeof(CBuffer_Light2D));
	constant_buffer->SetBufferBindStage(PipelineStage::Graphics_ALL);
	constant_buffer->BindPipeline();
}
