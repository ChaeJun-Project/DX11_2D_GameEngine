#pragma once

//D3DCOMPILE �ɼ� ����
//https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/d3dcompile-constants

//���� Shader ������ �ʿ��� �������� ��� �Լ��� �����ϰ�
//������ ���� ��� �Լ��� ���Ǵ� �ڽ� Ŭ�������� ����(������ ���)
class IShader
{
public:
	IShader(const ShaderType& shader_type)
	:m_shader_type(shader_type)
	{
	}

	virtual ~IShader() = default;

	//���� �����Լ�
	virtual void Create
	(
		const std::string& path,			//Shader ���� ���
		const std::string& function_name,	//Shader ���Ͽ��� ���Ǵ� �Լ��̸�
		const std::string& shader_version   //����� Shader ����
	) = 0;

protected:
    //Shader ������ �� ���� �޽����� ���
    const bool GetShaderError(const HRESULT& hResult, ID3DBlob* shader_error);
	//Shader ������
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

