#pragma once

class StructuredBuffer final
{
public:
	StructuredBuffer() = default;
	~StructuredBuffer() = default;

	void Create
	(
		const UINT& element_size,
		const UINT& element_count,
		const SBufferType& sbuffer_type,
		void* p_data = nullptr
	);

	void BindPipeline();
	void BindPipelineRW(const UINT& unordered_bind_slot);

	void Clear();

private:
    void CreateSRV();
	void CreateUAV();

public:
	const UINT& GetBufferBindSlot() { return m_buffer_bind_slot; }
	const UINT& GetBufferBindStage() { return m_buffer_bind_stage; }

	void SetBufferBindStage(const UINT& buffer_bind_stage) { m_buffer_bind_stage = buffer_bind_stage; }
	void SetBufferBindSlot(const UINT& buffer_bind_slot) { m_buffer_bind_slot = buffer_bind_slot; }

private:
	ComPtr<ID3D11Buffer> m_p_buffer = nullptr;

	ComPtr<ID3D11ShaderResourceView> m_p_shader_resource_view = nullptr;
	ComPtr<ID3D11UnorderedAccessView> m_p_unordered_access_view = nullptr;

	SBufferType m_sbuffer_type;
	UINT m_buffer_bind_stage = 0;
	UINT m_buffer_bind_slot = 0; //Register Num
	UINT m_unordered_bind_slot = 0; //u register 바인드 슬롯 번호

	UINT m_element_size = 0;
	UINT m_element_count = 0;
	UINT m_buffer_size = 0; //m_element_size * m_element_count
};

