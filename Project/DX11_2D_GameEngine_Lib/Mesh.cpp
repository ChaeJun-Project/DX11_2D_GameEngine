#include "stdafx.h"
#include "Mesh.h"

Mesh::Mesh(const std::string& resource_name)
	:IResource(ResourceType::Mesh, resource_name)
{
}

Mesh::~Mesh()
{
	//Vertex Vector ����
	this->m_vertex_vector.clear();
	this->m_vertex_vector.shrink_to_fit();

	//Index Vector ����
	this->m_index_vector.clear();
	this->m_index_vector.shrink_to_fit();
}

void Mesh::Create(const MeshType& mesh_type, const Vector2& mesh_size)
{
	this->m_mesh_type = mesh_type;
	this->m_mesh_size = mesh_size;

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
	auto vertex_buffer = this->m_p_vertex_buffer->GetBuffer();
	auto stride = this->m_p_vertex_buffer->GetStride();
	auto offset = this->m_p_vertex_buffer->GetOffset();

	auto device_context = GraphicsManager::GetInstance()->GetDeviceContext();
	device_context->IASetVertexBuffers
	(
		0,
		1,
		&vertex_buffer,
		&stride,
		&offset
	);

	auto index_buffer = this->m_p_index_buffer->GetBuffer();
	device_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
}

void Mesh::Render()
{
	BindPipeline();

	auto device_context = GraphicsManager::GetInstance()->GetDeviceContext();
	device_context->DrawIndexed(static_cast<UINT>(this->m_index_vector.size()), 0, 0);
}

void Mesh::CreateTriangleMesh()
{
	this->m_vertex_vector.reserve(3);
	this->m_index_vector.reserve(3);
}

void Mesh::CreateRectangleMesh()
{
    //Mesh�� Pivot�� �簢���� �߽������� ����
	//Vertex
	this->m_vertex_vector.reserve(4);
	{
		VertexColorTexture vertex;

		//Left Top Vertex(�»�� ����)
		vertex.position = Vector3(-(this->m_mesh_size.x * 0.5f), this->m_mesh_size.y * 0.5f, 0.0f);
		vertex.color = Color4::White;
		vertex.color.a = 0.0f;
		vertex.uv = Vector2(0.0f, 0.0f);
		this->m_vertex_vector.emplace_back(vertex);

		//Right Top Vertex(���� ����)
		vertex.position = Vector3(this->m_mesh_size.x * 0.5f, this->m_mesh_size.y * 0.5f, 0.0f);
		vertex.color = Color4::White;
		vertex.color.a = 0.0f;
		vertex.uv = Vector2(1.0f, 0.0f);
		this->m_vertex_vector.emplace_back(vertex);

		//Right Bottom Vertex(���ϴ� ����)
		vertex.position = Vector3(this->m_mesh_size.x * 0.5f, -(this->m_mesh_size.y * 0.5f), 0.0f);
		vertex.color = Color4::White;
		vertex.color.a = 0.0f;
		vertex.uv = Vector2(1.0f, 1.0f);
		this->m_vertex_vector.emplace_back(vertex);

		//Left Bottom Vertex(���ϴ� ����)
		vertex.position = Vector3(-(this->m_mesh_size.x * 0.5f), -(this->m_mesh_size.y * 0.5f), 0.0f);
		vertex.color = Color4::White;
		vertex.color.a = 0.0f;
		vertex.uv = Vector2(0.0f, 1.0f);
		this->m_vertex_vector.emplace_back(vertex);
	}

	//Create Vertex Buffer
	if (this->m_p_vertex_buffer == nullptr)
		this->m_p_vertex_buffer = std::make_shared<VertexBuffer>();

	this->m_p_vertex_buffer->Create(this->m_vertex_vector);

	//Index
	this->m_index_vector.reserve(6);
	{
		//Triangle 1
		this->m_index_vector.emplace_back(0);
		this->m_index_vector.emplace_back(1);
		this->m_index_vector.emplace_back(2);

		//Triangle 2
		this->m_index_vector.emplace_back(2);
		this->m_index_vector.emplace_back(3);
		this->m_index_vector.emplace_back(0);
	}

	//Create Index Buffer
	if (this->m_p_index_buffer == nullptr)
		this->m_p_index_buffer = std::make_shared<IndexBuffer>();

	this->m_p_index_buffer->Create(this->m_index_vector);
}

void Mesh::CreateCircleMesh()
{
	//Vertex
	//mesh ������(x�� y ��) �� ���� ����� ���������� ����
	float radius = this->m_mesh_size.x <= this->m_mesh_size.y ? this->m_mesh_size.x : this->m_mesh_size.y;
	UINT slice_count = 100; //���� ���� ���� ��
	float step_theta = XM_2PI / static_cast<float>(slice_count); //2PI = 360��, ������ ����
		
	//Vertex
	auto vertex_count = slice_count + 1;
	this->m_vertex_vector.reserve(static_cast<size_t>(vertex_count));
	{
		VertexColorTexture vertex;

		//Center Vertex(�߾� ����)
		vertex.position = Vector3(0.0f, 0.0f, 0.0f);
		vertex.color = Color4::White;
		vertex.color.a = 0.0f;
		vertex.uv = Vector2(0.5f, 0.5f);
		this->m_vertex_vector.emplace_back(vertex);

		//Circle Vertex(�� �ð� ��������)
		float theta = 0.0f;
		for (UINT i = 0; i <= slice_count; ++i)
		{
			vertex.position = Vector3(radius * cosf(theta), radius * sinf(theta), 0.0f);
			vertex.color = Color4::White;
			vertex.color.a = 0.0f;
			vertex.uv = Vector2(0.5f + (vertex.position.x / (2.0f * radius)) , 0.5f - (vertex.position.y / (2.0f * radius)));

			this->m_vertex_vector.emplace_back(vertex);
			
			theta += step_theta;
		}
	}

	//Create Vertex Buffer
	if (this->m_p_vertex_buffer == nullptr)
		this->m_p_vertex_buffer = std::make_shared<VertexBuffer>();

	this->m_p_vertex_buffer->Create(this->m_vertex_vector);

	//Index
	auto index_count = slice_count * 3;
	this->m_index_vector.reserve(6);
	{
		for (UINT i = 0; i < slice_count; ++i)
		{
			//�� �ﰢ���� �ð�������� �׸�
			this->m_index_vector.emplace_back(0);
			this->m_index_vector.emplace_back(i + 2);
			this->m_index_vector.emplace_back(i + 1);
		}
	}

	//Create Index Buffer
	if (this->m_p_index_buffer == nullptr)
		this->m_p_index_buffer = std::make_shared<IndexBuffer>();

	this->m_p_index_buffer->Create(this->m_index_vector);
}