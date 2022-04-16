#include "stdafx.h"
#include "Particle.h"

#include "GraphicsManager.h"

#include "ConstantBuffer.h"
#include "StructuredBuffer.h"
#include "ComputeShader.h"

Particle::Particle(const std::string& particle_resource_name)
	:IResource(ResourceType::Particle, particle_resource_name)
{
	ZeroMemory(&m_particle_property, sizeof(ParticleProperty));
	ZeroMemory(&m_particle_property_data, sizeof(CBuffer_Material));

	//Create Particle Structured Buffer
	m_p_particle_buffer = std::make_unique<StructuredBuffer>();
}

Particle::Particle(const Particle& origin)
	:IResource(origin.m_resource_type, origin.m_object_name)
{
	//Resource Path
	m_resource_path = origin.m_resource_path;

	m_p_compute_shader = origin.m_p_compute_shader;

	m_p_particle_buffer = std::make_unique<StructuredBuffer>();

	m_p_particle_texture = origin.m_p_particle_texture;

	m_particle_property = origin.m_particle_property;
	m_particle_property_data = origin.m_particle_property_data;
}

Particle::~Particle()
{
	m_p_compute_shader.reset();

	m_p_particle_shared_buffer.reset();
	m_p_particle_buffer.reset();

	m_p_particle_texture.reset();
}

void Particle::Update()
{
	if (m_p_compute_shader == nullptr || m_p_particle_buffer->GetElementCount() == 0)
		return;

	UpdateConstantBuffer();

	BindPipeline();

	Dispatch();

	//Compute Shader에 등록한 구조화 버퍼 등록 해제
	m_p_particle_buffer->ClearRW();
}

void Particle::BindPipeline()
{
	//Compute Shader에 Particle Shared Buffer(구조화 버퍼) 등록
	m_p_particle_shared_buffer->BindPipelineRW(0);
	//Compute Shader에 Particle Buffer(구조화 버퍼) 등록
	m_p_particle_buffer->BindPipelineRW(1);
}

void Particle::UpdateConstantBuffer()
{
	m_particle_property_data.i_array[0] = m_particle_property.max_count;

	auto constant_buffer = GRAPHICS_MANAGER->GetConstantBuffer(CBuffer_BindSlot::Material);
	constant_buffer->SetConstantBufferData(&m_particle_property_data, sizeof(CBuffer_Material));
	constant_buffer->SetBufferBindStage(PipelineStage::CS);
	constant_buffer->BindPipeline();
}

void Particle::Dispatch()
{
	if (m_p_compute_shader->GetGroupThreadXCount() == 0)
		return;

	UINT thread_group_x_count = 0;

	if ((m_particle_property.max_count % m_p_compute_shader->GetGroupThreadXCount()) == 0)
		thread_group_x_count = m_particle_property.max_count / m_p_compute_shader->GetGroupThreadXCount();

	else
		thread_group_x_count = (m_particle_property.max_count / m_p_compute_shader->GetGroupThreadXCount()) + 1;

	m_p_compute_shader->Dispatch(thread_group_x_count, 1, 1);
}

void Particle::CreateParticleBuffer()
{
	m_p_particle_buffer->Create<ParticleInfo>(m_particle_property.max_count, SBufferType::Read_Write, false);
}

void Particle::SetComputeShader(const std::shared_ptr<ComputeShader>& p_compute_shader)
{
	m_p_compute_shader = p_compute_shader;

	if(m_p_compute_shader != nullptr)
		m_p_compute_shader->SetGroupThreadCount(100, 1, 1);
}

