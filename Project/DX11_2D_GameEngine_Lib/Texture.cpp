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
	//��ũ��ġ �̹���
	ScratchImage image;

	//��ũ��ġ �̹��� ����
	auto hResult = GetScratchImage(FileManager::ConvertStringToWString(texture_path), image);
	assert(SUCCEEDED(hResult));

	//�ؽ�ó ����
	auto device = GraphicsManager::GetInstance()->GetDevice();
	hResult = CreateTexture
	(
		device,
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		(ID3D11Resource**)m_p_texture2D.GetAddressOf()
	);
	assert(SUCCEEDED(hResult));

	//Shader Resource View ����ü ����
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory(&srv_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MostDetailedMip = 0;
	srv_desc.Texture2D.MipLevels = 1;

	//Texture2D ����ü ����
	m_p_texture2D->GetDesc(&m_texture_desc);
	srv_desc.Format = m_texture_desc.Format;

	//Shader Resource View ����
	hResult = device->CreateShaderResourceView
	(
		m_p_texture2D.Get(),
		&srv_desc,
		m_p_shader_resource_view.GetAddressOf()
	);
	assert(SUCCEEDED(hResult));
}


const HRESULT& Texture::GetScratchImage(const std::wstring& texture_path, ScratchImage& image)
{
	//�ش� �ؽ�ó�� ��θ� �޾Ƽ� Ȯ���ڸ� �޾ƿ�(.png, .jpg ��)
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

	//�� ���� �ٸ� Ȯ���ڵ�(.png, .jpg ��)
	else
	{
		return LoadFromWICFile(texture_path.c_str(), WIC_FLAGS_NONE, nullptr, image);
	}

	return S_FALSE;
}

void Texture::SaveFile(const std::string& texture_path)
{
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
}

void Texture::Create(const UINT& width, const UINT& height, const DXGI_FORMAT& texture_format, const UINT& bind_flage)
{
	m_texture_desc.Width = width;
	m_texture_desc.Height = height;

	m_texture_desc.MipLevels = 1;
	m_texture_desc.ArraySize = 1;

	//Depth Stencil View�� �����ϴ� ���
	//3����Ʈ�� Depth ��, 1����Ʈ�� Stencil �� ����
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
		m_p_texture2D.GetAddressOf()
	);
	assert(SUCCEEDED(hResult));

	//View Resource ����
	//Depth Stencil View ����
	if (D3D11_BIND_DEPTH_STENCIL & m_texture_desc.BindFlags)
	{
		hResult = device->CreateDepthStencilView
		(
			m_p_texture2D.Get(),
			nullptr,
			m_p_depth_stencil_view.GetAddressOf()
		);
		assert(SUCCEEDED(hResult));
	}

	else
	{
		//Render Target View ����
		if (D3D11_BIND_RENDER_TARGET & m_texture_desc.BindFlags)
		{
			hResult = device->CreateRenderTargetView
			(
				m_p_texture2D.Get(),
				nullptr,
				m_p_render_target_view.GetAddressOf()
			);
			assert(SUCCEEDED(hResult));
		}

		//Shader Resource View ����
		if (D3D11_BIND_SHADER_RESOURCE & m_texture_desc.BindFlags)
		{
			hResult = device->CreateShaderResourceView
			(
				m_p_texture2D.Get(),
				nullptr,
				m_p_shader_resource_view.GetAddressOf()
			);
			assert(SUCCEEDED(hResult));
		}

		//Unordered Access View ����
		if (D3D11_BIND_UNORDERED_ACCESS & m_texture_desc.BindFlags)
		{
			hResult = device->CreateUnorderedAccessView
			(
				m_p_texture2D.Get(),
				nullptr,
				m_p_unordered_access_view.GetAddressOf()
			);
			assert(SUCCEEDED(hResult));
		}
	}
}

void Texture::Create(const ComPtr<ID3D11Texture2D>& texture2D)
{
	m_p_texture2D = texture2D;
	m_p_texture2D->GetDesc(&m_texture_desc);

	auto device = GraphicsManager::GetInstance()->GetDevice();
	HRESULT hResult;

	//View Resource ����
	//Depth Stencil View ����
	if (D3D11_BIND_DEPTH_STENCIL & m_texture_desc.BindFlags)
	{
		hResult = device->CreateDepthStencilView
		(
			m_p_texture2D.Get(),
			nullptr,
			m_p_depth_stencil_view.GetAddressOf()
		);
		assert(SUCCEEDED(hResult));
	}

	else
	{
		//Render Target View ����
		if (D3D11_BIND_RENDER_TARGET & m_texture_desc.BindFlags)
		{
			hResult = device->CreateRenderTargetView
			(
				m_p_texture2D.Get(),
				nullptr,
				m_p_render_target_view.GetAddressOf()
			);
			assert(SUCCEEDED(hResult));
		}

		//Shader Resource View ����
		if (D3D11_BIND_SHADER_RESOURCE & m_texture_desc.BindFlags)
		{
			hResult = device->CreateShaderResourceView
			(
				m_p_texture2D.Get(),
				nullptr,
				m_p_shader_resource_view.GetAddressOf()
			);
			assert(SUCCEEDED(hResult));
		}

		//Unordered Access View ����
		if (D3D11_BIND_UNORDERED_ACCESS & m_texture_desc.BindFlags)
		{
			hResult = device->CreateUnorderedAccessView
			(
				m_p_texture2D.Get(),
				nullptr,
				m_p_unordered_access_view.GetAddressOf()
			);
			assert(SUCCEEDED(hResult));
		}
	}
}