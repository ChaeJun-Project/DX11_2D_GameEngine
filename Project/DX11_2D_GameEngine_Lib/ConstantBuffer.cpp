#include "stdafx.h"
#include "ConstantBuffer.h"

void ConstantBuffer::Unmap()
{
	auto device_context = GraphicsManager::GetInstance()->GetDeviceContext();
	device_context->Unmap(m_p_buffer.Get(), 0);
}
