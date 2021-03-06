#include "stdafx.h"
#include "ConstantBuffer.h"

void ConstantBuffer::SetConstantBufferData(const void* buffer_data, const UINT& buffer_size)
{
    //생성 시 정해진 버퍼 사이즈보다 입력할 버퍼 사이즈가 큰 경우
    auto result = m_buffer_size >= buffer_size? true : false;
	assert(result);
	if (!result)
		return;

	D3D11_MAPPED_SUBRESOURCE mapped_sub_data;
	ZeroMemory(&mapped_sub_data, sizeof(D3D11_MAPPED_SUBRESOURCE));

	auto device_context = GRAPHICS_MANAGER->GetDeviceContext();
	auto hResult = device_context->Map
	(
		m_p_buffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mapped_sub_data
	);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return;
    
	//GPU에 보낼 데이터를 MAPPED_SUBRESOURCE에 복사
	memcpy(mapped_sub_data.pData, buffer_data, static_cast<size_t>(buffer_size));

	device_context->Unmap(m_p_buffer.Get(), 0);
}

void ConstantBuffer::BindPipeline()
{
	auto device_context = GRAPHICS_MANAGER->GetDeviceContext();
	if (m_buffer_bind_stage & PipelineStage::VS)
	{
		device_context->VSSetConstantBuffers(m_buffer_bind_slot, 1, m_p_buffer.GetAddressOf()); // b0 레지스터에 상수버퍼 바인딩(vertex shader 실행 시)	
	}

	if (m_buffer_bind_stage & PipelineStage::HS)
	{
		device_context->HSSetConstantBuffers(m_buffer_bind_slot, 1, m_p_buffer.GetAddressOf());
	}

	if (m_buffer_bind_stage & PipelineStage::DS)
	{
		device_context->DSSetConstantBuffers(m_buffer_bind_slot, 1, m_p_buffer.GetAddressOf());
	}

	if (m_buffer_bind_stage & PipelineStage::GS)
	{
		device_context->GSSetConstantBuffers(m_buffer_bind_slot, 1, m_p_buffer.GetAddressOf());
	}

	if (m_buffer_bind_stage & PipelineStage::PS)
	{
		device_context->PSSetConstantBuffers(m_buffer_bind_slot, 1, m_p_buffer.GetAddressOf());
	}

	if (m_buffer_bind_stage & PipelineStage::CS)
	{
		device_context->CSSetConstantBuffers(m_buffer_bind_slot, 1, m_p_buffer.GetAddressOf());
	}
}
