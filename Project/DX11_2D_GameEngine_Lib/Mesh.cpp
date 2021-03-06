#include "stdafx.h"
#include "Mesh.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

Mesh::Mesh(const std::string& mesh_resource_name)
	:IResource(ResourceType::Mesh, mesh_resource_name)
{
}

Mesh::~Mesh()
{
	//Vertex Vector 삭제
	m_vertex_vector.clear();
	m_vertex_vector.shrink_to_fit();

	//Index Vector 삭제
	m_index_vector.clear();
	m_index_vector.shrink_to_fit();
}

void Mesh::Create(const MeshType& mesh_type, const UINT& count_x, const UINT& count_y)
{
	m_mesh_type = mesh_type;

	switch (mesh_type)
	{
	case MeshType::Point:
		CreatePointMesh();
		break;
	case MeshType::Triangle:
		CreateTriangleMesh();
		break;
	case MeshType::Rectangle:
		CreateRectangleMesh();
		break;
	case MeshType::Circle:
		CreateCircleMesh();
		break;
	case MeshType::Grid:
		CreateGridMesh(count_x, count_y);
		break;
	}
}

void Mesh::BindPipeline()
{
	auto vertex_buffer = m_p_vertex_buffer->GetBuffer();
	auto stride = m_p_vertex_buffer->GetStride();
	auto offset = m_p_vertex_buffer->GetOffset();

	auto device_context = GRAPHICS_MANAGER->GetDeviceContext();
	device_context->IASetVertexBuffers
	(
		0,
		1,
		&vertex_buffer,
		&stride,
		&offset
	);

	auto index_buffer = m_p_index_buffer->GetBuffer();
	device_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
}

void Mesh::Render()
{
	BindPipeline();

	auto device_context = GRAPHICS_MANAGER->GetDeviceContext();
	device_context->DrawIndexed(static_cast<UINT>(m_index_vector.size()), 0, 0);
}

void Mesh::RenderInstance(const UINT& render_count)
{
	BindPipeline();

	auto device_context = GRAPHICS_MANAGER->GetDeviceContext();
	device_context->DrawIndexedInstanced(static_cast<UINT>(m_index_vector.size()), render_count, 0, 0, 0);
}

void Mesh::CreatePointMesh()
{
	//Vertex
	m_vertex_vector.reserve(1);
	{
		VertexColorTexture vertex;

		//월드 좌표계를 중점으로 한 점
		vertex.m_position = Vector3(0.0f, 0.0f, 0.0f);
		vertex.m_color = Vector4::White;
		vertex.m_uv = Vector2(0.0f, 0.0f);
		m_vertex_vector.emplace_back(vertex);
	}

	//Create Vertex Buffer
	if (m_p_vertex_buffer == nullptr)
		m_p_vertex_buffer = std::make_shared<VertexBuffer>();

	m_p_vertex_buffer->Create(m_vertex_vector);

	//Index
	m_index_vector.reserve(1);
	{
		//Point
		m_index_vector.emplace_back(0);
	}

	//Create Index Buffer
	if (m_p_index_buffer == nullptr)
		m_p_index_buffer = std::make_shared<IndexBuffer>();

	m_p_index_buffer->Create(m_index_vector);
}

