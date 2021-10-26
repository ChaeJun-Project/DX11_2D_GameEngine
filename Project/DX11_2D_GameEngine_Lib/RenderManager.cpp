#include "stdafx.h"
#include "RenderManager.h"

RenderManager::RenderManager()
{
	this->m_camera_vector.reserve(1);
	this->m_camera_vector.resize(1);
}

RenderManager::~RenderManager()
{
}

void RenderManager::Render()
{
    //메인 카메라 기준으로 화면 그리기
	if (this->m_camera_vector[0] != nullptr)
	{

	}


	this->m_camera_vector.clear();
	this->m_camera_vector.shrink_to_fit();
	this->m_camera_vector.resize(1);
}

void RenderManager::RegisterCamera(Camera* p_camera, int camera_index)
{
}
