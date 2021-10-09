#include "stdafx.h"
#include "ConstantBuffer.h"

void ConstantBuffer::SetConstantBufferData(const void* buffer_data, const UINT& buffer_size)
{
    //���� �� ������ ���� ������� �Է��� ���� ����� ū ���
    auto result = this->m_buffer_size >= buffer_size? true : false;
	assert(result);
	if (!result)
		return;

	D3D11_MAPPED_SUBRESOURCE mapped_sub_data;
	ZeroMemory(&mapped_sub_data, sizeof(D3D11_MAPPED_SUBRESOURCE));

	auto device_context = GraphicsManager::GetInstance()->GetDeviceContext();
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
    
	//GPU�� ���� �����͸� MAPPED_SUBRESOURCE�� ����
	memcpy(mapped_sub_data.pData, buffer_data, static_cast<size_t>(buffer_size));

	device_context->Unmap(m_p_buffer.Get(), 0);
}

void ConstantBuffer::BindPipeline()
{
	auto device_context = GraphicsManager::GetInstance()->GetDeviceContext();
	if (this->m_buffer_bind_stage & PipelineStage::VS)
	{
		device_context->VSSetConstantBuffers(this->m_buffer_bind_slot, 1, this->m_p_buffer.GetAddressOf()); // b0 �������Ϳ� ������� ���ε�(vertex shader ���� ��)	
	}

	if (this->m_buffer_bind_stage & PipelineStage::HS)
	{
		device_context->HSSetConstantBuffers(this->m_buffer_bind_slot, 1, this->m_p_buffer.GetAddressOf());
	}

	if (this->m_buffer_bind_stage & PipelineStage::DS)
	{
		device_context->DSSetConstantBuffers(this->m_buffer_bind_slot, 1, this->m_p_buffer.GetAddressOf());
	}

	if (this->m_buffer_bind_stage & PipelineStage::GS)
	{
		device_context->GSSetConstantBuffers(this->m_buffer_bind_slot, 1, this->m_p_buffer.GetAddressOf());
	}

	if (this->m_buffer_bind_stage & PipelineStage::PS)
	{
		device_context->PSSetConstantBuffers(this->m_buffer_bind_slot, 1, this->m_p_buffer.GetAddressOf());
	}
}