void Mesh::CreateTriangleMesh()
{
	//정삼각형 아님
	m_vertex_vector.reserve(3);
	{
		VertexColorTexture vertex;

		//Middle Top Vertex(중앙 상단 정점)
		vertex.m_position = Vector3(0.0f, 0.5f, 0.0f);
		vertex.m_color = Vector4::White;
		vertex.m_uv = Vector2(0.5f, 0.0f);
		m_vertex_vector.emplace_back(vertex);

		//Right Bottom Vertex(우하단 정점)
		vertex.m_position = Vector3(0.5f, -0.5f, 0.0f);
		vertex.m_color = Vector4::White;
		vertex.m_uv = Vector2(1.0f, 1.0f);
		m_vertex_vector.emplace_back(vertex);

		//Left Bottom Vertex(좌하단 정점)
		vertex.m_position = Vector3(-0.5f, -0.5f, 0.0f);
		vertex.m_color = Vector4::White;
		vertex.m_uv = Vector2(0.0f, 1.0f);
		m_vertex_vector.emplace_back(vertex);
	}

	//Create Vertex Buffer
	if (m_p_vertex_buffer == nullptr)
		m_p_vertex_buffer = std::make_shared<VertexBuffer>();

	m_p_vertex_buffer->Create(m_vertex_vector);

	m_index_vector.reserve(4);
	{
		//Triangle
		m_index_vector.emplace_back(0);
		m_index_vector.emplace_back(1);
		m_index_vector.emplace_back(2);
		m_index_vector.emplace_back(0);
	}

	//Create Index Buffer
	if (m_p_index_buffer == nullptr)
		m_p_index_buffer = std::make_shared<IndexBuffer>();

	m_p_index_buffer->Create(m_index_vector);
}

void Mesh::CreateRectangleMesh()
{
	//Mesh의 Pivot을 사각형의 중심점으로 지정
	//Vertex
	m_vertex_vector.reserve(4);
	{
		VertexColorTexture vertex;

		//Left Top Vertex(좌상단 정점)
		vertex.m_position = Vector3(-0.5f, 0.5f, 0.0f);
		vertex.m_color = Vector4::White;
		vertex.m_uv = Vector2(0.0f, 0.0f);
		m_vertex_vector.emplace_back(vertex);

		//Right Top Vertex(우상단 정점)
		vertex.m_position = Vector3(0.5f, 0.5f, 0.0f);
		vertex.m_color = Vector4::White;
		vertex.m_uv = Vector2(1.0f, 0.0f);
		m_vertex_vector.emplace_back(vertex);

		//Right Bottom Vertex(우하단 정점)
		vertex.m_position = Vector3(0.5f, -0.5f, 0.0f);
		vertex.m_color = Vector4::White;
		vertex.m_uv = Vector2(1.0f, 1.0f);
		m_vertex_vector.emplace_back(vertex);

		//Left Bottom Vertex(좌하단 정점)
		vertex.m_position = Vector3(-0.5f, -0.5f, 0.0f);
		vertex.m_color = Vector4::White;
		vertex.m_uv = Vector2(0.0f, 1.0f);
		m_vertex_vector.emplace_back(vertex);
	}

	//Create Vertex Buffer
	if (m_p_vertex_buffer == nullptr)
		m_p_vertex_buffer = std::make_shared<VertexBuffer>();

	m_p_vertex_buffer->Create(m_vertex_vector);

	//Index
	m_index_vector.reserve(6);
	{
		//Triangle 1
		m_index_vector.emplace_back(0);
		m_index_vector.emplace_back(1);
		m_index_vector.emplace_back(2);

		//Triangle 2
		m_index_vector.emplace_back(2);
		m_index_vector.emplace_back(3);
		m_index_vector.emplace_back(0);
	}

	//Create Index Buffer
	if (m_p_index_buffer == nullptr)
		m_p_index_buffer = std::make_shared<IndexBuffer>();

	m_p_index_buffer->Create(m_index_vector);
}

