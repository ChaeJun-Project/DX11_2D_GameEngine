#include "stdafx.h"
#include "RenderManager.h"

#include "Core.h"
#include "Settings.h"

#include "GraphicsManager.h"
#include "ConstantBuffer.h"

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
	//Render Texture 
	m_p_render_target_texture.reset();
	m_p_depth_stencil_texture.reset();

	//Camera
	m_camera_vector.clear();
	m_camera_vector.shrink_to_fit();

	//Light2D
	m_light2D_vector.clear();
	m_light2D_vector.shrink_to_fit();

	//Post Effect Texture
	m_p_post_effect_render_target_texture.reset();
}

void RenderManager::Initialize()
{
	
}

void RenderManager::Render()
{
	//Render Debug Mode 
	if (KEY_DOWN(Key::KEY_G))
	{
		m_is_debug_mode = !m_is_debug_mode;
	}

	GRAPHICS_MANAGER->ClearRenderTarget();

	//<summary>
	//ClientState
	//Game = 1
	//Editor = 2
	switch (SCENE_MANAGER->GetClientState())
	{
	case 1:
		RenderPlay();
		break;
	case 2:
		RenderEditor();
		//윈도우 창 전체를 그릴 Render Target 연결
		GRAPHICS_MANAGER->SetRenderTarget();
		break;
	}

	CalcClientSceneRect();

	ClearCameraAndLight();
}

void RenderManager::RenderPlay()
{
	//Graphics Clear Target
	GRAPHICS_MANAGER->SetRenderTarget();

	m_p_render_target_texture = GRAPHICS_MANAGER->GetRenderTexture();
	m_p_depth_stencil_texture = GRAPHICS_MANAGER->GetDepthStencilTexture();
	m_client_resolution_size = Vector2(m_p_render_target_texture->GetWidth(), m_p_render_target_texture->GetHeight());

	//Program 데이터 업데이트
	UpdateConstantBuffer();

	ResizePostEffectTexture(static_cast<UINT>(m_client_resolution_size.x), static_cast<UINT>(m_client_resolution_size.y));

	//메인 카메라(index 0) 기준으로 화면 그리기
	if (m_camera_vector[0] != nullptr)
	{
		m_camera_vector[0]->UpdateMatrix();

		m_camera_vector[0]->SortObjects();
		m_camera_vector[0]->RenderForwardObjects();
		m_camera_vector[0]->RenderParticleObjects();
		m_camera_vector[0]->RenderPostEffectObjects();
	}

	//서브 카메라 화면 그리기(index 1부터)
	for (UINT i = 1; i < static_cast<UINT>(m_camera_vector.size()); ++i)
	{
		if (m_camera_vector[i] == nullptr)
			continue;

		m_camera_vector[i]->UpdateMatrix();

		m_camera_vector[i]->SortObjects();
		m_camera_vector[i]->RenderForwardObjects();
		m_camera_vector[i]->RenderPostEffectObjects();
	}

	RenderDebugMode();
}

void RenderManager::RenderEditor()
{
	if (m_p_render_target_texture == nullptr)
		return;

	ClearRenderTexture();
	SetRenderTexture();

	//Program 데이터 업데이트
	UpdateConstantBuffer();

	ResizePostEffectTexture(static_cast<UINT>(m_client_resolution_size.x), static_cast<UINT>(m_client_resolution_size.y));

	if (SCENE_MANAGER->GetEditorState() == EditorState::EditorState_Stop)
	{
		//Editor Camera 기준으로 화면 그리기
		if (m_p_editor_camera != nullptr)
		{
			m_p_editor_camera->UpdateMatrix();

			m_p_editor_camera->SortObjects();
			m_p_editor_camera->RenderForwardObjects();
			m_p_editor_camera->RenderParticleObjects();
			m_p_editor_camera->RenderPostEffectObjects();
		}
	}

	else
	{
		if (m_client_resolution_size != SETTINGS->GetGameResolution())
		{
			auto game_resolution = SETTINGS->GetGameResolution();
			SetResolution(static_cast<UINT>(game_resolution.x), static_cast<UINT>(game_resolution.y));
			m_client_resolution_size = SETTINGS->GetGameResolution();
			SetRenderTexture();
		}

		//메인 카메라(index 0) 기준으로 화면 그리기
		if (m_camera_vector[0] != nullptr)
		{
			m_camera_vector[0]->UpdateMatrix();

			m_camera_vector[0]->SortObjects();
			m_camera_vector[0]->RenderForwardObjects();
			m_camera_vector[0]->RenderParticleObjects();
		}

		//서브 카메라 화면 그리기(index 1부터)
		for (UINT i = 1; i < static_cast<UINT>(m_camera_vector.size()); ++i)
		{
			if (m_camera_vector[i] == nullptr)
				continue;

			m_camera_vector[i]->UpdateMatrix();

			m_camera_vector[i]->SortObjects();
			m_camera_vector[i]->RenderForwardObjects();
			m_camera_vector[i]->RenderPostEffectObjects();
		}
	}

	RenderDebugMode();
}

