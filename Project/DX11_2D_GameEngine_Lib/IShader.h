#pragma once

//D3DCOMPILE 옵션 설명
//https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/d3dcompile-constants

//각종 Shader 생성에 필요한 공통적인 멤버 함수를 포함하고
//생성에 관한 멤버 함수의 정의는 자식 클래스에서 정의(다형성 사용)
class IShader
{
public:
	IShader(const ShaderType& shader_type)
	:m_shader_type(shader_type)
	{
	}

	virtual ~IShader() = default;

	//순수 가상함수
	virtual void Create
	(
		const std::string& path,			//Shader 파일 경로
		const std::string& function_name,	//Shader 파일에서 사용되는 함수이름
		const std::string& shader_version   //사용할 Shader 버전
	) = 0;

protected:
    //Shader 컴파일 시 오류 메시지를 띄움
    const bool GetShaderError(const HRESULT& hResult, ID3DBlob* shader_error);
	//Shader 컴파일
	const bool CompileShader
	(
		const std::string& path,
		const std::string& function_name,
		const std::string& shader_version,
		ID3DBlob** blob
	);

public:
	const ShaderType& GetShaderType() const { return m_shader_type; }

protected:
    const ShaderType m_shader_type;

	std::string m_path;
	std::string m_function_name;
	std::string m_shader_version;
};

