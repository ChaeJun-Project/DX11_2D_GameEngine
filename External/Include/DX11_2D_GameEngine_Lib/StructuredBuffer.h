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
		const SBufferType& structured_buffer_type,
		const bool& is_cpu_access,
		void* p_data = nullptr
	);

	void SetStructuredBufferData(const void* buffer_data, const UINT& buffer_size);
	void GetStructuredBufferData(void* buffer_data, const UINT& buffer_size);

	void BindPipeline();
	void BindPipelineRW(const UINT& unordered_bind_slot);

	void Clear();
	void ClearRW();

private:
	void CreateSRV();
	void CreateUAV();

public:
	UINT GetBufferBindSlot() const { return m_buffer_bind_slot; }
	UINT GetBufferBindStage() const { return m_buffer_bind_stage; }

	void SetBufferBindStage(const UINT& buffer_bind_stage) { m_buffer_bind_stage = buffer_bind_stage; }
	void SetBufferBindSlot(const UINT& buffer_bind_slot) { m_buffer_bind_slot = buffer_bind_slot; }

	UINT GetElementCount() const { return m_element_count; }

private:
	//=============================================
	//GPU Read_Only or Read_Write StructuredBuffer
	//=============================================
	ComPtr<ID3D11Buffer> m_p_buffer = nullptr;

	ComPtr<ID3D11ShaderResourceView> m_p_shader_resource_view = nullptr; //Read_Only
	ComPtr<ID3D11UnorderedAccessView> m_p_unordered_access_view = nullptr; //Read_Write

	SBufferType m_sbuffer_type;

	//=============================================
    //CPU StructuredBuffer
	//=============================================
	ComPtr<ID3D11Buffer> m_p_cpu_read_buffer = nullptr;
	ComPtr<ID3D11Buffer> m_p_cpu_write_buffer = nullptr;
	bool is_cpu_access = false;

	UINT m_element_size = 0;
	UINT m_element_count = 0;
	UINT m_buffer_size = 0;	  //m_element_size * m_element_count

	UINT m_buffer_bind_stage = 0;   //Bind Stage
	UINT m_buffer_bind_slot = 0;	//t(texture) register 바인드 슬롯 번호
	UINT m_unordered_bind_slot = 0; //u(unordered) register 바인드 슬롯 번호
};

