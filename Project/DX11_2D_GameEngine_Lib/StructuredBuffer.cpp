#include "stdafx.h"
#include "StructuredBuffer.h"

void StructuredBuffer::Create(const UINT& element_size, const UINT& element_count, const SBufferType& sbuffer_type, const bool& is_cpu_access, void* p_data)
{
	m_element_size = element_size;				  //������ ���� ��Ҵ� ������
	m_element_count = element_count;			  //������ ���� ��� ����
	m_buffer_size = element_size * element_count; //������ ���� �� ũ��
	m_sbuffer_type = sbuffer_type;				  //������ ���� ���� ����

	//=============================================
	//������ ���� ����
	//=============================================
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = m_buffer_size;

	switch (m_sbuffer_type)
	{
	case SBufferType::Read_Only: //�б� ����
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; //SRV �ڿ� �並 ���� ������ ���ο� ����
		break;
	case SBufferType::Read_Write: //�а� ���� ����
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS; //SRV, UAV �ڿ� �並 ���� ������ ���ο� ����
		break;
	}

	desc.Usage = D3D11_USAGE_DEFAULT; //GPU�� ���ؼ� �а� ���� ���� CPU ���� �Ұ�
	desc.CPUAccessFlags = 0; //CPU ���� �Ұ�

	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = element_size; //������ ���� ��Ҵ� ũ��

	auto device = GRAPHICS_MANAGER->GetDevice();
	HRESULT hResult;

	//�ʱ� �����Ͱ� �ִٸ�
	if (p_data != nullptr)
	{
		D3D11_SUBRESOURCE_DATA sub_data;
		ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));

		sub_data.pSysMem = p_data;

		//�ʱ� �����ͷ� �ʱ�ȭ�� ���·� ������ ���� ����
		hResult = device->CreateBuffer(&desc, &sub_data, m_p_buffer.GetAddressOf());
		assert(SUCCEEDED(hResult));
		if (!SUCCEEDED(hResult))
			return;
	}
	//�ʱ� �����Ͱ� ���ٸ�
	else
	{
		//�ʱ� �����Ͱ� ���� ���·� ������ ���� ����
		hResult = device->CreateBuffer(&desc, nullptr, m_p_buffer.GetAddressOf());
		assert(SUCCEEDED(hResult));
		if (!SUCCEEDED(hResult))
			return;
	}

	//=============================================
	//Create Shader Resource View(GPU �б⸸ ������ ���)
	//=============================================
	CreateSRV();

	//=============================================
	//Create Unordered Access View(GPU �б�, ���� ��� ������ ���)
	//=============================================
	if (m_sbuffer_type == SBufferType::Read_Write)
	{
		CreateUAV();
	}

	//=============================================
	//CPU���� ���� ������ Structured Buffer ����
	//=============================================
	if (is_cpu_access)
	{
		//CPU Read
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.ByteWidth = m_buffer_size;

		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; //CPU �б⸸ ���

		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = m_element_size;

		//�ʱ� �����Ͱ� ���� ���·� ������ ���� ����
		hResult = device->CreateBuffer(&desc, nullptr, m_p_cpu_read_buffer.GetAddressOf());
		assert(SUCCEEDED(hResult));
		if (!SUCCEEDED(hResult))
			return;

		//CPU Write
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.ByteWidth = m_buffer_size;

		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		desc.Usage = D3D11_USAGE_DYNAMIC; //GPU�� ���ؼ� �б� ����, CPU���� ���� ����
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //CPU���� ���� ���

		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = m_element_size;

		//�ʱ� �����Ͱ� ���� ���·� ������ ���� ����
		hResult = device->CreateBuffer(&desc, nullptr, m_p_cpu_write_buffer.GetAddressOf());
		assert(SUCCEEDED(hResult));
	}
}