void Mesh::CreateCircleMesh()
{
	//Vertex
	float radius = 0.5f;
	UINT slice_count = 100; //원을 나눌 조각 수
	float step_theta = XM_2PI / static_cast<float>(slice_count); //2PI = 360도, 조각당 각도

	//Vertex
	auto vertex_count = slice_count + 1;
	m_vertex_vector.reserve(static_cast<size_t>(vertex_count));
	{
		VertexColorTexture vertex;

		//Center Vertex(중앙 정점)
		vertex.m_position = Vector3(0.0f, 0.0f, 0.0f);
		vertex.m_color = Vector4::White;
		vertex.m_uv = Vector2(0.5f, 0.5f);
		m_vertex_vector.emplace_back(vertex);

		//Circle Vertex(반 시계 방향으로)
		float theta = 0.0f;
		for (UINT i = 0; i <= slice_count; ++i)
		{
			vertex.m_position = Vector3(radius * cosf(theta), radius * sinf(theta), 0.0f);
			vertex.m_color = Vector4::White;
			vertex.m_uv = Vector2(0.5f + (vertex.m_position.x / (2.0f * radius)), 0.5f - (vertex.m_position.y / (2.0f * radius)));

			m_vertex_vector.emplace_back(vertex);

			theta += step_theta;
		}
	}

	//Create Vertex Buffer
	if (m_p_vertex_buffer == nullptr)
		m_p_vertex_buffer = std::make_shared<VertexBuffer>();

	m_p_vertex_buffer->Create(m_vertex_vector);

	//Index
	auto index_count = slice_count * 3;
	m_index_vector.reserve(6);
	{
		for (UINT i = 0; i < slice_count; ++i)
		{
			//각 삼각형을 시계방향으로 그림
			m_index_vector.emplace_back(0);
			m_index_vector.emplace_back(i + 2);
			m_index_vector.emplace_back(i + 1);
		}
	}

	//Create Index Buffer
	if (m_p_index_buffer == nullptr)
		m_p_index_buffer = std::make_shared<IndexBuffer>();

	m_p_index_buffer->Create(m_index_vector);
}

void Mesh::CreateGridMesh(const UINT& count_x, const UINT& count_y)
{
	if (!m_vertex_vector.empty() && !m_index_vector.empty() && !m_grid_left_top.empty())
	{
		Clear();
		m_grid_left_top.clear();
	}

	float tile_per_width = static_cast<float>(1.0f / count_x); //타일 한 칸의 너비
	float tile_per_height = static_cast<float>(1.0f / count_y); //타일 한 칸의 높이

	UINT index = 0;
	//Vertex & Index
	//행
	for (UINT row = 0; row < count_y; ++row)
	{
		//열
		for (UINT column = 0; column < count_x; ++column)
		{
			VertexColorTexture vertex;

			//=============================================
			//Line1
			//=============================================
			//Left Top Vertex(좌상단 정점)
			vertex.m_position = Vector3(-0.5f + (column * tile_per_width), 0.5f - (row * tile_per_height), 0.0f);
			vertex.m_color = Vector4::White;
			vertex.m_uv = Vector2(0.0f, 0.0f);
			m_vertex_vector.emplace_back(vertex);
			m_index_vector.emplace_back(index);
			++index;

			//Grid Left Top
			m_grid_left_top.emplace_back(Vector2(vertex.m_position.x, vertex.m_position.y));

			//Right Top Vertex(우상단 정점)
			vertex.m_position = Vector3(-0.5f + ((column + 1) * tile_per_width), 0.5f - (row * tile_per_height), 0.0f);
			vertex.m_color = Vector4::White;
			vertex.m_uv = Vector2(0.0f, 0.0f);
			m_vertex_vector.emplace_back(vertex);
			m_index_vector.emplace_back(index);
			++index;

			//=============================================
			//Line2
			//=============================================
			//Right Top Vertex(우상단 정점)
			vertex.m_position = Vector3(-0.5f + ((column + 1) * tile_per_width), 0.5f - (row * tile_per_height), 0.0f);
			vertex.m_color = Vector4::White;
			vertex.m_uv = Vector2(0.0f, 0.0f);
			m_vertex_vector.emplace_back(vertex);
			m_index_vector.emplace_back(index);
			++index;

			//Right Bottom Vertex(우하단 정점)
			vertex.m_position = Vector3(-0.5f + ((column + 1) * tile_per_width), 0.5f - ((row + 1) * tile_per_height), 0.0f);
			vertex.m_color = Vector4::White;
			vertex.m_uv = Vector2(0.0f, 0.0f);
			m_vertex_vector.emplace_back(vertex);
			m_index_vector.emplace_back(index);
			++index;

			//=============================================
			//Line3
			//=============================================
			//Right Bottom Vertex(우하단 정점)
			vertex.m_position = Vector3(-0.5f + ((column + 1) * tile_per_width), 0.5f - ((row + 1) * tile_per_height), 0.0f);
			vertex.m_color = Vector4::White;
			vertex.m_uv = Vector2(0.0f, 0.0f);
			m_vertex_vector.emplace_back(vertex);
			m_index_vector.emplace_back(index);
			++index;

			//Left Bottom Vertex(좌하단 정점)
			vertex.m_position = Vector3(-0.5f + (column * tile_per_width), 0.5f - ((row + 1) * tile_per_height), 0.0f);
			vertex.m_color = Vector4::White;
			vertex.m_uv = Vector2(0.0f, 1.0f);
			m_vertex_vector.emplace_back(vertex);
			m_index_vector.emplace_back(index);
			++index;

			//=============================================
			//Line4
			//=============================================
			//Left Bottom Vertex(좌하단 정점)
			vertex.m_position = Vector3(-0.5f + (column * tile_per_width), 0.5f - ((row + 1) * tile_per_height), 0.0f);
			vertex.m_color = Vector4::White;
			vertex.m_uv = Vector2(0.0f, 1.0f);
			m_vertex_vector.emplace_back(vertex);
			m_index_vector.emplace_back(index);
			++index;

			//Left Top Vertex(좌상단 정점)
			vertex.m_position = Vector3(-0.5f + (column * tile_per_width), 0.5f - (row * tile_per_height), 0.0f);
			vertex.m_color = Vector4::White;
			vertex.m_uv = Vector2(0.0f, 0.0f);
			m_vertex_vector.emplace_back(vertex);
			m_index_vector.emplace_back(index);
			++index;
		}
	}

	//Create Vertex Buffer
	if (m_p_vertex_buffer == nullptr)
		m_p_vertex_buffer = std::make_shared<VertexBuffer>();

	m_p_vertex_buffer->Create(m_vertex_vector);

	//Create Index Buffer
	if (m_p_index_buffer == nullptr)
		m_p_index_buffer = std::make_shared<IndexBuffer>();

	m_p_index_buffer->Create(m_index_vector);
}

