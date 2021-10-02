#include "stdafx.h"
#include "InputLayout.h"

const bool InputLayout::Create(ID3DBlob* blob)
{
	//blob에 저장된 Shader 정보가 없는 경우
	if (blob == nullptr)
		return false;

	//blob에 저장된 Shader 정보를 가리킬 포인터
	ComPtr<ID3D11ShaderReflection> reflector;

	//Shader 정보에 접근하기 위한 인터페이스 reflection 생성
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
	//Shader 구조체 정보를 받아옴
	reflector->GetDesc(&shader_desc);

	//Shader의 여러 vertex 속성을 설명하는 구조체를 담을 배열 생성(InputLayout)
	std::vector<D3D11_INPUT_ELEMENT_DESC> input_layout_element_descs;

	//Shader의 D3D11_INPUT_ELEMENT_DESC의 요소 개수만큼 실행
	for (UINT i = 0; i < shader_desc.InputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC shader_input_param_desc;
		//해당 인덱스의 Shader Input Layout 구조체 정보를 받아옴
		reflector->GetInputParameterDesc(i, &shader_input_param_desc);

		//D3D11_INPUT_ELEMENT_DESC : 각각의 Vertex의 속성을 설명하는 구조체
		D3D11_INPUT_ELEMENT_DESC input_layout_desc;
		input_layout_desc.SemanticName = shader_input_param_desc.SemanticName;
		input_layout_desc.SemanticIndex = shader_input_param_desc.SemanticIndex;
		input_layout_desc.InputSlot = 0;
		input_layout_desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		input_layout_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		input_layout_desc.InstanceDataStepRate = 0;

		std::string semantic_name = input_layout_desc.SemanticName;
		//위치정보는 항상 x, y, z 총 3가지로 구성되어 있기 때문에 고정
		if (semantic_name.compare("POSITION") == 0)
			input_layout_desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

		//위치정보 외의 정보 (color 나 uv정보)
	//요소가 float 1개
		else if (shader_input_param_desc.Mask == 1)
		{
			if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				input_layout_desc.Format = DXGI_FORMAT_R32_UINT;
			else if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				input_layout_desc.Format = DXGI_FORMAT_R32_SINT;
			else if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				input_layout_desc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		//요소가 float 2개(Vector2)
		else if (shader_input_param_desc.Mask <= 3)
		{
			if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				input_layout_desc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				input_layout_desc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				input_layout_desc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		//요소가 float 3개(Vector3)
		else if (shader_input_param_desc.Mask <= 7)
		{
			if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				input_layout_desc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				input_layout_desc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (shader_input_param_desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				input_layout_desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		//요소가 float 4개(Vector4)
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
