#pragma once

#include "IResource.h"
#include "IShader.h"

//�ϳ��� �׷��Ƚ� ���������ΰ� Compute Shader�� ������ Shader Class�� ����
//���� Shader(VS, HS, DS, GS, PS, CS)���� �����Ͽ� ����� �� �ֱ� ����
//CS Binding�� ������ ComputeShader�� Dispatch���� ����
class Shader final : public IResource
{
private:
	template<typename T>
	static constexpr ShaderType GetShaderType();

public:
	Shader(const std::string& shader_resource_name);
	explicit Shader(const Shader& origin) = default;
	~Shader();

	void BindPipeline() override;

public:
	//Shader �߰� �� ����
	template<typename T>
	void AddShader
	(
		const std::string& path,			//Shader ���� ���
		const std::string& function_name,	//Shader ���Ͽ��� ���Ǵ� �Լ��̸�
		const std::string& shader_version   //����� Shader ����
	);

	template<typename T>
	std::shared_ptr<T> GetShader() const;

public:
	RenderTimePointType GetRenderTimePointType() const { return m_render_time_point; }
	void SetRenderTimePointType(const RenderTimePointType& render_time_point) { m_render_time_point = render_time_point; }

	UINT GetShaderBindStage() const { return m_shader_bind_stage; }
	void SetShaderBindStage(const UINT& shader_bind_stage) { m_shader_bind_stage = shader_bind_stage; }

	const D3D11_PRIMITIVE_TOPOLOGY& GetPrimitiveTopology() const { return m_primitive_topology; }
	void SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY& primitive_topology) { m_primitive_topology = primitive_topology; }

	const RasterizerType& GetRasterizerType() const { return m_rasterizer_type; }
	void SetRasterizerType(const RasterizerType& rasterizer_type) { m_rasterizer_type = rasterizer_type; }

	const BlendType& GetBlendType() const { return m_blend_type; }
	void SetBlendType(const BlendType& blend_type) { m_blend_type = blend_type; }

	const DepthStencilType& GetDepthStencilType() const { return m_depth_stencil_type; }
	void SetDepthStencilType(const DepthStencilType& depth_stencil_type) { m_depth_stencil_type = depth_stencil_type; }

public:
	CLONE(Shader);

private:
    std::map<ShaderType, std::shared_ptr<IShader>> m_shader_map;

	RenderTimePointType m_render_time_point = RenderTimePointType::NONE;

	UINT m_shader_bind_stage = 0;

	D3D11_PRIMITIVE_TOPOLOGY m_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	RasterizerType m_rasterizer_type;
	BlendType m_blend_type;
	DepthStencilType m_depth_stencil_type;
};

template<typename T>
void Shader::AddShader(const std::string& path, const std::string& function_name, const std::string& shader_version)
{
	//Class T�� IShader�� ��ӹ޴� Ŭ�������� Ȯ��
	auto result = std::is_base_of<IShader, T>::value;
	assert(result);
	if (!result)
		return;
	

	//Ÿ�� T�� �ش��ϴ� Shader Type ��ȯ
	auto shader_type = GetShaderType<T>();
	//un_map�� ���ο� Shader �߰�
	//insert�� �����ϸ� pair_ib<iterator, bool> ��ȯ
	//iterator�� map �����͸� �����ϴ� �ݺ���, bool�� map�� ������ ���� ��������
	auto shader_iter = m_shader_map.insert(std::make_pair(shader_type, std::make_shared<T>()));

	//�ش��ϴ� Type�� map�� ���������� �����Ͱ� �߰��Ǿ��ٸ�
	assert(shader_iter.second);
	if (shader_iter.second)
	{
		auto shader = std::dynamic_pointer_cast<T>(shader_iter.first->second);

		//Ÿ�� T�� �ش��ϴ� Shader ����
		shader->Create(path, function_name, shader_version);
	}
}

template<typename T>
std::shared_ptr<T> Shader::GetShader() const
{
	//Class T�� IShader�� ��ӹ޴� Ŭ�������� Ȯ��
	auto result = std::is_base_of<IShader, T>::value;
	assert(result);
	if (!result)
	{
		return nullptr;
	}

	//Ÿ�� T�� �ش��ϴ� Shader Type ��ȯ
	auto shader_type = GetShaderType<T>();
	auto shader_iter = m_shader_map.find(shader_type);

	//�ش� Shader Type�� Shader �����Ͱ� �����ϴ� ���
	assert(shader_iter != m_shader_map.end());
	//Ÿ�� T�� �ش��ϴ� Shader�� �����ϴ� ���
	if (shader_iter->second != nullptr)
	{
		return std::dynamic_pointer_cast<T>(shader_iter->second);
	}

	return nullptr;
}
