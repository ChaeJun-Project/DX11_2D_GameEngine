#include "stdafx.h"
#include "Texture.h"

#include "GraphicsManager.h"

Texture::Texture(const std::string resource_name)
	:IResource(ResourceType::Texture, resource_name)
{
	ZeroMemory(&m_texture_desc, sizeof(D3D11_TEXTURE2D_DESC));
}

void Texture::LoadFromFile(const std::string& texture_path)
{
	//스크래치 이미지
	ScratchImage image;

	//스크래치 이미지 생성
	auto hResult = GetScratchImage(FileManager::ConvertStringToWString(texture_path), image);
	assert(SUCCEEDED(hResult));
	image.GetPixels();
	//텍스처 생성
	auto device = GraphicsManager::GetInstance()->GetDevice();
	hResult = CreateTexture
	(
		device,
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		(ID3D11Resource**)m_p_texture.GetAddressOf()
	);
	assert(SUCCEEDED(hResult));

	//Shader Resource View 구조체 정의
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory(&srv_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MostDetailedMip = 0;
	srv_desc.Texture2D.MipLevels = 1;
	
	//Texture 구조체 정의
	m_p_texture->GetDesc(&m_texture_desc);
	srv_desc.Format = m_texture_desc.Format;
	
	//Shader Resource View 생성
	hResult = device->CreateShaderResourceView
	(
		m_p_texture.Get(),
		&srv_desc,
		m_p_shader_resource_view.GetAddressOf()
	);
	assert(SUCCEEDED(hResult));
}


const HRESULT& Texture::GetScratchImage(const std::wstring& texture_path, ScratchImage& image)
{
	//해당 텍스처의 경로를 받아서 확장자를 받아옴(.png, .jpg 등)
	std::wstring extension = FileManager::GetExtensionFromPath(texture_path);

	//.dds
	if (extension._Equal(L".DDS") || extension._Equal(L".dds"))
	{
		return LoadFromDDSFile(texture_path.c_str(), DDS_FLAGS_NONE, nullptr, image);
	}

	//.tga
	else if (extension._Equal(L".TGA") || extension._Equal(L".tga"))
	{
		return LoadFromTGAFile(texture_path.c_str(), nullptr, image);
	}

	//그 밖의 다른 확장자들(.png, .jpg 등)
	else
	{
		return LoadFromWICFile(texture_path.c_str(), WIC_FLAGS_NONE, nullptr, image);
	}

	return S_FALSE;
}

void Texture::SaveFile(const std::string& texture_path)
{
}

void Texture::Create(const UINT& width, const UINT& height, const DXGI_FORMAT& texture_format, const UINT& bind_flage)
{
	m_texture_desc.Width = width;
	m_texture_desc.Height = height;

	m_texture_desc.MipLevels = 1;
	m_texture_desc.ArraySize = 1;

	//Depth Stencil View를 생성하는 경우
	//3바이트는 Depth 값, 1바이트는 Stencil 값 저장
	m_texture_desc.Format = texture_format;
	m_texture_desc.SampleDesc.Count = 1;
	m_texture_desc.SampleDesc.Quality = 0;

	m_texture_desc.Usage = D3D11_USAGE_DEFAULT;
	m_texture_desc.BindFlags = bind_flage;

	//Create Texture
	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateTexture2D
	(
		&m_texture_desc,
		0,
		m_p_texture.GetAddressOf()
	);
	assert(SUCCEEDED(hResult));

	//View Resource 생성
	//Depth Stencil View 생성
	if (D3D11_BIND_DEPTH_STENCIL & m_texture_desc.BindFlags)
	{
		hResult = device->CreateDepthStencilView
		(
			m_p_texture.Get(),
			nullptr,
			m_p_depth_stencil_view.GetAddressOf()
		);
		assert(SUCCEEDED(hResult));
	}

	else
	{
		//Render Target View 생성
		if (D3D11_BIND_RENDER_TARGET & m_texture_desc.BindFlags)
		{
			hResult = device->CreateRenderTargetView
			(
				m_p_texture.Get(),
				nullptr,
				m_p_render_target_view.GetAddressOf()
			);
			assert(SUCCEEDED(hResult));
		}

		//Shader Resource View 생성
		if (D3D11_BIND_SHADER_RESOURCE & m_texture_desc.BindFlags)
		{
			hResult = device->CreateShaderResourceView
			(
				m_p_texture.Get(),
				nullptr,
				m_p_shader_resource_view.GetAddressOf()
			);
			assert(SUCCEEDED(hResult));
		}

		//Unordered Access View 생성
		if (D3D11_BIND_UNORDERED_ACCESS & m_texture_desc.BindFlags)
		{
			hResult = device->CreateUnorderedAccessView
			(
				m_p_texture.Get(),
				nullptr,
				m_p_unordered_access_view.GetAddressOf()
			);
			assert(SUCCEEDED(hResult));
		}
	}
}

void Texture::Create(const ComPtr<ID3D11Texture2D>& texture2D)
{
	m_p_texture = texture2D;
	m_p_texture->GetDesc(&m_texture_desc);

	auto device = GraphicsManager::GetInstance()->GetDevice();
	HRESULT hResult;

	//View Resource 생성
	//Depth Stencil View 생성
	if (D3D11_BIND_DEPTH_STENCIL & m_texture_desc.BindFlags)
	{
		hResult = device->CreateDepthStencilView
		(
			m_p_texture.Get(),
			nullptr,
			m_p_depth_stencil_view.GetAddressOf()
		);
		assert(SUCCEEDED(hResult));
	}

	else
	{
		//Render Target View 생성
		if (D3D11_BIND_RENDER_TARGET & m_texture_desc.BindFlags)
		{
			hResult = device->CreateRenderTargetView
			(
				m_p_texture.Get(),
				nullptr,
				m_p_render_target_view.GetAddressOf()
			);
			assert(SUCCEEDED(hResult));
		}

		//Shader Resource View 생성
		if (D3D11_BIND_SHADER_RESOURCE & m_texture_desc.BindFlags)
		{
			hResult = device->CreateShaderResourceView
			(
				m_p_texture.Get(),
				nullptr,
				m_p_shader_resource_view.GetAddressOf()
			);
			assert(SUCCEEDED(hResult));
		}

		//Unordered Access View 생성
		if (D3D11_BIND_UNORDERED_ACCESS & m_texture_desc.BindFlags)
		{
			hResult = device->CreateUnorderedAccessView
			(
				m_p_texture.Get(),
				nullptr,
				m_p_unordered_access_view.GetAddressOf()
			);
			assert(SUCCEEDED(hResult));
		}
	}
}

void Texture::BindPipeline()
{
	auto device_context = GraphicsManager::GetInstance()->GetDeviceContext();

	//Vertex Shader Stage
	if (m_texture_bind_stage & PipelineStage::VS)
	{
		device_context->VSSetShaderResources(m_texture_bind_slot, 1, m_p_shader_resource_view.GetAddressOf());
	}

	//Hull Shader Stage
	if (m_texture_bind_stage & PipelineStage::HS)
	{
		device_context->HSSetShaderResources(m_texture_bind_slot, 1, m_p_shader_resource_view.GetAddressOf());
	}

	//Domain Shader Stage
	if (m_texture_bind_stage & PipelineStage::DS)
	{
		device_context->DSSetShaderResources(m_texture_bind_slot, 1, m_p_shader_resource_view.GetAddressOf());
	}

	//Geometry Shader Stage
	if (m_texture_bind_stage & PipelineStage::GS)
	{
		device_context->GSSetShaderResources(m_texture_bind_slot, 1, m_p_shader_resource_view.GetAddressOf());
	}

	//Pixel Shader Stage
	if (m_texture_bind_stage & PipelineStage::PS)
	{
		device_context->PSSetShaderResources(m_texture_bind_slot, 1, m_p_shader_resource_view.GetAddressOf());
	}

	//Compute Shader Stage
	if (m_texture_bind_stage & PipelineStage::CS)
	{
		device_context->CSSetShaderResources(m_texture_bind_slot, 1, m_p_shader_resource_view.GetAddressOf());
	}
}

void Texture::BindPipelineRW(const UINT& unordered_bind_slot)
{
	m_unordered_bind_slot = unordered_bind_slot;

	auto device_context = GraphicsManager::GetInstance()->GetDeviceContext();

	//Compute Shader Stage
	ID3D11UnorderedAccessView* p_unordered_access_view = nullptr;
	UINT i = -1;
	device_context->CSSetUnorderedAccessViews(m_unordered_bind_slot, 1, &p_unordered_access_view, &i);
}

void Texture::Clear()
{
	auto device_context = GraphicsManager::GetInstance()->GetDeviceContext();

	//Clear Shader Resource View
	ID3D11ShaderResourceView* p_shader_resource_view = nullptr;
	//Vertex Shader Stage
	if (m_texture_bind_stage & PipelineStage::VS)
	{
		device_context->VSSetShaderResources(m_texture_bind_slot, 1, &p_shader_resource_view);
	}

	//Hull Shader Stage
	if (m_texture_bind_stage & PipelineStage::HS)
	{
		device_context->HSSetShaderResources(m_texture_bind_slot, 1, &p_shader_resource_view);
	}

	//Domain Shader Stage
	if (m_texture_bind_stage & PipelineStage::DS)
	{
		device_context->DSSetShaderResources(m_texture_bind_slot, 1, &p_shader_resource_view);
	}

	//Geometry Shader Stage
	if (m_texture_bind_stage & PipelineStage::GS)
	{
		device_context->GSSetShaderResources(m_texture_bind_slot, 1, &p_shader_resource_view);
	}

	//Pixel Shader Stage
	if (m_texture_bind_stage & PipelineStage::PS)
	{
		device_context->PSSetShaderResources(m_texture_bind_slot, 1, &p_shader_resource_view);
	}

	//Compute Shader Stage
	if (m_texture_bind_stage & PipelineStage::CS)
	{
		device_context->PSSetShaderResources(m_texture_bind_slot, 1, &p_shader_resource_view);
	}

	//Clear Unordered Access View
	ID3D11UnorderedAccessView* p_unordered_access_view = nullptr;
	UINT i = -1;
	device_context->CSSetUnorderedAccessViews(m_unordered_bind_slot, 1, &p_unordered_access_view, &i);
}

void Texture::ReleaseTexture()
{
	m_p_texture.Reset();
}

void Texture::ReleaseRenderTargetView()
{
	m_p_texture.Reset();
	m_p_render_target_view.Reset();
}

void Texture::ReleaseDepthStencilView()
{
	m_p_texture.Reset();
	m_p_depth_stencil_view.Reset();
}

