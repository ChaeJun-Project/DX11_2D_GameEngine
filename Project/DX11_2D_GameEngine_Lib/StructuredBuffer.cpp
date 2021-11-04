#include "stdafx.h"
#include "StructuredBuffer.h"

void StructuredBuffer::Create(const UINT& element_size, const UINT& element_count, const SBufferType& sbuffer_type, void* p_data)
{
	m_element_size = element_size;
	m_element_count = element_count;
	m_buffer_size = element_size * element_count;
	m_sbuffer_type = sbuffer_type;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = m_buffer_size;

	switch (m_sbuffer_type)
	{
	case SBufferType::Read: //읽기 전용
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		break;
	case SBufferType::Read_Write: //읽고 쓰기 전용
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		break;
	case SBufferType::Cpu_Access:
		break;
	}

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;

	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = element_size;

	auto device = GraphicsManager::GetInstance()->GetDevice();
	HRESULT hResult;

	//초기 데이터가 있다면
	if (p_data != nullptr)
	{
		D3D11_SUBRESOURCE_DATA sub_data;
		ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));

		sub_data.pSysMem = p_data;

		hResult = device->CreateBuffer(&desc, &sub_data, m_p_buffer.GetAddressOf());
		assert(SUCCEEDED(hResult));
		if (!SUCCEEDED(hResult))
			return;
	}

	//초기 데이터가 없다면
	else
	{
		hResult = device->CreateBuffer(&desc, nullptr, m_p_buffer.GetAddressOf());
		assert(SUCCEEDED(hResult));
		if (!SUCCEEDED(hResult))
			return;
	}

	//Create Shader Resource View
	CreateSRV();

	//Create Unordered Access
	if (m_sbuffer_type == SBufferType::Read_Write || m_sbuffer_type == SBufferType::Cpu_Access)
	{
		CreateUAV();
	}
}

void StructuredBuffer::CreateSRV()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory(&srv_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	srv_desc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
	srv_desc.BufferEx.NumElements = m_element_count;

	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateShaderResourceView(m_p_buffer.Get(), &srv_desc, m_p_shader_resource_view.GetAddressOf());
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return;
}

void StructuredBuffer::CreateUAV()
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
	ZeroMemory(&uav_desc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uav_desc.Buffer.NumElements = m_element_count;
	
	auto device = GraphicsManager::GetInstance()->GetDevice();
	auto hResult = device->CreateUnorderedAccessView(m_p_buffer.Get(), &uav_desc, m_p_unordered_access_view.GetAddressOf());
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return;
}

void StructuredBuffer::BindPipeline()
{
	auto device_context = GraphicsManager::GetInstance()->GetDeviceContext();

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
	m_unordered_bind_slot = unordered_bind_slot;

	//u레지스터 바인딩이 불가능한 구조화버퍼인 경우
	if (m_sbuffer_type == SBufferType::Read)
	{
		return;
	}

	auto device_context = GraphicsManager::GetInstance()->GetDeviceContext();
	UINT i = -1;
	device_context->CSSetUnorderedAccessViews(m_unordered_bind_slot, 1, m_p_unordered_access_view.GetAddressOf(), &i);
}

void StructuredBuffer::Clear()
{
	auto device_context = GraphicsManager::GetInstance()->GetDeviceContext();

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

	//Clear Unordered Access View
	ID3D11UnorderedAccessView* p_unordered_access_view = nullptr;

	UINT i = -1;
	device_context->CSSetUnorderedAccessViews(m_unordered_bind_slot, 1, &p_unordered_access_view, &i);
}
