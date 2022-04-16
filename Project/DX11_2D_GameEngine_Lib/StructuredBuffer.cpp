#include "stdafx.h"
#include "StructuredBuffer.h"

void StructuredBuffer::SetStructuredBufferData(const void* buffer_data, const UINT& buffer_size)
{
	//=============================================
	//System(CPU) -> m_p_cpu_write_buffer 데이터 전달
	//=============================================
	D3D11_MAPPED_SUBRESOURCE mapped_sub_data;
	ZeroMemory(&mapped_sub_data, sizeof(D3D11_MAPPED_SUBRESOURCE));

	auto device_context = DEVICE_CONTEXT;
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

	//매개변수로 들어온 buffer_data를 MAPPED_SUBRESOURCE 데이터에 복사 
	memcpy(mapped_sub_data.pData, buffer_data, static_cast<size_t>(buffer_size));

	device_context->Unmap(m_p_cpu_write_buffer.Get(), 0);

	//=============================================
	//m_p_cpu_write_buffer -> m_p_buffer 데이터 전달
	//=============================================
	device_context->CopyResource(m_p_buffer.Get(), m_p_cpu_write_buffer.Get());
}

void StructuredBuffer::GetStructuredBufferData(void* buffer_data, const UINT& buffer_size)
{
	//=============================================
	//m_p_buffer -> m_p_cpu_read_buffer 데이터 전달
	//=============================================
	auto device_context = DEVICE_CONTEXT;
	device_context->CopyResource(m_p_cpu_read_buffer.Get(), m_p_buffer.Get());

	//=============================================
	//m_p_cpu_read_buffer -> System(CPU) 데이터 전달
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

	//MAPPED_SUBRESOURCE 데이터를 매개변수로 들어온 buffer_data에 복사
	memcpy(buffer_data, mapped_sub_data.pData, static_cast<size_t>(buffer_size));

	device_context->Unmap(m_p_cpu_read_buffer.Get(), 0);
}

void StructuredBuffer::CreateSRV()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory(&srv_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	//구조적 버퍼로 SRV를 사용하는 경우에 Format은 DXGI_FORMAT_UNKNOWN으로 설정해야 함
	//이는 구조적 버퍼의 원소가 사용자 정의 구조체이므로 DXGI가 모든 가능한 형식을 미리 정희해서 두는 것이 불가능하기 때문
	srv_desc.Format = DXGI_FORMAT_UNKNOWN;
	srv_desc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
	srv_desc.BufferEx.NumElements = m_element_count;

	auto hResult = DEVICE->CreateShaderResourceView(m_p_buffer.Get(), &srv_desc, m_p_shader_resource_view.GetAddressOf());
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return;
}

void StructuredBuffer::CreateUAV()
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
	ZeroMemory(&uav_desc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

	//구조적 버퍼로 UAV를 사용하는 경우에 Format은 DXGI_FORMAT_UNKNOWN으로 설정해야 함
	//이는 구조적 버퍼의 원소가 사용자 정의 구조체이므로 DXGI가 모든 가능한 형식을 미리 정희해서 두는 것이 불가능하기 때문
	uav_desc.Format = DXGI_FORMAT_UNKNOWN;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uav_desc.Buffer.NumElements = m_element_count;

	auto hResult = DEVICE->CreateUnorderedAccessView(m_p_buffer.Get(), &uav_desc, m_p_unordered_access_view.GetAddressOf());
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return;
}

void StructuredBuffer::BindPipeline()
{
	auto device_context = DEVICE_CONTEXT;

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
	//u레지스터 바인딩이 불가능한 구조화버퍼인 경우
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
	auto device_context = DEVICE_CONTEXT;

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