void StructuredBuffer::SetStructuredBufferData(const void* buffer_data, const UINT& buffer_size)
{
	//=============================================
	//System(CPU) -> m_p_cpu_write_buffer ������ ����
	//=============================================
	D3D11_MAPPED_SUBRESOURCE mapped_sub_data;
	ZeroMemory(&mapped_sub_data, sizeof(D3D11_MAPPED_SUBRESOURCE));

	auto device_context = GRAPHICS_MANAGER->GetDeviceContext();
	auto hResult = device_context->Map
	(
		m_p_cpu_write_buffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mapped_sub_data
	);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return;

	//�Ű������� ���� buffer_data�� MAPPED_SUBRESOURCE �����Ϳ� ���� 
	memcpy(mapped_sub_data.pData, buffer_data, static_cast<size_t>(buffer_size));

	device_context->Unmap(m_p_cpu_write_buffer.Get(), 0);

	//=============================================
	//m_p_cpu_write_buffer -> m_p_buffer ������ ����
	//=============================================
	device_context->CopyResource(m_p_buffer.Get(), m_p_cpu_write_buffer.Get());
}

void StructuredBuffer::GetStructuredBufferData(void* buffer_data, const UINT& buffer_size)
{
	//=============================================
	//m_p_buffer -> m_p_cpu_read_buffer ������ ����
	//=============================================
	auto device_context = GRAPHICS_MANAGER->GetDeviceContext();
	device_context->CopyResource(m_p_cpu_read_buffer.Get(), m_p_buffer.Get());

	//=============================================
	//m_p_cpu_read_buffer -> System(CPU) ������ ����
	//=============================================
	D3D11_MAPPED_SUBRESOURCE mapped_sub_data;
	ZeroMemory(&mapped_sub_data, sizeof(D3D11_MAPPED_SUBRESOURCE));

	auto hResult = device_context->Map
	(
		m_p_cpu_read_buffer.Get(),
		0,
		D3D11_MAP_READ,
		0,
		&mapped_sub_data
	);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return;

	//MAPPED_SUBRESOURCE �����͸� �Ű������� ���� buffer_data�� ����
	memcpy(buffer_data, mapped_sub_data.pData, static_cast<size_t>(buffer_size));

	device_context->Unmap(m_p_cpu_read_buffer.Get(), 0);
}

void StructuredBuffer::CreateSRV()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory(&srv_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	//������ ���۷� SRV�� ����ϴ� ��쿡 Format�� DXGI_FORMAT_UNKNOWN���� �����ؾ� ��
	//�̴� ������ ������ ���Ұ� ����� ���� ����ü�̹Ƿ� DXGI�� ��� ������ ������ �̸� �����ؼ� �δ� ���� �Ұ����ϱ� ����
	srv_desc.Format = DXGI_FORMAT_UNKNOWN;
	srv_desc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
	srv_desc.BufferEx.NumElements = m_element_count;

	auto device = GRAPHICS_MANAGER->GetDevice();
	auto hResult = device->CreateShaderResourceView(m_p_buffer.Get(), &srv_desc, m_p_shader_resource_view.GetAddressOf());
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return;
}

void StructuredBuffer::CreateUAV()
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
	ZeroMemory(&uav_desc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

	//������ ���۷� UAV�� ����ϴ� ��쿡 Format�� DXGI_FORMAT_UNKNOWN���� �����ؾ� ��
	//�̴� ������ ������ ���Ұ� ����� ���� ����ü�̹Ƿ� DXGI�� ��� ������ ������ �̸� �����ؼ� �δ� ���� �Ұ����ϱ� ����
	uav_desc.Format = DXGI_FORMAT_UNKNOWN;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uav_desc.Buffer.NumElements = m_element_count;

	auto device = GRAPHICS_MANAGER->GetDevice();
	auto hResult = device->CreateUnorderedAccessView(m_p_buffer.Get(), &uav_desc, m_p_unordered_access_view.GetAddressOf());
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return;
}

