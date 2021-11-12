#pragma once

#include "IResource.h"

class Mesh final : public IResource
{
public:
	Mesh(const std::string& resource_name);
	~Mesh();

	void Create(const MeshType& mesh_type);

	void BindPipeline() override;

	void Render();
	void RenderInstance(const UINT& render_count);

private:
    void CreatePointMesh();
	void CreateTriangleMesh();
	void CreateRectangleMesh();
	void CreateCircleMesh();

public:
	const std::vector<VertexColorTexture> GetVertexVector() const { return this->m_vertex_vector; }
	const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const { SAFE_GET_POINTER(this->m_p_vertex_buffer); }

	const std::vector<UINT> GetIndexVector() const { return this->m_index_vector; }
	const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { SAFE_GET_POINTER(this->m_p_index_buffer); }

	const MeshType& GetMeshType() const { return this->m_mesh_type; }

private:
	std::vector<VertexColorTexture> m_vertex_vector;
	std::shared_ptr<VertexBuffer> m_p_vertex_buffer = nullptr;

	std::vector<UINT> m_index_vector;
	std::shared_ptr<IndexBuffer> m_p_index_buffer = nullptr;

	MeshType m_mesh_type = MeshType::Point;
};