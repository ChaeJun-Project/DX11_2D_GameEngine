#include "stdafx.h"
#include "Texture.h"

Texture::Texture(const std::string resource_name)
	:IResource(ResourceType::Texture, resource_name)
{
}

const bool& Texture::LoadFromFile(const std::string& texture_path)
{
	//스크래치 이미지
	ScratchImage image;

	//스크래치 이미지 생성
	auto hResult = GetScratchImage(FileManager::ConvertStringToWString(texture_path), image);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return false;

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
	if (!SUCCEEDED(hResult))
		return false;

	//Shader Resource View 구조체 정의
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory(&srv_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MostDetailedMip = 0;
	srv_desc.Texture2D.MipLevels = 1;

	//Texture2D 구조체 정의
	D3D11_TEXTURE2D_DESC texture_desc;
	m_p_texture->GetDesc(&texture_desc);
	srv_desc.Format = texture_desc.Format;

	//텍스처 사이즈 설정
	m_texture_size.x = static_cast<float>(texture_desc.Width); //너비
	m_texture_size.y = static_cast<float>(texture_desc.Height); //높이

	//Shader Resource View 생성
	hResult = device->CreateShaderResourceView
	(
		m_p_texture.Get(),
		&srv_desc,
		m_p_shader_resource_view.GetAddressOf()
	);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return false;

	return true;
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

