#include "stdafx.h"
#include "Mesh.h"

Mesh::Mesh(const std::string& resource_name)
	:IResource(ResourceType::Mesh, resource_name)
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

void Mesh::Create(const MeshType& mesh_type, const UINT& width, const UINT& height)
{
	m_mesh_type = mesh_type;
	m_width = width;
	m_height = height;

	switch (mesh_type)
	{
	case MeshType::Triangle:
		CreateTriangleMesh();
		break;
	case MeshType::Rectangle:
		CreateRectangleMesh();
		break;
	case MeshType::Circle:
		CreateCircleMesh();
		break;
	}
}

void Mesh::BindPipeline()
{
	auto vertex_buffer = m_p_vertex_buffer->GetBuffer();
	auto stride = m_p_vertex_buffer->GetStride();
	auto offset = m_p_vertex_buffer->GetOffset();

	auto device_context = GraphicsManager::GetInstance()->GetDeviceContext();
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

	auto device_context = GraphicsManager::GetInstance()->GetDeviceContext();
	device_context->DrawIndexed(static_cast<UINT>(m_index_vector.size()), 0, 0);
}

void Mesh::CreateTriangleMesh()
{
	m_vertex_vector.reserve(3);
	m_index_vector.reserve(3);
}

void Mesh::CreateRectangleMesh()
{
    //Mesh의 Pivot을 사각형의 중심점으로 지정
	//Vertex
	m_vertex_vector.reserve(4);
	{
		VertexColorTexture vertex;

		//Left Top Vertex(좌상단 정점)
		vertex.position = Vector3(-(m_width * 0.5f), m_height * 0.5f, 0.0f);
		vertex.color = Color4::White;
		vertex.color.a = 0.0f;
		vertex.uv = Vector2(0.0f, 0.0f);
		m_vertex_vector.emplace_back(vertex);

		//Right Top Vertex(우상단 정점)
		vertex.position = Vector3(m_width * 0.5f, m_height * 0.5f, 0.0f);
		vertex.color = Color4::White;
		vertex.color.a = 0.0f;
		vertex.uv = Vector2(1.0f, 0.0f);
		m_vertex_vector.emplace_back(vertex);

		//Right Bottom Vertex(우하단 정점)
		vertex.position = Vector3(m_width * 0.5f, -(m_height * 0.5f), 0.0f);
		vertex.color = Color4::White;
		vertex.color.a = 0.0f;
		vertex.uv = Vector2(1.0f, 1.0f);
		m_vertex_vector.emplace_back(vertex);

		//Left Bottom Vertex(좌하단 정점)
		vertex.position = Vector3(-(m_width * 0.5f), -(m_height * 0.5f), 0.0f);
		vertex.color = Color4::White;
		vertex.color.a = 0.0f;
		vertex.uv = Vector2(0.0f, 1.0f);
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
	//mesh 사이즈(x와 y 값) 중 작은 사이즈를 반지름으로 결정
	float radius = m_width <= m_height ? static_cast<float>(m_width) : static_cast<float>(m_height);
	UINT slice_count = 100; //원을 나눌 조각 수
	float step_theta = XM_2PI / static_cast<float>(slice_count); //2PI = 360도, 조각당 각도
		
	//Vertex
	auto vertex_count = slice_count + 1;
	m_vertex_vector.reserve(static_cast<size_t>(vertex_count));
	{
		VertexColorTexture vertex;

		//Center Vertex(중앙 정점)
		vertex.position = Vector3(0.0f, 0.0f, 0.0f);
		vertex.color = Color4::White;
		vertex.color.a = 0.0f;
		vertex.uv = Vector2(0.5f, 0.5f);
		m_vertex_vector.emplace_back(vertex);

		//Circle Vertex(반 시계 방향으로)
		float theta = 0.0f;
		for (UINT i = 0; i <= slice_count; ++i)
		{
			vertex.position = Vector3(radius * cosf(theta), radius * sinf(theta), 0.0f);
			vertex.color = Color4::White;
			vertex.color.a = 0.0f;
			vertex.uv = Vector2(0.5f + (vertex.position.x / (2.0f * radius)) , 0.5f - (vertex.position.y / (2.0f * radius)));

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