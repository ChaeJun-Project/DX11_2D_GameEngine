#include "stdafx.h"
#include "RenderManager.h"

#include "GraphicsManager.h"

#include "Camera.h"
#include "Light2D.h"

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
}

void RenderManager::Render()
{
	//Program, Light2D 데이터 업데이트
	UpdateConstantBuffer();

	//Graphics Manager
	auto graphis_manager = GraphicsManager::GetInstance();

	//Graphics Clear Target
	graphis_manager->BeginScene();

	//메인 카메라(index 0) 기준으로 화면 그리기
	if (m_camera_vector[0] != nullptr)
	{
		m_camera_vector[0]->SortObjects();
		m_camera_vector[0]->RenderForwardObjects();
		m_camera_vector[0]->RenderParticleObjects();

		//TODO
		//RenderTargetView의 이미지를 카피하는 작업

		m_camera_vector[0]->RenderPostEffectObjects();
	}

	//서브 카메라 화면 그리기(index 1부터)
	for (UINT i = 1; i < static_cast<UINT>(m_camera_vector.size()); ++i)
	{
		if (m_camera_vector[i] == nullptr)
			continue;

		m_camera_vector[i]->SortObjects();
		m_camera_vector[i]->RenderForwardObjects();
	}

	//Graphics Swap
	graphis_manager->EndScene();

	//카메라 벡터 초기화
	m_camera_vector.clear();
	m_camera_vector.resize(1);

	//Light2D 벡터 초기화
	m_light2D_vector.clear();
}

void RenderManager::RegisterCamera(Camera* p_camera, int& camera_index)
{
	//index가 설정되지 않은 카메라라면
	if (camera_index == -1)
	{
		m_camera_vector.emplace_back(p_camera);
		camera_index = static_cast<int>(m_camera_vector.size() - 1);
		return;
	}

	else
	{
		//인자로 들어온 카메라 인덱스가 
		//현재 카메라 벡터 사이즈보다 크거나 같은 경우
		if (m_camera_vector.size() <= camera_index)
		{
			//카메라 벡터 사이즈를 증가
			m_camera_vector.resize(camera_index);
			m_camera_vector[camera_index] = p_camera;
			return;
		}

		//인자로 들어온 카메라 인덱스가
		//현재 카메라 벡터 사이즈보다 작은 경우
		else
		{
			//해당 카메라 인덱스에 이미 카메라가 등록되어 있는 경우
			if (m_camera_vector[camera_index] != nullptr)
			{
				//이미 메인 카메라가 등록되어 있는데 새로운 메인 카메라를 넣을 경우
				if (camera_index == 0)
				{
					assert(false); //오류체크
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

	//Light2D 데이터 업데이트
	for (UINT i = 0; i < static_cast<UINT>(m_light2D_vector.size()); ++i)
	{
		m_light2Ds_data.light2D_array[i] = m_light2D_vector[i]->GetLight2DInfo();
	}
	//Light2D 개수 업데이트
	m_light2Ds_data.light_count = static_cast<UINT>(m_light2D_vector.size());

	constant_buffer = GraphicsManager::GetInstance()->GetConstantBuffer(CBuffer_BindSlot::Light2D);
	constant_buffer->SetConstantBufferData(&m_light2Ds_data, sizeof(CBuffer_Light2D));
	constant_buffer->SetBufferBindStage(PipelineStage::Graphics_ALL);
	constant_buffer->BindPipeline();
}
