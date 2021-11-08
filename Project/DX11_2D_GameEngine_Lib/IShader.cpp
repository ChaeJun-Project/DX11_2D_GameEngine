#include "stdafx.h"
#include "IShader.h"

const bool IShader::GetShaderError(const HRESULT& hResult, ID3DBlob* shader_error)
{
	if (FAILED(hResult))
	{
		if (shader_error)
		{
		   std::string error_str = static_cast<const char*>(shader_error->GetBufferPointer());

		   std::string shader_type;

		   switch (this->m_shader_type)
		   {
		   case ShaderType::VS:
			   shader_type = "Vertex Shader";
			   break;
		   case ShaderType::HS:
			   shader_type = "Hull Shader";
			   break;
		   case ShaderType::DS:
			   shader_type = "Domain Shader";
			   break;
		   case ShaderType::GS:
			   shader_type = "Geometry Shader";
			   break;
		   case ShaderType::PS:
			   shader_type = "Pixel Shader";
			   break;
		   case ShaderType::CS:
			   shader_type = "Compute Shader";
			   break;
		   }

		   shader_type += " Error!";

		   //Shader 컴파일 시 오류 메시지를 띄움
		   MessageBoxA(nullptr, error_str.c_str(), shader_type.c_str() , MB_OK);
		}
		return false;
	}

	return true;
}

const bool IShader::CompileShader(const std::string& path, const std::string& function_name, const std::string& shader_version, ID3DBlob** blob)
{
#ifdef _DEBUG
	UINT flags = D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS;
#else
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

	ComPtr<ID3DBlob> p_error_blob = nullptr;
	
	//D3D11ComplieFromFile은 Windows 8 이후 버전부터는 지원햐지 않으므로
	//D3DCompileFromFile을 사용
	auto hResult = D3DCompileFromFile
	(
		FileManager::ConvertStringToWString(path).c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		function_name.c_str(),
		shader_version.c_str(),
		flags,
		0,
		blob,
		p_error_blob.GetAddressOf()
	);

	//Error Check
	auto result = GetShaderError(hResult, p_error_blob.Get());
	assert(result);

	return result;
}