void RenderManager::RenderDebugMode()
{
	if (m_is_debug_mode)
	{
		//Render Time Manager
		TIME_MANAGER->Render();
		//Render Input Manager
		INPUT_MANAGER->Render();
	}
}

void RenderManager::CalcClientSceneRect()
{
	//Left Top
	m_client_rect_left_top = m_screen_offset;

	//Right Bottom
	m_client_rect_right_bottom = m_client_rect_left_top + m_client_resolution_size;
}

void RenderManager::ClearRenderTexture()
{
	auto p_render_target_view = m_p_render_target_texture->GetRenderTargetView();
	auto p_depth_stencil_view = m_p_depth_stencil_texture->GetDepthStencilView();

	//render_target_view에 그려진 내용 지우기
	DEVICE_CONTEXT->ClearRenderTargetView(p_render_target_view, Vector4::Black);
	//깊이 버퍼 내용 지우기
	DEVICE_CONTEXT->ClearDepthStencilView(p_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void RenderManager::SetRenderTexture()
{
	auto p_render_target_view = m_p_render_target_texture->GetRenderTargetView();
	auto p_depth_stencil_view = m_p_depth_stencil_texture->GetDepthStencilView();
	auto view_port = m_p_render_target_texture->GetViewPort();

	//백 버퍼에 그려진 내용(render_target_view)을 Output_Merger의 렌더타겟으로 설정
	DEVICE_CONTEXT->OMSetRenderTargets(1, &p_render_target_view, p_depth_stencil_view);

	//설정한 뷰포트 등록
	DEVICE_CONTEXT->RSSetViewports(1, &view_port);

	m_client_resolution_size = Vector2(view_port.Width, view_port.Height);
}

const bool RenderManager::CheckMouseWorldPositionInRect(const Vector2& mouse_position, const Vector2& rect_left_top, const Vector2& rect_right_bottom)
{
	//현재 마우스 커서의 위치가 Rect 내부에 있는 경우
	if (rect_left_top.x < mouse_position.x && rect_left_top.y > mouse_position.y &&
		rect_right_bottom.x > mouse_position.x && rect_right_bottom.y < mouse_position.y)
		return true;

	else
		return false;
}

const bool RenderManager::CheckMouseClientPositionInRect(const Vector2& mouse_position, const Vector2& rect_left_top, const Vector2& rect_right_bottom)
{
	//현재 마우스 커서의 위치가 Rect 내부에 있는 경우
	if (rect_left_top.x < mouse_position.x && rect_left_top.y < mouse_position.y &&
		rect_right_bottom.x > mouse_position.x && rect_right_bottom.y > mouse_position.y)
		return true;

	else
		return false;
}

const bool RenderManager::CheckClickedEditorSceneRect(const Vector2& mouse_position)
{
	if (CheckMouseClientPositionInRect(
		mouse_position,
		m_client_rect_left_top,
		m_client_rect_right_bottom))
	{
		return true;
	}

	return false;
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
			m_camera_vector.resize(m_camera_vector.size() + camera_index);
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

Camera* RenderManager::GetCamera(const UINT& camera_index)
{
	if (m_camera_vector.size() <= camera_index || m_camera_vector[camera_index] == nullptr)
		return nullptr;

	return m_camera_vector[camera_index];
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
	g_cbuffer_program.client_resolution = m_client_resolution_size;
	g_cbuffer_program.game_resolution = SETTINGS->GetGameResolution();

	auto constant_buffer = GRAPHICS_MANAGER->GetConstantBuffer(CBuffer_BindSlot::Program);
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

	constant_buffer = GRAPHICS_MANAGER->GetConstantBuffer(CBuffer_BindSlot::Light2D);
	constant_buffer->SetConstantBufferData(&m_light2Ds_data, sizeof(CBuffer_Light2D));
	constant_buffer->SetBufferBindStage(PipelineStage::PS);
	constant_buffer->BindPipeline();
}

void RenderManager::ClearCameraAndLight()
{
	//카메라 벡터 초기화
	m_camera_vector.clear();
	m_camera_vector.resize(1);

	//Light2D 벡터 초기화
	m_light2D_vector.clear();
}

void RenderManager::SetResolution(const UINT& width, const UINT& height)
{
	if (width == 0 || height == 0)
	{
		return;
	}

	CreateRenderTexture(width, height);
}

void RenderManager::CreateRenderTexture(const UINT& width, const UINT& height)
{
	//추가가 안 되어있는 경우
	if (m_p_render_target_texture == nullptr)
	{
		//Create Render Target View
		m_p_render_target_texture = std::make_shared<Texture>("Render Texture");

		m_p_render_target_texture->Create
		(
			width,
			height,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		);

		//Create Depth Stencil View
		m_p_depth_stencil_texture = std::make_shared<Texture>("Depth Stencil Texture");

		//Create Depth Stencil View
		m_p_depth_stencil_texture->Create
		(
			width,
			height,
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			D3D11_BIND_DEPTH_STENCIL
		);
	}

	//이미 추가가 된 경우 -> 이미 만들어진 View 자원을 해제하고 새로운 해상도의 View 자원을 생성
	else
	{
		if (static_cast<UINT>(m_client_resolution_size.x) == width && static_cast<UINT>(m_client_resolution_size.y) == height)
			return;

		//Create Render Target View
		m_p_render_target_texture->Create
		(
			width,
			height,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		);

		//Create Depth Stencil View
		m_p_depth_stencil_texture->Create
		(
			width,
			height,
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			D3D11_BIND_DEPTH_STENCIL
		);
	}
}

void RenderManager::ResizePostEffectTexture(const UINT& width, const UINT& height)
{
	//최초 생성    
	if (m_p_post_effect_render_target_texture == nullptr)
	{
		//현재 해상도에 맞게 Post Effect Target텍스처 생성
		m_p_post_effect_render_target_texture = RESOURCE_MANAGER->CreateTexture
		(
			"PostEffectTarget",
			width,
			height,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_SHADER_RESOURCE
		);
	}
	//현재 생성된 Post Effect Target Texture가 있다면
	else
	{
		if (m_p_post_effect_render_target_texture->GetWidth() == width && m_p_post_effect_render_target_texture->GetHeight() == height)
			return;

		//Post Effect Target Texture 해제
		m_p_post_effect_render_target_texture->ReleaseTexture();

		//새로 생성
		m_p_post_effect_render_target_texture->Create
		(
			width,
			height,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_SHADER_RESOURCE
		);
	}

	//Post Effect Material에 Texture 연결
	auto post_effect_material = RESOURCE_MANAGER->GetResource<Material>("Light2D_Material");
	post_effect_material->SetConstantBufferData(Material_Parameter::TEX_0, nullptr, m_p_post_effect_render_target_texture);
}

//Camera Component의 RenderPostEffectObjects에서 호출
void RenderManager::CopyPostEffect()
{
	//Render Target Texture의 이미지를 카피
	DEVICE_CONTEXT->CopyResource(m_p_post_effect_render_target_texture->GetTexture(), m_p_render_target_texture->GetTexture());
}