void Mesh::Clear()
{
	//Vertex Vector 삭제
	m_vertex_vector.clear();
	m_vertex_vector.shrink_to_fit();

	//Index Vector 삭제
	m_index_vector.clear();
	m_index_vector.shrink_to_fit();
}

bool Mesh::SaveToFile(const std::string& mesh_path)
{
	FILE* p_file = nullptr;
	fopen_s(&p_file, mesh_path.c_str(), "wb"); //파일 쓰기

	if (p_file != nullptr)
	{
		//Mesh Name
		fprintf(p_file, "[Mesh Name]\n");
		fprintf(p_file, "%s\n", m_object_name.c_str());

		//Mesh Type
		fprintf(p_file, "[Mesh Type]\n");
		auto mesh_type = static_cast<UINT>(m_mesh_type);
		fprintf(p_file, "%d\n", mesh_type);

		fclose(p_file);

		return true;
	}

	else
		return false;
}

bool Mesh::LoadFromFile(const std::string& mesh_path)
{
	FILE* p_file = nullptr;
	fopen_s(&p_file, mesh_path.c_str(), "rb"); //파일 읽기

	if (p_file != nullptr)
	{
		char char_buffer[256] = { 0 };

		//Mesh Name
		FILE_MANAGER->FScanf(char_buffer, p_file);
		FILE_MANAGER->FScanf(char_buffer, p_file);
		m_object_name = std::string(char_buffer);

		//Mesh Type
		FILE_MANAGER->FScanf(char_buffer, p_file);
		auto mesh_type = -1;
		fscanf_s(p_file, "%d\n", &mesh_type);
		m_mesh_type = static_cast<MeshType>(mesh_type);

		//Create Mesh
		if (m_mesh_type == MeshType::Grid)
			Create(m_mesh_type, 1, 1);

		else
			Create(m_mesh_type);

		fclose(p_file);

		return true;
	}

	else
		return false;
}