bool Particle::SaveToFile(const std::string& tile_map_path)
{
	FILE* p_file = nullptr;
	fopen_s(&p_file, tile_map_path.c_str(), "wb"); //파일 쓰기

	if (p_file != nullptr)
	{
		//Compute Shader
		fprintf(p_file, "[Compute Shader]\n");
		fprintf(p_file, "%s\n", m_object_name.c_str());

		//Particle Texture
		fprintf(p_file, "[Particle Texture]\n");
		RESOURCE_MANAGER->SaveResource<Texture>(m_p_particle_texture, p_file);

		//Particle Property
		//Particle Count
		fprintf(p_file, "[Particle Activable Count]\n");
		fprintf(p_file, "%d\n", m_particle_property.activable_count);
		fprintf(p_file, "[Particle Max Count]\n");
		fprintf(p_file, "%d\n", m_particle_property.max_count);

		//Particle Spawn Range
		fprintf(p_file, "[Particle Spawn Range]\n");
		FILE_MANAGER->FPrintf_Vector3(m_particle_property.spawn_range, p_file);

		//Particle Scale
		fprintf(p_file, "[Particle Start Scale]\n");
		FILE_MANAGER->FPrintf_Vector3(m_particle_property.start_scale, p_file);

		fprintf(p_file, "[Particle End Scale]\n");
		FILE_MANAGER->FPrintf_Vector3(m_particle_property.end_scale, p_file);

		//Particle Color
		fprintf(p_file, "[Particle Start Color]\n");
		FILE_MANAGER->FPrintf_Vector4(m_particle_property.start_color, p_file);

		fprintf(p_file, "[Particle End Color]\n");
		FILE_MANAGER->FPrintf_Vector4(m_particle_property.end_color, p_file);

		//Particle Speed
		fprintf(p_file, "[Particle Min Speed]\n");
		fprintf(p_file, "%f\n", m_particle_property.min_speed);
		fprintf(p_file, "[Particle Max Speed]\n");
		fprintf(p_file, "%f\n", m_particle_property.max_speed);

		//Particle Life
		fprintf(p_file, "[Particle Min Life]\n");
		fprintf(p_file, "%f\n", m_particle_property.min_life);
		fprintf(p_file, "[Particle Max Life]\n");
		fprintf(p_file, "%f\n", m_particle_property.max_life);

		//Particle Spawn Frequency
		fprintf(p_file, "[Particle Spawn Frequency]\n");
		fprintf(p_file, "%f\n", m_particle_property.spawn_frequency);

		fclose(p_file);
		return true;
	}

	else
		return false;
}

bool Particle::LoadFromFile(const std::string& tile_map_path)
{
	FILE* p_file = nullptr;
	fopen_s(&p_file, tile_map_path.c_str(), "rb"); //파일 읽기

	if (p_file != nullptr)
	{
		char char_buffer[256] = {};

		//Compute Shader
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Compute Shader]
		FILE_MANAGER->FScanf(char_buffer, p_file);
		auto compute_shader_name = std::string(char_buffer);
		SetComputeShader(RESOURCE_MANAGER->GetComputeShader(compute_shader_name));

		//Particle Texture
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Particle Texture]
		RESOURCE_MANAGER->LoadResource<Texture>(m_p_particle_texture, p_file);

		//Particle Property
		//Particle Count
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Particle Activable Count]
		fscanf_s(p_file, "%d\n", &(m_particle_property.activable_count));

		FILE_MANAGER->FScanf(char_buffer, p_file); //[Particle Max Count]
		fscanf_s(p_file, "%d\n", &(m_particle_property.max_count));

		//Particle Spawn Range
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Particle Spawn Range]
		FILE_MANAGER->FScanf_Vector3(m_particle_property.spawn_range, p_file);
		m_particle_property_data.v4_array[1] = m_particle_property.spawn_range;

		//Particle Scale
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Particle Start Scale]
		FILE_MANAGER->FScanf_Vector3(m_particle_property.start_scale, p_file);
		m_particle_property_data.v4_array[2] = m_particle_property.start_scale;

		FILE_MANAGER->FScanf(char_buffer, p_file); //[Particle End Scale]
		FILE_MANAGER->FScanf_Vector3(m_particle_property.end_scale, p_file);
		m_particle_property_data.v4_array[3] = m_particle_property.end_scale;

		//Particle Color
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Particle Start Color]
		FILE_MANAGER->FScanf_Vector4(m_particle_property.start_color, p_file);
		m_particle_property_data.v4_array[4] = m_particle_property.start_color;

		FILE_MANAGER->FScanf(char_buffer, p_file); //[Particle End Color]
		FILE_MANAGER->FScanf_Vector4(m_particle_property.end_color, p_file);
		m_particle_property_data.v4_array[5] = m_particle_property.end_color;

		//Particle Speed
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Particle Min Speed]
		fscanf_s(p_file, "%f\n", &(m_particle_property.min_speed));
		m_particle_property_data.f_array[0] = m_particle_property.min_speed;

		FILE_MANAGER->FScanf(char_buffer, p_file); //[Particle Max Speed]
		fscanf_s(p_file, "%f\n", &(m_particle_property.max_speed));
		m_particle_property_data.f_array[1] = m_particle_property.max_speed;

		//Particle Life
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Particle Min Life]
		fscanf_s(p_file, "%f\n", &(m_particle_property.min_life));
		m_particle_property_data.f_array[2] = m_particle_property.min_life;

		FILE_MANAGER->FScanf(char_buffer, p_file); //[Particle Max Life]
		fscanf_s(p_file, "%f\n", &(m_particle_property.max_life));
		m_particle_property_data.f_array[3] = m_particle_property.max_life;

		//Particle Spawn Frequency
		FILE_MANAGER->FScanf(char_buffer, p_file); //[Particle Spawn Frequency]
		fscanf_s(p_file, "%f\n", &(m_particle_property.spawn_frequency));

		fclose(p_file);

		return true;
	}

	else
		return false;
}
