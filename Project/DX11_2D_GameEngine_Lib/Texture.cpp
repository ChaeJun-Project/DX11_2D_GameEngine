#include "stdafx.h"
#include "Texture.h"

Texture::Texture(const std::string resource_name)
	:IResource(ResourceType::Texture, resource_name)
{
}

const bool& Texture::LoadFromFile(const std::string& texture_path)
{
	//��ũ��ġ �̹���
	ScratchImage image;

	//��ũ��ġ �̹��� ����
	auto hResult = GetScratchImage(FileManager::ConvertStringToWString(texture_path), image);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return false;

	//�ؽ�ó ����
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

	//Shader Resource View ����ü ����
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory(&srv_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MostDetailedMip = 0;
	srv_desc.Texture2D.MipLevels = 1;

	//Texture2D ����ü ����
	D3D11_TEXTURE2D_DESC texture_desc;
	m_p_texture->GetDesc(&texture_desc);
	srv_desc.Format = texture_desc.Format;

	//�ؽ�ó ������ ����
	m_texture_size.x = static_cast<float>(texture_desc.Width); //�ʺ�
	m_texture_size.y = static_cast<float>(texture_desc.Height); //����

	//Shader Resource View ����
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