void StructuredBuffer::BindPipeline()
{
	auto device_context = GRAPHICS_MANAGER->GetDeviceContext();

	//Vertex Shader Stage
	if (m_buffer_bind_stage & PipelineStage::VS)
	{
		device_context->VSSetShaderResources(m_buffer_bind_slot, 1, m_p_shader_resource_view.GetAddressOf());
	}

	//Hull Shader Stage
	if (m_buffer_bind_stage & PipelineStage::HS)
	{
		device_context->HSSetShaderResources(m_buffer_bind_slot, 1, m_p_shader_resource_view.GetAddressOf());
	}

	//Domain Shader Stage
	if (m_buffer_bind_stage & PipelineStage::DS)
	{
		device_context->DSSetShaderResources(m_buffer_bind_slot, 1, m_p_shader_resource_view.GetAddressOf());
	}

	//Geometry Shader Stage
	if (m_buffer_bind_stage & PipelineStage::GS)
	{
		device_context->GSSetShaderResources(m_buffer_bind_slot, 1, m_p_shader_resource_view.GetAddressOf());
	}

	//Pixel Shader Stage
	if (m_buffer_bind_stage & PipelineStage::PS)
	{
		device_context->PSSetShaderResources(m_buffer_bind_slot, 1, m_p_shader_resource_view.GetAddressOf());
	}

	//Compute Shader Stage
	if (m_buffer_bind_stage & PipelineStage::CS)
	{
		device_context->PSSetShaderResources(m_buffer_bind_slot, 1, m_p_shader_resource_view.GetAddressOf());
	}
}

void StructuredBuffer::BindPipelineRW(const UINT& unordered_bind_slot)
{
	//u�������� ���ε��� �Ұ����� ����ȭ������ ���
	if (m_sbuffer_type == SBufferType::Read_Only)
	{
		return;
	}

	m_unordered_bind_slot = unordered_bind_slot;

	UINT i = -1;
	DEVICE_CONTEXT->CSSetUnorderedAccessViews(m_unordered_bind_slot, 1, m_p_unordered_access_view.GetAddressOf(), &i);
}

void StructuredBuffer::Clear()
{
	auto device_context = GRAPHICS_MANAGER->GetDeviceContext();

	//Clear Shader Resource View
	ID3D11ShaderResourceView* p_shader_resource_view = nullptr;

	//Vertex Shader Stage
	if (m_buffer_bind_stage & PipelineStage::VS)
	{
		device_context->VSSetShaderResources(m_buffer_bind_slot, 1, &p_shader_resource_view);
	}

	//Hull Shader Stage
	if (m_buffer_bind_stage & PipelineStage::HS)
	{
		device_context->HSSetShaderResources(m_buffer_bind_slot, 1, &p_shader_resource_view);
	}

	//Domain Shader Stage
	if (m_buffer_bind_stage & PipelineStage::DS)
	{
		device_context->DSSetShaderResources(m_buffer_bind_slot, 1, &p_shader_resource_view);
	}

	//Geometry Shader Stage
	if (m_buffer_bind_stage & PipelineStage::GS)
	{
		device_context->GSSetShaderResources(m_buffer_bind_slot, 1, &p_shader_resource_view);
	}

	//Pixel Shader Stage
	if (m_buffer_bind_stage & PipelineStage::PS)
	{
		device_context->PSSetShaderResources(m_buffer_bind_slot, 1, &p_shader_resource_view);
	}

	//Compute Shader Stage
	if (m_buffer_bind_stage & PipelineStage::CS)
	{
		device_context->PSSetShaderResources(m_buffer_bind_slot, 1, &p_shader_resource_view);
	}
}

void StructuredBuffer::ClearRW()
{
	//Clear Unordered Access View
	ID3D11UnorderedAccessView* p_unordered_access_view = nullptr;

	UINT i = -1;
	DEVICE_CONTEXT->CSSetUnorderedAccessViews(m_unordered_bind_slot, 1, &p_unordered_access_view, &i);
}
