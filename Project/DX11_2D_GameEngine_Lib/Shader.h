#pragma once

#include "IResource.h"
#include "IShader.h"

//하나의 그래픽스 파이프라인과 Compute Shader의 과정을 Shader Class로 정의
//여러 Shader(VS, HS, DS, GS, PS, CS)들을 조합하여 사용할 수 있기 때문
//CS Binding은 별도로 ComputeShader의 Dispatch에서 수행
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
	//Shader 추가 후 생성
	template<typename T>
	void AddShader
	(
		const std::string& path,			//Shader 파일 경로
		const std::string& function_name,	//Shader 파일에서 사용되는 함수이름
		const std::string& shader_version   //사용할 Shader 버전
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
	//Class T가 IShader를 상속받는 클래스인지 확인
	auto result = std::is_base_of<IShader, T>::value;
	assert(result);
	if (!result)
		return;
	

	//타입 T에 해당하는 Shader Type 반환
	auto shader_type = GetShaderType<T>();
	//un_map에 새로운 Shader 추가
	//insert에 성공하면 pair_ib<iterator, bool> 반환
	//iterator는 map 데이터를 참조하는 반복자, bool은 map에 데이터 삽입 성공여부
	auto shader_iter = m_shader_map.insert(std::make_pair(shader_type, std::make_shared<T>()));

	//해당하는 Type의 map에 성공적으로 데이터가 추가되었다면
	assert(shader_iter.second);
	if (shader_iter.second)
	{
		auto shader = std::dynamic_pointer_cast<T>(shader_iter.first->second);

		//타입 T에 해당하는 Shader 생성
		shader->Create(path, function_name, shader_version);
	}
}

template<typename T>
std::shared_ptr<T> Shader::GetShader() const
{
	//Class T가 IShader를 상속받는 클래스인지 확인
	auto result = std::is_base_of<IShader, T>::value;
	assert(result);
	if (!result)
	{
		return nullptr;
	}

	//타입 T에 해당하는 Shader Type 반환
	auto shader_type = GetShaderType<T>();
	auto shader_iter = m_shader_map.find(shader_type);

	//해당 Shader Type의 Shader 데이터가 존재하는 경우
	assert(shader_iter != m_shader_map.end());
	//타입 T에 해당하는 Shader가 존재하는 경우
	if (shader_iter->second != nullptr)
	{
		return std::dynamic_pointer_cast<T>(shader_iter->second);
	}

	return nullptr;
}
