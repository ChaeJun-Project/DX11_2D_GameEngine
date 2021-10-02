#include "stdafx.h"
#include "InputLayout.h"

const bool InputLayout::Create(ID3DBlob* blob)
{
	//blob�� ����� Shader ������ ���� ���
	if (blob == nullptr)
		return false;

	//blob�� ����� Shader ������ ����ų ������
	ComPtr<ID3D11ShaderReflection> reflector;

	//Shader ������ �����ϱ� ���� �������̽� reflection ����
	auto hResult = D3DReflect
	(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		__uuidof(ID3D11ShaderReflection),
		reinterpret_cast<void**>(reflector.GetAddressOf())
	);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return false;

	D3D11_SHADER_DESC shader_desc;
	//Shader ����ü ������ �޾ƿ�
	reflector->GetDesc(&shader_desc);

	//Shader�� ���� vertex �Ӽ��� �����ϴ� ����ü�� ���� �迭 ����(InputLayout)
	std::vector<D3D11_INPUT_ELEMENT_DESC> input_layout_element_descs;

	//Shader�� D3D11_INPUT_ELEMENT_DESC�� ��� ������ŭ ����
	for (UINT i = 0; i < shader_desc.InputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC shader_input_param_desc;
		//�ش� �ε����� Shader Input Layout ����ü ������ �޾ƿ�
		reflector->GetInputParameterDesc(i, &shader_input_param_desc);

		//D3D11_INPUT_ELEMENT_DESC : ������ Vertex�� �Ӽ��� �����ϴ� ����ü
		D3D11_INPUT_ELEMENT_DESC input_layout_desc;
		input_layout_desc.SemanticName = shader_input_param_desc.SemanticName;
		input_layout_desc.SemanticIndex = shader_input_param_desc.SemanticIndex;
		input_layout_desc.InputSlot = 0;
		input_layout_desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		input_layout_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		input_layout_desc.InstanceDataStepRate = 0;

		std::string semantic_name = input_layout_desc.SemanticName;
		//��ġ������ �׻� x, y, z �� 3������ �����Ǿ� �ֱ� ������ ����
		if (semantic_name.compare("POSITION") == 0)
			input_layout_desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

		//��ġ���� ���� ���� (color �� uv����)
	//��Ұ� float 1��
		else if (shader_input_param_desc.Mask == 1)
		{
			if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				input_layout_desc.Format = DXGI_FORMAT_R32_UINT;
			else if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				input_layout_desc.Format = DXGI_FORMAT_R32_SINT;
			else if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				input_layout_desc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		//��Ұ� float 2��(Vector2)
		else if (shader_input_param_desc.Mask <= 3)
		{
			if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				input_layout_desc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				input_layout_desc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				input_layout_desc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		//��Ұ� float 3��(Vector3)
		else if (shader_input_param_desc.Mask <= 7)
		{
			if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				input_layout_desc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				input_layout_desc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				input_layout_desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		//��Ұ� float 4��(Vector4)
		else if (shader_input_param_desc.Mask <= 15)
		{
			if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				input_layout_desc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				input_layout_desc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				input_layout_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}


		input_layout_element_descs.emplace_back(input_layout_desc);
	}

	//Create Input Layout
	auto device = GraphicsManager::GetInstance()->GetDevice();
    hResult = device->CreateInputLayout
	(
		input_layout_element_descs.data(),
		static_cast<UINT>(input_layout_element_descs.size()),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		m_p_input_layout.GetAddressOf()
	);
	assert(SUCCEEDED(hResult));
	if (!SUCCEEDED(hResult))
		return false;

	return true;
}
